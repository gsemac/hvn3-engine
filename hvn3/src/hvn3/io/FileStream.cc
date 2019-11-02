#include "hvn3/io/IOException.h"
#include "hvn3/io/FileStream.h"
#include <algorithm>
#include <sys/stat.h>

namespace hvn3 {
	namespace IO {

		FileStream::FileStream(const char* path, FileMode mode) : FileStream(path, mode, FileAccess::ReadWrite) {}
		FileStream::FileStream(const char* path, FileMode mode, FileAccess access) {

			// Initialize member variables.
			_position = 0;
			_path = path;
			_last_read = false;

			// Initialize flags.
			InitFlags(mode, access);

			// Open the file.
			_stream.open(path, _flags);

			// If opened in "Append" mode, seek to the end of the file.
			if (_flags & std::fstream::app) {
				_stream.seekg(0, std::ios_base::end);
				_position = (size_t)_stream.tellg();
			}

		}
		FileStream::~FileStream() {

			Close();

		}

		size_t FileStream::Length() {

			// If the stream is in "Append" mode, just return the current position (guaranteed to be at the end of the stream).
			if (_flags & std::fstream::app)
				return _position;

			// If the stream does not support reading, throw an error.
			if (!CanRead())
				throw NotSupportedException();

			// Preserve the current seek position.
			std::streampos pos = _stream.tellg();

			// Seek to the end of the stream, and record the size.
			_stream.seekg(0, std::ios::end);
			std::streampos size = _stream.tellg();

			// Seek back to the previous position.
			_stream.seekg(pos, std::ios::beg);

			// Return the file size.
			return size < 0 ? 0 : (size_t)size;

		}
		size_t FileStream::Position() const {

			return _position;

		}
		void FileStream::Flush() {

			_stream.flush();

		}
		void FileStream::SetLength(size_t length) {

			// Throw error if the stream does not support writing or seeking.
			if (!CanWrite() || !CanSeek())
				throw NotSupportedException();

			// Get the current length.
			size_t clength = Length();

			if (length < clength) {

				// Create a buffer, and copy into it the first length bytes of the file.
				Byte* buf = new Byte[length];
				size_t prev_pos = _position;
				Seek(0, IO::SeekOrigin::Begin);
				Read(buf, 0, length);

				// Close the file stream.
				_stream.flush();
				_stream.close();

				// Create a new file stream with the truncation flag.
				_stream.open(_path, std::fstream::out | std::fstream::trunc);

				// Write the buffer back to the file, and free it.
				_position = 0;
				Write(buf, 0, length);
				delete[] buf;

				// Close the file.
				_stream.flush();
				_stream.close();

				// Re-open the file with previous flags, and seek the former or truncated position.
				_stream.open(_path, _flags);
				_last_read = false;
				_stream.seekg((std::min)(prev_pos, length));

			}
			else if (length > clength) {

				// Seek to the desired length, write a null byte, and then seek back.
				_stream.seekg(-1 + length);
				_stream.write("\0", 1);
				_stream.seekg(_position);

			}

		}
		bool FileStream::ReadByte(Byte& byte) {

			// Throw an exception of the stream is not readable.
			if (!CanRead())
				throw NotSupportedException();

			// If the last action was not a read, seek to refresh the stream.
			if (!_last_read) _stream.seekg(_position);
			_last_read = true;

			// Attempt to read a byte from the Stream.
			char b;
			_stream.read(&b, 1);

			// If we've reached the end of the Stream, a valid character was not read.
			if (_stream.eof() || b == -1)
				return false;

			// Set the value read, and increment the seek position.
			byte = b;
			++_position;

			// Return true, since a byte was successfully read.
			return true;

		}
		void FileStream::WriteByte(Byte byte) {

			// Throw an exception of the stream is not writeable.
			if (!CanWrite())
				throw NotSupportedException();

			// If the last action was not a write, seek to refresh the stream.
			if (_last_read) _stream.seekg(_position);
			_last_read = false;

			_stream.write((const char*)&byte, 1);
			++_position;

		}
		size_t FileStream::Read(void* buffer, size_t offset, size_t length) {

			// Throw an exception of the stream is not readable.
			if (!CanRead())
				throw NotSupportedException();

			// If the last action was not a read, seek to refresh the stream.
			if (!_last_read) _stream.seekg(_position);
			_last_read = true;

			// Write bytes from the stream to the buffer.
			_stream.read((char*)buffer + offset * sizeof(Byte), length);

			// Update the seek position.
			size_t bytes_read = (size_t)_stream.gcount();
			_position += bytes_read;

			return bytes_read;

		}
		void FileStream::Write(const void* buffer, size_t offset, size_t length) {

			// Throw an exception of the stream is not writeable.
			if (!CanWrite())
				throw NotSupportedException();

			// If the last action was not a write, seek to refresh the stream.
			if (_last_read) _stream.seekg(_position);
			_last_read = false;

			// Write bytes to the stream from the buffer.
			_stream.write((const char*)buffer + offset, length);

			// Update the sek position.
			_position += length;

		}
		void FileStream::Close() {

			// Flush and close the stream if it is open.
			if (_stream.is_open()) {
				_stream.flush();
				_stream.close();
			}

			// Reset the seek position and flags.
			_position = 0;
			_flags = 0;
			_access_flags = 0;

		}
		size_t FileStream::Seek(long long offset, SeekOrigin origin) {

			// Throw an exception of the stream is not seekable.
			if (!CanSeek()) throw
				IO::IOException();

			// For fstream, seekg and seekp refer to the same pointer, and do not need to be considered separately.

			// Store the current stream position (to compute difference later).
			std::streampos pos = _stream.tellg();

			// Determine the seek origin.
			std::ios_base::seekdir seek_origin = std::ios_base::beg;
			switch (origin) {
			case SeekOrigin::Current:
				seek_origin = std::ios_base::cur;
				break;
			case SeekOrigin::End:
				seek_origin = std::ios_base::end;
				break;
			}

			// Move the seek pointer.
			_stream.seekg(offset, seek_origin);

			// Adjust the seek position according to the difference.
			_position += (size_t)(_stream.tellg() - pos);

			// Return the new position.
			return _position;

		}
		size_t FileStream::Seek(long long position) {

			return Seek(position, SeekOrigin::Begin);

		}
		bool FileStream::CanRead() const {

			return (_access_flags & std::fstream::in) && !(_flags & std::fstream::app) && _stream.is_open();

		}
		bool FileStream::CanSeek() const {

			return !(_flags & std::fstream::app) && _stream.is_open();

		}
		bool FileStream::CanWrite() const {

			return (_access_flags & std::fstream::out) && _stream.is_open();

		}

