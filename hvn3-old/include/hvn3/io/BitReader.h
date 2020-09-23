#pragma once
#include "hvn3/io/StreamBase.h"
#include "hvn3/io/Buffer.h"
#include <string>

namespace hvn3 {
	namespace IO {

		class BitReader {

		public:
			BitReader(StreamBase& stream);
			~BitReader();

			// Exposes access to the underlying stream of the BitReader.
			StreamBase& BaseStream();

			// Closes the current BitReader and the underlying stream.
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
			// Returns the next available byte, or -1 if no more bytes are available, and does not advance the read position.
			int Peek();

			// Attempts to read a single bit from the underlying stream. Returns true if successful.
			bool ReadBool(bool& value);
			// Attempts to read a byte from the underlying stream. Returns true if successful.
			bool ReadByte(Byte& value, Byte min = 0, Byte max = UCHAR_MAX);
			// Attempts to read "length" bytes from the underlying stream into the given array. Returns the actual number of bytes read.
			size_t ReadBytes(Byte* value, size_t length);
			// Attempts to read "length" bytes from the underlying stream into the given array beginning at the given offset. Returns the actual number of bytes read.
			size_t ReadBytes(Byte* value, size_t offset, size_t length);
			// Attempts to read a signed char from the underlying stream. Returns true if successful.
			bool ReadChar(signed char& value, signed char min = SCHAR_MIN, signed char max = SCHAR_MAX);
			// Attempts to read a char array from the underlying stream until the first null byte. Returns the number of characters read.
			size_t ReadString(char* value);
			// Attempts to read "length" characters from the stream. Returns the actual number of bytes read.
			size_t ReadString(char* value, size_t length);
			// Attempts to read "length" characters from the stream into the array at the given offset. Returns the actual number of bytes read.
			size_t ReadString(char* value, size_t offset, size_t length);
			// Attempts to read a string from the underlying stream until the first null byte. Returns the number of characters read.
			size_t ReadString(std::string& value);
			// Attempts to read "length" characters from the stream into the string. Returns the actual number of bytes read.
			size_t ReadString(std::string& value, size_t length);
			// Attempts to read a float from the underlying stream. Returns true if successful.
			bool ReadFloat(float& value);
			// Attempts to read an unsigned int from the underlying stream. Returns true if successful.
			bool ReadInteger(unsigned int& value, unsigned int min = 0, unsigned int max = UINT_MAX);
			// Attempts to read a signed int from the underlying stream. Returns true if successful.
			bool ReadInteger(signed int& value, signed int min = INT_MIN, signed int max = INT_MAX);
			// Attempts to read an unsigned short from the underlying stream. Returns true if successful.
			bool ReadShort(unsigned short& value, unsigned short min = 0, unsigned short max = USHRT_MAX);
			// Attempts to read a signed short from the underlying stream. Returns true if successful.
			bool ReadShort(signed short& value, signed short min = SHRT_MIN, signed short max = SHRT_MAX);

		protected:
			// Flushes reads performed on the buffer to the underlying stream.
			void FlushRead();
			// Fills the read buffer with bytes from the underlying stream.
			void FillBuffer();
			// Clears the read buffer and resets byte/bit offsets.
			void ClearBuffer();
			// Returns the number of free bits left in the read buffer.
			size_t UnreadBitsLeft() const;
			// Increments the bit offset, resetting it back to 0 and incrementing the byte offset when appropriate.
			void IncrementBitOffset();
			// Reads "bits" bits from the read buffer into "value".
			bool ReadBits(uint32_t& value, unsigned int bits);

		private:
			// The underlying stream.
			StreamBase* _stream;
			// The buffer used for reads.
			Buffer _buffer;
			// The current byte offset in the read buffer.
			size_t _byte_offset;
			// The number of bytes read into the read buffer from the underlying stream.
			size_t _bytes_read;
			// The current bit offset in the read buffer.
			Byte _bit_offset;

		};

	}
}