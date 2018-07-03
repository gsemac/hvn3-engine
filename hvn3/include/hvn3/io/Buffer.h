#pragma once
#include "hvn3/utility/BitUtils.h"

namespace hvn3 {
	namespace IO {

		class Buffer {

		public:
			Buffer(size_t bytes, bool zero = false);
			Buffer(const Buffer& other);
			Buffer(Buffer&& other);
			~Buffer();

			// Shifts the contents of the buffer by the given amount.
			void Shift(int amount);
			// Rotates the contents of the buffer by the given amount.
			void Rotate(int amount);
			// Sets every byte in the buffer to the given value.
			void Fill(Byte value);
			// Sets all memory in the range to the given value.
			void Fill(size_t index, size_t length, Byte value);
			// Copies the memory in the range to the given buffer at the given offset.
			void Copy(Buffer& buffer, size_t offset, size_t index, size_t length);
			// Clears the contents of the buffer by setting every byte to 0.
			void Clear();
			// Clears all memory in the range by setting every byte to 0.
			void Clear(size_t index, size_t length);
			// Reverses the contents of the buffer.
			void Reverse();
			// Reverses the contents of the buffer in the given range.
			void Reverse(size_t index, size_t length);
			// Resizes the buffer to the given size, and copies the original memory to the new buffer.
			void Resize(size_t bytes, bool zero = false);
			// Resizes the buffer if it is smaller than the given size. If the given size is equal to or less than the current size, does nothimg.
			void Reserve(size_t bytes, bool zero = false);
			// Returns the size of the buffer in bytes.
			size_t Size() const;
			// Returns the address of the underlying memory segment.
			Byte* Pointer() const;
			// Returns the value of the byte at the given index.
			Byte& operator[](size_t index);
			Buffer& operator=(const Buffer& other);
			Buffer& operator=(Buffer&& other);
			operator Byte*() const;

		private:
			Byte* _buffer;
			size_t _size;

			void _freeBuffer();

		};

	}
}