		bool FileStream::FileExists(const char* path) {

			struct stat buf;
			return (stat(path, &buf) != -1);

		}
		void FileStream::InitFlags(FileMode mode, FileAccess access) {

			// Initialize variables.
			_flags = std::fstream::binary;
			_access_flags = 0;

			// Set access flags.
			switch (access) {
			case FileAccess::Read:
				_access_flags |= std::fstream::in;
				break;
			case FileAccess::ReadWrite:
				_access_flags |= std::fstream::in;
			case FileAccess::Write:
				_access_flags |= std::fstream::out;
				break;
			}

			// Set mode flags.
			switch (mode) {

			case FileMode::Append:
				_flags |= std::fstream::app | std::fstream::out;
				break;

			case FileMode::Open:
				if (!FileExists(_path.c_str()))
					throw IO::FileNotFoundException(_path);
				_flags |= _access_flags;
				break;

			case FileMode::OpenOrCreate:
				if (FileExists(_path.c_str()))
					_flags |= _access_flags;
				else
					_flags |= std::fstream::out;
				break;

			case FileMode::CreateNew:
				if (FileExists(_path.c_str()))
					throw IO::IOException();

			case FileMode::Create:

			case FileMode::Truncate:
				_flags |= std::fstream::trunc | _access_flags;
				break;

			}

		}

	}
}