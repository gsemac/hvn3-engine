#ifndef __BITPACKER_H
#define __BITPACKER_H

namespace Net {
	namespace IO {

		class BitPacker {

		public:
			enum MODE {
				READ,
				WRITE
			};

		private:
			int bit_index;
			unsigned char * ptr;
			unsigned char * buffer;
			int bytes;
			MODE mode;

		public:
			BitPacker(MODE mode, void * buffer, int bytes);

			void WriteBits(unsigned int value, int bits = 32);
			void ReadBits(unsigned int & value, int bits = 32);

			void * GetData();
			int GetBits() const;
			int GetBytes() const;
			int BitsRemaining() const;
			MODE GetMode() const;

			bool IsValid() const;

		};

	}
}

#endif