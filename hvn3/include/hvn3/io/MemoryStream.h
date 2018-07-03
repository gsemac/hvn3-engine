#pragma once
#include "hvn3/io/StreamBase.h"

namespace hvn3 {
	namespace IO {

		class MemoryStream : public StreamBase {

		public:
			// Initializes a new instance of the MemoryStream class with an expandable capacity initialized to zero.
			MemoryStream();
			// Initializes a new instance of the MemoryStream class with an expandable capacity initialized as specified.
			MemoryStream(size_t capacity);
			// Initializes a new non-resizable instance of the MemoryStream class based on the specified byte array.
			MemoryStream(Byte* buffer, size_t size);
			// Releases all resources used by the Stream.
			virtual ~MemoryStream();

			// Gets the length of the stream in bytes.
			virtual size_t Length() override;
			// Gets the number of bytes allocated for this stream.
			virtual size_t Capacity() const;
			// Gets the current position within the stream.
			virtual size_t Position() const override;
			// Returns true if the stream is empty.
			virtual bool IsEmpty() const;
			// Clears contents of the Stream.
			virtual void Flush() override;
			// Sets the length of the current stream to the specified value.
			virtual void SetLength(size_t length) override;
			// Reads a byte from the stream and advances the position within the stream by one byte, or returns false if at the end of the stream.
			virtual bool ReadByte(Byte& byte) override;
			// Writes a byte to the current stream at the current position.
			virtual void WriteByte(Byte byte) override;
			// Reads a block of bytes from the current stream and writes the data to a buffer.
			virtual size_t Read(void* buffer, size_t offset, size_t length) override;
			// Writes a block of bytes to the current stream using data read from a buffer.
			virtual void Write(const void* buffer, size_t offset, size_t length) override;
			// Closes the current stream and releases any resources (such as sockets and file handles) associated with the current stream.
			virtual void Close() override;
			// Reads the bytes from the current stream and writes them to another stream.
			virtual void CopyTo(StreamBase& stream) override;
			// Reads the bytes from the current stream and writes them to another stream, using a specified buffer size.
			virtual void CopyTo(StreamBase& stream, size_t buffer_size) override;
			// Sets the position within the current stream to the specified value.
			virtual size_t Seek(long long offset, SeekOrigin origin) override;
			// Sets the position within the current stream to the specified value.
			virtual size_t Seek(long long position) override;
			// Gets a value indicating whether the current stream supports reading.
			virtual bool CanRead() const override;
			// Gets a value indicating whether the current stream supports seeking.
			virtual bool CanSeek() const override;
			// Gets a value indicating whether the current stream supports writing.
			virtual bool CanWrite() const override;

		protected:
			// Expands the buffer (if necessary) to be able to contain "bytes" additional bytes.
			virtual void AllocateBytes(size_t bytes);
			// Returns the underlying Byte buffer.
			virtual Byte* Buffer();
			// Resizes the underlying Byte buffer. If the given capacity is less than the current capacity, the buffer size is not modified.
			virtual void Reserve(size_t capacity);
			// Resizes the underlying Byte buffer, and fills new memory with the given value. If the given capacity is less than the current capacity, the buffer is truncated.
			virtual void Resize(size_t size, Byte value = 0);

		private:
			bool _owns_buffer;
			Byte* _buffer;
			size_t _length;
			size_t _capacity;
			size_t _position;

		};

	}
}