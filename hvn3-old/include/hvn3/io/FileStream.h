#pragma once
#include "StreamBase.h"
#include <fstream>

namespace hvn3 {
	namespace IO {

		// Specifies how the operating system should open a file.
		enum class FileMode {
			// Opens the file if it exists and seeks to the end of the file, or creates a new file.
			Append,
			// Specifies that the operating system should create a new file. If the file already exists, it will be overwritten.
			Create,
			// Specifies that the operating system should create a new file. 
			CreateNew,
			// Specifies that the operating system should open an existing file.
			Open,
			// Specifies that the operating system should open a file if it exists; otherwise, a new file should be created.
			OpenOrCreate,
			// Specifies that the operating system should open an existing file. When the file is opened, it should be truncated so that its size is zero bytes.
			Truncate
		};

		// Defines constants for read, write, or read/write access to a file.
		enum class FileAccess {
			// Read access to the file. Data can be read from the file.
			Read,
			// Read and write access to the file. Data can be written to and read from the file.
			ReadWrite,
			// Write access to the file. Data can be written to the file. 
			Write
		};

		// Provides a Stream for a file.
		class FileStream : public StreamBase {

		public:
			// Initializes a new instance of the FileStream class with the specified path and creation mode.
			FileStream(const char* path, FileMode mode);
			// Initializes a new instance of the FileStream class with the specified path, creation mode, and read/write permission.
			FileStream(const char* path, FileMode mode, FileAccess access);
			// Releases all resources used by the Stream.
			virtual ~FileStream();

			// Gets the length in bytes of the stream.
			size_t Length() override;
			// Gets the current position of this stream.
			size_t Position() const override;
			// Clears buffers for this stream and causes any buffered data to be written to the file.
			virtual void Flush() override;
			// Sets the length of this stream to the given value.
			virtual void SetLength(size_t length) override;
			// Reads a byte from the file and advances the read position one byte.
			virtual bool ReadByte(Byte& byte) override;
			// Writes a byte to the current position in the file stream.
			virtual void WriteByte(Byte byte) override;
			// Reads a block of bytes from the stream and writes the data in a given buffer.
			virtual size_t Read(void* buffer, size_t offset, size_t length) override;
			// Writes a block of bytes to the file stream.
			virtual void Write(const void* buffer, size_t offset, size_t length) override;
			// Closes the current stream and releases any resources associated with the current stream.
			virtual void Close() override;
			// Sets the current position of this stream to the given value.
			virtual size_t Seek(long long offset, SeekOrigin origin) override;
			// Sets the current position of this stream to the given value.
			virtual size_t Seek(long long position) override;
			// Gets a value indicating whether the current stream supports reading.
			virtual bool CanRead() const override;
			// Gets a value indicating whether the current stream supports seeking.
			virtual bool CanSeek() const override;
			// Gets a value indicating whether the current stream supports writing.
			virtual bool CanWrite() const override;

		protected:
			// Returns true if a file with the given path exists.
			bool FileExists(const char* path);
			// Initializes mode/access flag values.
			void InitFlags(FileMode mode, FileAccess access);

		private:
			// The underlying stream.
			std::fstream _stream;
			// The path to the file the stream is accessing.
			std::string _path;
			// The current position in the stream.
			std::size_t _position;
			// Mode flags.
			unsigned int _flags;
			// File access flags.
			unsigned int _access_flags;
			// Set to true if the last action was a read (as opposed to a write).
			bool _last_read;

		};

	}
}