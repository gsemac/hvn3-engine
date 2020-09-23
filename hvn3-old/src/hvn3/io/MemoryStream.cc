#include "hvn3/io/MemoryStream.h"

#include "hvn3/io/IOException.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace hvn3 {
	namespace IO {

		// Public methods

		MemoryStream::MemoryStream() : MemoryStream(0) {}
		MemoryStream::MemoryStream(size_t capacity) {

			_length = 0;
			_capacity = capacity;
			_buffer = (Byte*)malloc(capacity);
			_position = 0;
			_owns_buffer = true;

		}
		MemoryStream::MemoryStream(Byte* buffer, size_t size) {

			_length = size;
			_capacity = size;
			_buffer = buffer;
			_position = 0;
			_owns_buffer = false;

		}
		MemoryStream::~MemoryStream() {

			Close();

		}

		size_t MemoryStream::Length() {

			return _length;

		}
		size_t MemoryStream::Capacity() const {

			return _capacity;

		}
		size_t MemoryStream::Position() const {

			return _position;

		}
		bool MemoryStream::IsEmpty() const {

			return (_length == 0);

		}
		void MemoryStream::Flush() {}
		void MemoryStream::SetLength(size_t length) {

			// Throw error if the stream does not support writing or seeking.
			if (!CanWrite() || !CanSeek())
				throw NotSupportedException();

			// If the new length is larger than the current capacity, resize the buffer.
			if (length > _capacity)
				Reserve(length);

			// Set the new length.
			_length = length;

			// If the length is less than the seek position, move seek position to end.
			if (_position > _length)
				_position = _length;

		}
		bool MemoryStream::ReadByte(Byte& byte) {

			// Throw an exception of the stream is not readable.
			if (!CanRead())
				throw NotSupportedException();

			// Return false if the buffer does not contain enough bytes to read.
			if (_position + sizeof(Byte) > _length)
				return false;

			// Copy data from the buffer to the output memory.
			memcpy(&byte, _buffer + _position * sizeof(Byte), sizeof(Byte));

			// Move the seek position forward by the number of bytes written.
			_position += sizeof(Byte);

			// A byte was successfully read, so return true.
			return true;

		}
		void MemoryStream::WriteByte(Byte byte) {

			// Throw an exception of the stream is not writeable.
			if (!CanWrite())
				throw NotSupportedException();

			// Make enough room in the buffer for new data.
			AllocateBytes(sizeof(Byte));

			// Copy the data to the buffer.
			memcpy(_buffer + _position * sizeof(Byte), &byte, sizeof(Byte));

			// Move the seek position forward by the number of bytes written.
			_position += sizeof(Byte);

			// If the stream is now longer, increase the length.
			if (_position > _length)
				_length = _position;

		}
		size_t MemoryStream::Read(void* buffer, size_t offset, size_t length) {

			// Throw an exception of the stream is not readable.
			if (!CanRead())
				throw NotSupportedException();

			// Calculate new length if necessary (if length is greater than buffer size).
			size_t len = length;
			if (_position + length > _length)
				if (_position > _length)
					// The stream has been seeked beyond the end of the stream. Nothing to read.
					return 0;
				else
					len = _length - _position;

			// If length is 0, return.
			if (len == 0)
				return 0;

			// Copy memory from the buffer to the output buffer.
			memcpy((Byte*)buffer + offset * sizeof(Byte), _buffer + _position * sizeof(Byte), length);

			// Increase the seek position by bytes read.
			_position += len;

			// Return the number of bytes read.
			return len;

		}
		void MemoryStream::Write(const void* buffer, size_t offset, size_t length) {

			// Throw an exception of the stream is not writeable.
			if (!CanWrite())
				throw NotSupportedException();

			// Make enough room in the buffer for new data.
			AllocateBytes(length);

			// Copy memory from the input buffer to the internal buffer.
			memcpy(_buffer + _position * sizeof(Byte), (Byte*)buffer + offset * sizeof(Byte), length);

			// Move the seek position forward by the number of bytes written.
			_position += length;

			// If the stream is now longer, increase the length.
			if (_position > _length)
				_length = _position;

		}
		void MemoryStream::Close() {

			if (_buffer && _owns_buffer)
				free(_buffer);
			_buffer = nullptr;

		}
		void MemoryStream::CopyTo(StreamBase& stream) {

			// Throw an exception of the stream is not readable.
			if (!CanRead())
				throw NotSupportedException();

			// If the seek position is greater than or equal to the length of the stream, do nothing.
			if (_position >= _length)
				return;

			// Write remaining contents of the current stream to the other stream.
			stream.Write(_buffer, _position, _length - _position);
			_position = _length;

		}
		void MemoryStream::CopyTo(StreamBase& stream, size_t buffer_size) {

			StreamBase::CopyTo(stream, buffer_size);

		}
		size_t MemoryStream::Seek(long long offset, SeekOrigin origin) {

			// Throw an exception of the stream is not seekable.
			if (!CanSeek())
				throw NotSupportedException();

			// Get the origin position from the seek origin.
			size_t origin_position = 0;
			switch (origin) {
			case SeekOrigin::Begin:
				origin_position = 0;
				break;
			case SeekOrigin::Current:
				origin_position = _position;
				break;
			case SeekOrigin::End:
				origin_position = _length;
				break;
			}

			// Throw an error if the new position is less than 0.
			if (offset + origin_position < 0)
				throw IO::IOException("An attempt was made to move the position before the beginning of the stream.");

			// Apply the new position.
			_position = (size_t)(offset + origin_position);

			// Return the new position.
			return _position;

		}
		size_t MemoryStream::Seek(long long position) {

			return Seek(position, SeekOrigin::Begin);

		}
		bool MemoryStream::CanRead() const {

			return _buffer != nullptr;

		}
		bool MemoryStream::CanSeek() const {

			return _buffer != nullptr;

		}
		bool MemoryStream::CanWrite() const {

			return _buffer != nullptr;

		}

		// Protected methods

		void MemoryStream::AllocateBytes(size_t bytes) {

			// Calculate the required capacity. Note that the position may be greater than the length.
			size_t required_capacity = _position + bytes;

			// Increase buffer capacity if needed.
			if (required_capacity > _capacity) {
				size_t new_capacity = _capacity > 0 ? _capacity : 1;
				while (required_capacity > new_capacity)
					new_capacity *= 2;
				Reserve(new_capacity);
			}

		}
		Byte* MemoryStream::Buffer() {

			return _buffer;

		}
		void MemoryStream::Reserve(size_t capacity) {

			// If we don't own the buffer, don't attempt to resize it.
			if (!_owns_buffer)
				throw NotSupportedException("Memory stream is not expandable.");

			// If the new capacity is less than the current capacity, do nothing.
			if (_capacity >= capacity)
				return;

			// Increase the capacity of the buffer and clear the new memory.
			_buffer = (Byte*)realloc(_buffer, capacity);
			memset(_buffer + _capacity, 0, capacity - _capacity);

			_capacity = capacity;

		}
		void MemoryStream::Resize(size_t size, Byte value) {

			// If we don't own the buffer, don't attempt to resize it.
			if (!_owns_buffer)
				throw NotSupportedException("Memory stream is not expandable.");

			// Adjust the size of the buffer.
			_buffer = (Byte*)realloc(_buffer, size);

			// If the buffer is larger, fill new memory with given value.
			if (size > _length)
				memset(_buffer + _length * sizeof(Byte), value, size - _length);

			// Update size and capcity counters.
			_capacity = size;
			_length = size;

			// If the seek position is now beyond the end of the buffer, move it to the end of the buffer.
			if (_position > _length)
				_position = _length;


		}

	}
}