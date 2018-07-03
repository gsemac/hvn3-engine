#include "hvn3/exceptions/Exception.h"
#include "hvn3/io/BitWriter.h"

namespace hvn3 {
	namespace IO {

		// Public methods

		BitWriter::BitWriter() {

			_stream = nullptr;
			_owns_stream = false;
			_buffer = nullptr;
			_byte_offset = 0;
			_bit_offset = 0;

			// Set buffer to 64-bits initially, which should be enough to contain most primitive types.
			_buffer_size = 8;
			AllocateBuffer(_buffer_size);

		}
		BitWriter::BitWriter(StreamBase& stream) : BitWriter() {

			_stream = &stream;

		}
		BitWriter::~BitWriter() {

			// Flush writes to the underlying stream.
			FlushWrite();

			// Free the write buffer if one has been allocated.
			if (_buffer)
				free(_buffer);

		}

		StreamBase& BitWriter::BaseStream() {

			// If there is no stream, throw an exception.
			if (!_stream)
				throw System::IO::IOException();

			return *_stream;

		}

		void BitWriter::Close() {

			// If there is no stream, throw an exception.
			if (!_stream)
				throw System::IO::IOException();

			// Flush writes to the underlying stream.
			FlushWrite();

			// Close the underlying stream.
			_stream->Close();

			// Set the underlying stream to null.
			_stream = nullptr;

		}
		void BitWriter::Flush() {

			// If there is no stream, throw an exception.
			if (!_stream)
				throw System::IO::IOException();

			// Flush the write buffer.
			FlushWrite();

			// Flush the underlying stream.
			_stream->Flush();

		}
		void BitWriter::Seek(long long position, SeekOrigin offset) {

			// If there is no stream or the stream does not support seeking, throw error.
			if (!_stream || !_stream->CanSeek())
				throw System::NotSupportedException();

			// Flush any data in the write buffer to the stream before seeking to a new position.
			FlushWrite();

			// Seek the underlying stream.
			_stream->Seek(position, offset);

		}
		void BitWriter::Seek(long long position) {

			Seek(position, SeekOrigin::Begin);

		}
		void BitWriter::SeekBits(long long bits, SeekOrigin offset) {

			// Throw an exception of the underlying stream does not support reading.
			// We will need to read from the stream into the write buffer in order to modify individual bits.
			if (!_stream || !_stream->CanRead())
				throw System::NotSupportedException();

			// Convert the offset into one relative to the start of the stream.
			switch (offset) {
			case SeekOrigin::Current:
				bits += _stream->Position() * 8;
				bits += _bit_offset;
				break;
			case SeekOrigin::End:
				bits += _stream->Length() * 8;
				break;
			}

			// Get the number of bytes that we'll need to advance into the stream.
			long long bytes = bits / 8;

			// Calculate the remaining bit offset.
			bits -= bytes * 8;

			// Seek by the number of bytes.
			Seek(bytes);

			// Fill the write buffer with data from the stream.
			long long bytes_read = _stream->Read(_buffer, 0, _buffer_size);

			// Seek back to the original position.
			Seek(-bytes_read, SeekOrigin::Current);

			// Set the bit and byte offsets.
			_bit_offset = static_cast<Byte>(bits);
			_byte_offset = 0;

		}
		void BitWriter::SeekBits(long long bits) {

			SeekBits(bits, SeekOrigin::Begin);

		}

