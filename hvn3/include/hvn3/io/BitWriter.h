#pragma once
#include "hvn3/io/StreamBase.h"
#include <string>

namespace hvn3 {
	namespace IO {

		class BitWriter {

		public:
			BitWriter(StreamBase& stream);
			~BitWriter();

			// Gets the underlying stream of the BitWriter.
			StreamBase& BaseStream();

			// Closes the current BitWriter and the underlying stream.
			void Close();
			// Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.
			void Flush();

			// Sets the position within the current stream.
			void Seek(long long position, SeekOrigin offset);
			// Sets the position within the current stream.
			void Seek(long long position);
			// Sets the bit position within the current stream.
			void SeekBits(long long bits, SeekOrigin offset);
			// Sets the bit position within the current stream.
			void SeekBits(long long bits);

			// Writes a single bit to the underlying stream.
			void WriteBool(bool value);
			// Writes a byte to the underlying stream.
			void WriteByte(Byte value, Byte min = 0, Byte max = UCHAR_MAX);
			// Writes "length" bytes from the given byte array to the underlying stream.
			void WriteBytes(Byte* value, size_t length);
			// Writes "length" bytes from the given byte array beginning at "offset" to the underlying stream.
			void WriteBytes(Byte* value, size_t offset, size_t length);
			// Writes a signed character to the underlying stream.
			void WriteChar(signed char value, signed char min = SCHAR_MIN, signed char max = SCHAR_MAX);
			// Writes a null-terminated character array to the underlying stream.
			void WriteString(const char* value);
			// Writes "length" characters from the given array to the underlying stream.
			void WriteString(const char* value, size_t length);
			// Writes "length" characters from the given array beginning at "offset" to the underlying stream.
			void WriteString(const char* value, size_t offset, size_t length);
			// Writes a null-terminated string to the underlying stream.
			void WriteString(const std::string& value);
			// Writes a float to the underlying stream.
			void WriteFloat(float value);
			// Writes an unsigned integer to the underlying stream.
			void WriteInteger(unsigned int value, unsigned int min = 0, unsigned int max = UINT_MAX);
			// Writes a signed integer to the underlying stream.
			void WriteInteger(signed int value, signed int min = INT_MIN, signed int max = INT_MAX);
			// Writes an unsigned short to the underlying stream.
			void WriteShort(unsigned short value, unsigned short min = 0, unsigned short max = USHRT_MAX);
			// Writes a signed short to the underlying stream.
			void WriteShort(signed short value, signed short min = SHRT_MIN, signed short max = SHRT_MAX);

		protected:
			BitWriter();
			// Flushes all data in the write buffer to the underlying stream.
			void FlushWrite();
			// Creates a new write buffer of "bytes" bytes, and copies any existing data into the new buffer.
			void AllocateBuffer(size_t bytes);
			// Clears the write buffer by setting every byte to 0.
			void ClearBuffer();
			// Returns the number of unwritten bits remaining in the write buffer.
			size_t BitsRemaining() const;
			// Increments the bit offset, resetting it back to 0 and incrementing the byte offset when appropriate.
			void IncrementBitOffset();
			// Writes "bits" bits from "value" into the write buffer.
			void WriteBits(uint32_t value, int bits);

		private:
			// The underlying stream.
			StreamBase* _stream;
			bool _owns_stream;
			// The buffer used for writing.
			Byte* _buffer;
			// The site of the write buffer.
			size_t _buffer_size;
			// The current byte offset into the write buffer.
			size_t _byte_offset;
			// The curret bit offset into the write buffer.
			Byte _bit_offset;

		};

	}
}