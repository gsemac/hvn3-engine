#include "graphics/bitmap.h"

#include "core/engine.h"

#include <cassert>
#include <utility>

#include <allegro5/allegro.h>
#include <allegro5/allegro_memfile.h>

namespace hvn3::graphics {

	// Public members

	Bitmap::Bitmap() {

		ownsBitmap = true;

	}
	Bitmap::Bitmap(int width, int height) :
		Bitmap(width, height, BitmapOptions::Default) {
	}
	Bitmap::Bitmap(int width, int height, BitmapOptions options) :
		Bitmap() {

		assert(width >= 0);
		assert(height >= 0);

		if (width > 0 && height > 0)
			this->bitmap = std::shared_ptr<underlying_bitmap_t>(CreateBitmap(width, height, options), FreeBitmap);

	}
	Bitmap::Bitmap(underlying_bitmap_t* bitmap, bool takeOwnership) :
		Bitmap() {

		if (bitmap != nullptr) {

			if (takeOwnership) {

				core::Engine::Initialize(core::EngineModules::Core);

				this->bitmap = std::shared_ptr<underlying_bitmap_t>(bitmap, FreeBitmap);
				this->ownsBitmap = true;

			}
			else {

				// Uses the aliasing constructor to avoid both freeing the data and allocating a control block.
				// https://stackoverflow.com/questions/27109379/what-is-shared-ptrs-aliasing-constructor-for

				this->bitmap = std::shared_ptr<underlying_bitmap_t>(std::shared_ptr<underlying_bitmap_t>{}, bitmap);
				this->ownsBitmap = false;

			}

		}

	}
	Bitmap::Bitmap(const Bitmap& other) :
		Bitmap() {

		CopyAssignFrom(other);

	}
	Bitmap::Bitmap(const Bitmap& other, const math::RectangleI& region) {

		this->bitmap = other.bitmap;
		this->subBitmap = std::unique_ptr<underlying_bitmap_t, SubBitmapDeleter>(al_create_sub_bitmap(other.bitmap.get(), region.X(), region.Y(), region.Width(), region.Height()));

	}
	Bitmap::Bitmap(Bitmap&& other) noexcept {

		MoveAssignFrom(std::move(other));

	}

	int Bitmap::Width() const {

		underlying_bitmap_t* bitmap = GetUnderlyingData();

		return (bitmap != nullptr) ?
			al_get_bitmap_width(bitmap) :
			0;

	}
	int Bitmap::Height() const {

		underlying_bitmap_t* bitmap = GetUnderlyingData();

		return (bitmap != nullptr) ?
			al_get_bitmap_height(bitmap) :
			0;

	}

	Bitmap Bitmap::Copy() const {

		// Note that al_clone_bitmap calls al_create_bitmap, which uses the current new bitmap flags.
		// In order to preserve the flags the other bitmap was created with, we need to set them first.

		underlying_bitmap_t* underlyingBitmap = GetUnderlyingData();

		assert(underlyingBitmap != nullptr);

		int flags = al_get_new_bitmap_flags();

		al_set_new_bitmap_flags(al_get_bitmap_flags(underlyingBitmap));

		underlying_bitmap_t* bitmap = al_clone_bitmap(underlyingBitmap);

		al_set_new_bitmap_flags(flags);

		return Bitmap(bitmap, true);

	}
	Bitmap Bitmap::Copy(const math::RectangleI& region) const {

		// Currently, we copy the entire bitmap and return a sub-bitmap with the requested region.
		// It would be better if we only copied the requested region.

		Bitmap copiedBitmap = Copy();

		return Bitmap(copiedBitmap, region);

	}