		void BitWriter::WriteBool(bool value) {

			WriteBits(value, 1);

		}
		void BitWriter::WriteByte(Byte value, Byte min, Byte max) {

			WriteBits(value, BitUtils::BitsRequired(min, max));

		}
		void BitWriter::WriteBytes(Byte* value, size_t length) {

			WriteBytes(value, 0, length);

		}
		void BitWriter::WriteBytes(Byte* value, size_t offset, size_t length) {

			// Increment the pointer by the offset value.
			value += offset;

			// Write bytes from the array.
			for (size_t i = 0; i < length; ++i)
				WriteByte(*(value++));

		}
		void BitWriter::WriteChar(signed char value, signed char min, signed char max) {

			WriteInteger(value, min, max);

		}
		void BitWriter::WriteString(const char* value) {

			size_t length = std::strlen(value) + 1;
			WriteString(value, length);

		}
		void BitWriter::WriteString(const char* value, size_t length) {

			WriteString(value, 0, length);

		}
		void BitWriter::WriteString(const char* value, size_t offset, size_t length) {

			size_t index = offset;
			for (size_t i = 0; i < length; ++i)
				WriteByte(*(value + index++));

		}
		void BitWriter::WriteString(const std::string& value) {

			WriteString(value.c_str());

		}
		void BitWriter::WriteFloat(float value) {

			union FloatInt {
				unsigned int i;
				float f;
			} float_int;

			float_int.f = value;
			WriteInteger(float_int.i);

		}
		void BitWriter::WriteInteger(unsigned int value, unsigned int min, unsigned int max) {

			WriteBits(value - min, BitUtils::BitsRequired(min, max));

		}
		void BitWriter::WriteInteger(signed int value, signed int min, signed int max) {

			unsigned int uvalue = value + INT_MAX;
			unsigned int umin = min + INT_MAX + 1;
			unsigned int umax = max + INT_MAX + 1;

			WriteInteger(uvalue, umin, umax);

		}
		void BitWriter::WriteShort(unsigned short value, unsigned short min, unsigned short max) {

			WriteInteger(value, min, max);

		}
		void BitWriter::WriteShort(signed short value, signed short min, signed short max) {

			WriteInteger(value, min, max);

		}

		// Protected methods

		void BitWriter::FlushWrite() {

			// Write the buffer to the underlying stream. If we've written any bits to the current byte, flush it.
			size_t length = _byte_offset + (_bit_offset > 0);
			if (_buffer && length > 0)
				_stream->Write(_buffer, 0, length);

			// Reset the buffer.
			ClearBuffer();

		}
		void BitWriter::AllocateBuffer(size_t bytes) {

			// Create a new buffer.
			Byte* new_buffer = (Byte*)calloc(bytes, sizeof(Byte));

			// If the buffer isn't empty, copy the contents of the old buffer into the new buffer.
			if (_buffer != nullptr && (_byte_offset > 0 || _bit_offset > 0))
				memcpy(new_buffer, _buffer, _byte_offset > 0 ? _byte_offset : 1);

			// Free the old buffer (if it exists).
			if (_buffer != nullptr)
				free(_buffer);

			// Apply the new buffer.
			_buffer = new_buffer;

		}
		void BitWriter::ClearBuffer() {

			// Zero-out the portion of the buffer we've written to.
			size_t length = _byte_offset + (_bit_offset > 0);
			memset(_buffer, 0, length);

			// Reset buffer offsets.
			_byte_offset = 0;
			_bit_offset = 0;

		}
		size_t BitWriter::BitsRemaining() const {

			return (_buffer_size * 8) - (_byte_offset * 8 + _bit_offset);

		}
		void BitWriter::IncrementBitOffset() {

			if (_bit_offset == 7) {

				// If we've reached the end of the byte, reset bit count, increment byte count.
				_bit_offset = 0;
				++_byte_offset;

				// If we've reached the end of the buffer, flush write.
				if (_byte_offset == _buffer_size)
					FlushWrite();

			}
			else
				++_bit_offset;

		}
		void BitWriter::WriteBits(uint32_t value, int bits) {

			// If we're writing a single byte and the bit offset is 0, write it directly.
			if (bits == 8 && !_bit_offset) {
				_buffer[_byte_offset++] = value;
				if (_byte_offset == _buffer_size)
					FlushWrite();
				return;
			}

			// Write "bits" least-significant bits from the value.
			for (int i = bits - 1; i >= 0; --i) {
				_buffer[_byte_offset] |= ((value >> i) & 1) << (7 - _bit_offset);
				IncrementBitOffset();
			}

		}

	}
}