#pragma once

#include "graphics/bitmap_data.h"
#include "graphics/bitmap_options.h"
#include "graphics/color.h"
#include "graphics/image_format.h"
#include "io/file_access.h"
#include "math/rectangle.h"

#include <memory>

struct ALLEGRO_BITMAP;

namespace hvn3::graphics {

	class Bitmap {

		typedef ALLEGRO_BITMAP underlying_t;

	public:
		Bitmap();
		Bitmap(int width, int height);
		Bitmap(int width, int height, BitmapOptions options);
		Bitmap(underlying_t* bitmap, bool takeOwnership);
		Bitmap(const Bitmap& other);
		Bitmap(const Bitmap& other, const math::RectangleI& region);
		Bitmap(Bitmap&& other) noexcept;

		int Width() const;
		int Height() const;

		Bitmap Copy() const;
		Bitmap Copy(const math::RectangleI& region) const;

		BitmapData Lock(io::FileAccess access = io::FileAccess::ReadWrite);
		BitmapData LockRegion(const math::RectangleI& region, io::FileAccess access = io::FileAccess::ReadWrite);
		void Unlock();
		bool IsLocked() const;

		underlying_t* GetUnderlyingData() const;
		underlying_t* GetUnderlyingData(bool performPreWriteOperations);

		Bitmap& operator=(const Bitmap& other);
		Bitmap& operator=(Bitmap&& other) noexcept;

		explicit operator bool() const;

		static Bitmap FromBuffer(const uint8_t* buffer, size_t bufferSize, ImageFormat format);
		static Bitmap FromFile(const std::string& filePath);

	private:
		struct SubBitmapDeleter {

			void operator()(underlying_t* subBitmap) const;

		};

		std::shared_ptr<underlying_t> bitmap;
		std::unique_ptr<underlying_t, SubBitmapDeleter> subBitmap;
		bool ownsBitmap;

		void CopyAssignFrom(const Bitmap& other);
		void MoveAssignFrom(Bitmap&& other);
		bool IsCopyRequired() const;
		void PerformPreWriteOperations();

		static int ConvertBitmapOptionsToFlags(BitmapOptions options);
		static int ConvertFileAccessToFlags(io::FileAccess fileAccess);
		static PixelFormat ConvertPixelFormat(int pixelFormat);
		static underlying_t* CreateBitmap(int width, int height, BitmapOptions options);
		static void FreeBitmap(underlying_t* bitmap);
		static underlying_t* CloneSubBitmap(underlying_t* parentBitmap, underlying_t* subBitmap);

	};

	bool operator==(const Bitmap& lhs, const Bitmap& rhs);
	bool operator!=(const Bitmap& lhs, const Bitmap& rhs);

}