	BitmapData Bitmap::Lock(io::FileAccess access) {

		underlying_bitmap_t* bitmap = GetUnderlyingData();

		assert(bitmap != nullptr);

		if (access == io::FileAccess::ReadWrite || access == io::FileAccess::Write)
			PerformPreWriteOperations();

		ALLEGRO_LOCKED_REGION* lockedRegion = al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), ConvertFileAccessToFlags(access));

		BitmapData bitmapData;

		bitmapData.Scan0 = (unsigned char*)lockedRegion->data;
		bitmapData.PixelFormat = ConvertPixelFormat(lockedRegion->format);
		bitmapData.Stride = lockedRegion->pitch;
		bitmapData.BytesPerPixel = lockedRegion->pixel_size;

		return bitmapData;

	}
	BitmapData Bitmap::LockRegion(const math::RectangleI& region, io::FileAccess access) {

		underlying_bitmap_t* bitmap = GetUnderlyingData();

		assert(bitmap != nullptr);

		if (access == io::FileAccess::ReadWrite || access == io::FileAccess::Write)
			PerformPreWriteOperations();

		ALLEGRO_LOCKED_REGION* lockedRegion = al_lock_bitmap_region(bitmap, region.X(), region.Y(), region.Width(), region.Height(), al_get_bitmap_format(bitmap), ConvertFileAccessToFlags(access));

		BitmapData bitmapData;

		bitmapData.Scan0 = (unsigned char*)lockedRegion->data;
		bitmapData.PixelFormat = ConvertPixelFormat(lockedRegion->format);
		bitmapData.Stride = lockedRegion->pitch;
		bitmapData.BytesPerPixel = lockedRegion->pixel_size;

		return bitmapData;

	}
	void Bitmap::Unlock() {

		if (IsLocked()) {

			underlying_bitmap_t* bitmap = GetUnderlyingData();

			assert(bitmap != nullptr);

			al_unlock_bitmap(bitmap);

		}

	}
	bool Bitmap::IsLocked() const {

		underlying_bitmap_t* bitmap = GetUnderlyingData();

		return (bitmap != nullptr) ?
			al_is_bitmap_locked(bitmap) :
			false;

	}

	void Bitmap::SetPixel(int x, int y, const Color& color) {

		underlying_bitmap_t* bitmap = GetUnderlyingData(true);

		assert(bitmap != nullptr);

		underlying_bitmap_t* currentTargetBitmap = al_get_target_bitmap();
		ALLEGRO_COLOR allegroColor = al_map_rgba_f(color.Rf() * color.Af(), color.Gf() * color.Af(), color.Bf() * color.Af(), color.Af());

		al_set_target_bitmap(bitmap);

		al_put_pixel(x, y, allegroColor);

		al_set_target_bitmap(currentTargetBitmap);

	}
	Color Bitmap::GetPixel(int x, int y) const {

		underlying_bitmap_t* bitmap = GetUnderlyingData();

		assert(bitmap != nullptr);

		ALLEGRO_COLOR color = al_get_pixel(bitmap, x, y);

		return Color::FromArgbf(color.r, color.g, color.b, color.a);

	}

	Bitmap::underlying_bitmap_t* Bitmap::GetUnderlyingData() const {

		return subBitmap ? subBitmap.get() : bitmap.get();

	}
	Bitmap::underlying_bitmap_t* Bitmap::GetUnderlyingData(bool performPreWriteOperations) {

		if (performPreWriteOperations)
			PerformPreWriteOperations();

		return GetUnderlyingData();

	}

	Bitmap& Bitmap::operator=(const Bitmap& other) {

		CopyAssignFrom(other);

		return *this;

	}
	Bitmap& Bitmap::operator=(Bitmap&& other) noexcept {

		MoveAssignFrom(std::move(other));

		return *this;

	}

	Bitmap::operator bool() const {

		return GetUnderlyingData() != nullptr;

	}

	Bitmap Bitmap::FromBuffer(const uint8_t* buffer, size_t bufferSize, ImageFormat format) {

		if (buffer == nullptr || bufferSize <= 0)
			return Bitmap();

		ALLEGRO_FILE* file = al_open_memfile((void*)buffer, bufferSize, "r");
		underlying_bitmap_t* bitmap = al_load_bitmap_f(file, GetImageFormatFileExtension(format).c_str());

		al_fclose(file);

		return Bitmap(bitmap, true);

	}
	Bitmap Bitmap::FromFile(const std::string& filePath) {

		underlying_bitmap_t* bitmap = al_load_bitmap(filePath.c_str());

		return Bitmap(bitmap, true);

	}

	bool operator==(const Bitmap& lhs, const Bitmap& rhs) {

		return lhs.GetUnderlyingData() == rhs.GetUnderlyingData();

	}
	bool operator!=(const Bitmap& lhs, const Bitmap& rhs) {

		return !(lhs == rhs);

	}

	// Private members

	void Bitmap::SubBitmapDeleter::operator()(underlying_bitmap_t* subBitmap) const {

		if (subBitmap != nullptr)
			al_destroy_bitmap(subBitmap);

	}

	void Bitmap::CopyAssignFrom(const Bitmap& other) {

		// Free the existing sub-bitmap (if we have one).

		subBitmap.release();

		// We'll make a shallow copy of the other bitmap. These means the two bitmaps will share the same memory.
		// It's possible that the other bitmap is non-owning, meaning copy-on-write functionality will not be available.
		// However, we don't want copy-on-write for such bitmaps, as this would prevent display's backbuffers from working correctly.

		this->bitmap = other.bitmap;

		if (other.subBitmap)
			this->subBitmap = std::unique_ptr<underlying_bitmap_t, SubBitmapDeleter>(CloneSubBitmap(other.bitmap.get(), other.subBitmap.get()));

	}
	void Bitmap::MoveAssignFrom(Bitmap&& other) {

		this->bitmap = std::move(other.bitmap);
		this->subBitmap = std::move(other.subBitmap);
		this->ownsBitmap = other.ownsBitmap;

	}
	bool Bitmap::IsCopyRequired() const {

		// Owned bitmaps are always copied when assigned to new objects, so non-owned bitmaps can always write without making a copy.

		return ownsBitmap && bitmap && bitmap.use_count() > 1;

	}
	void Bitmap::PerformPreWriteOperations() {

		if (IsCopyRequired())
			*this = std::move(Copy());

	}

	int Bitmap::ConvertBitmapOptionsToFlags(BitmapOptions options) {

		const int defaultNewBitmapFlags = 4096;

		int flags = 0;

		if (HasFlag(options, BitmapOptions::Default))
			flags |= defaultNewBitmapFlags;

		if (HasFlag(options, BitmapOptions::MinLinear))
			flags |= ALLEGRO_MIN_LINEAR;

		if (HasFlag(options, BitmapOptions::MagLinear))
			flags |= ALLEGRO_MAG_LINEAR;

		return flags;

	}
	int Bitmap::ConvertFileAccessToFlags(io::FileAccess fileAccess) {

		switch (fileAccess) {

		case io::FileAccess::Read:
			return ALLEGRO_LOCK_READONLY;

		case io::FileAccess::Write:
			return ALLEGRO_LOCK_WRITEONLY;

		default:
			return ALLEGRO_LOCK_READWRITE;

		}

	}
	PixelFormat Bitmap::ConvertPixelFormat(int pixelFormat) {

		switch (pixelFormat) {

		case ALLEGRO_PIXEL_FORMAT_ANY:
			return PixelFormat::Any;

		case ALLEGRO_PIXEL_FORMAT_ANY_NO_ALPHA:
			return PixelFormat::AnyNoAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA:
			return PixelFormat::AnyWithAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_15_NO_ALPHA:
			return PixelFormat::Any15NoAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_16_NO_ALPHA:
			return PixelFormat::Any16NoAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_16_WITH_ALPHA:
			return PixelFormat::Any16WithAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA:
			return PixelFormat::Any24NoAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_32_NO_ALPHA:
			return PixelFormat::Any32NoAlpha;

		case ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA:
			return PixelFormat::Any32WithAlpha;

		case ALLEGRO_PIXEL_FORMAT_ARGB_8888:
			return PixelFormat::Argb8888;

		case ALLEGRO_PIXEL_FORMAT_RGBA_8888:
			return PixelFormat::Rgba8888;

		case ALLEGRO_PIXEL_FORMAT_ARGB_4444:
			return PixelFormat::Argb4444;

		case ALLEGRO_PIXEL_FORMAT_RGB_888:
			return PixelFormat::Rgb888;

		case ALLEGRO_PIXEL_FORMAT_RGB_565:
			return PixelFormat::Rgb565;

		case ALLEGRO_PIXEL_FORMAT_RGB_555:
			return PixelFormat::Rgb555;

		case ALLEGRO_PIXEL_FORMAT_RGBA_5551:
			return PixelFormat::Rgba5551;

		case ALLEGRO_PIXEL_FORMAT_ARGB_1555:
			return PixelFormat::Argb1555;

		case ALLEGRO_PIXEL_FORMAT_ABGR_8888:
			return PixelFormat::Argb8888;

		case ALLEGRO_PIXEL_FORMAT_XBGR_8888:
			return PixelFormat::Xbgr8888;

		case ALLEGRO_PIXEL_FORMAT_BGR_888:
			return PixelFormat::Bgr888;

		case ALLEGRO_PIXEL_FORMAT_BGR_565:
			return PixelFormat::Bgr565;

		case ALLEGRO_PIXEL_FORMAT_BGR_555:
			return PixelFormat::Bgr555;

		case ALLEGRO_PIXEL_FORMAT_RGBX_8888:
			return PixelFormat::Rgbx8888;

		case ALLEGRO_PIXEL_FORMAT_XRGB_8888:
			return PixelFormat::Xrgb8888;

		case ALLEGRO_PIXEL_FORMAT_ABGR_F32:
			return PixelFormat::AbgrF32;

		case ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE:
			return PixelFormat::Abgr8888LE;

		default:
			return PixelFormat::Any;

		}

	}
	Bitmap::underlying_bitmap_t* Bitmap::CreateBitmap(int width, int height, BitmapOptions options) {

		assert(width > 0);
		assert(height > 0);

		core::Engine::Initialize(core::EngineModules::Core);

		int flags = al_get_new_bitmap_flags();

		al_set_new_bitmap_flags(ConvertBitmapOptionsToFlags(options));

		underlying_bitmap_t* bitmap = al_create_bitmap(width, height);

		al_set_new_bitmap_flags(flags);

		return bitmap;

	}
	void Bitmap::FreeBitmap(underlying_bitmap_t* bitmap) {

		if (bitmap != nullptr) {

			al_destroy_bitmap(bitmap);

			core::Engine::Deinitialize(core::EngineModules::Core);

		}

	}
	Bitmap::underlying_bitmap_t* Bitmap::CloneSubBitmap(underlying_bitmap_t* parentBitmap, underlying_bitmap_t* subBitmap) {

		// There is more than one way to clone a sub-bitmap.
		// We can create a sub-bitmap from a sub-bitmap (which will have the same parent), or we can create a new sub-bitmap from the parent.
		// I have chosen to the do the latter.

		int x = al_get_bitmap_x(subBitmap);
		int y = al_get_bitmap_y(subBitmap);
		int width = al_get_bitmap_width(subBitmap);
		int height = al_get_bitmap_height(subBitmap);

		return al_create_sub_bitmap(parentBitmap, x, y, width, height);

	}

}