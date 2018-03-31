#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/graphics/BitmapUtils.h"
#include "hvn3/graphics/Graphics.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_memfile.h>

namespace hvn3 {
	namespace Graphics {

		static int to_lock_flag(IO::FileAccess flag) {

			switch (flag) {
			case IO::FileAccess::Read:
				return ALLEGRO_LOCK_READONLY;
			case IO::FileAccess::Write:
				return ALLEGRO_LOCK_WRITEONLY;
			case IO::FileAccess::ReadWrite:
			default:
				return ALLEGRO_LOCK_READWRITE;
			}

		}



		Bitmap::Bitmap() {

			_sub_bitmap = nullptr;
			_managed = true;

		}
		Bitmap::Bitmap(const SizeI& size) :
			Bitmap(size.Width(), size.Height()) {
		}
		Bitmap::Bitmap(const SizeI& size, BitmapFlags flags) :
			Bitmap(size.Width(), size.Height(), flags) {
		}
		Bitmap::Bitmap(int width, int height) :
			Bitmap() {

			if (width <= 0 || height <= 0)
				return;

			_src_bitmap = bitmap_ptr_type(al_create_bitmap(width, height), al_destroy_bitmap);

		}
		Bitmap::Bitmap(int width, int height, BitmapFlags flags) {
			
			BitmapFlags old_flags = NewBitmapFlags();
			SetNewBitmapFlags(flags);
			
			*this = Bitmap(width, height);
			
			SetNewBitmapFlags(old_flags);

		}
		Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap, bool managed) :
			Bitmap() {

			if (bitmap == nullptr)
				return;

			if (managed)
				_src_bitmap = bitmap_ptr_type(bitmap, al_destroy_bitmap);
			else {
				// Uses the aliasing constructor to avoid both freeing the data and allocating a control block.
				_src_bitmap = bitmap_ptr_type(bitmap_ptr_type{}, bitmap);
				_managed = false;
			}

		}

		Bitmap::Bitmap(const Bitmap& other, const RectangleI& region) :
			Bitmap() {

			// Sub-bitmaps will not copy-on-write, and will simply refer to a region of the source bitmap.
			// Use the Copy method to explicitly copy a region of a bitmap.

			_src_bitmap = other._src_bitmap;

			ALLEGRO_BITMAP* parent = _src_bitmap.get();

			if (other._sub_bitmap != nullptr)
				parent = other._sub_bitmap;

			// Even if we create a sub-bitmap of a sub-bitmap, the parent will still be bitmap it was sourced from.
			// Therefore, it's okay if the source sub-bitmap gets destroyed before the child does.
			_sub_bitmap = al_create_sub_bitmap(parent, region.X(), region.Y(), region.Width(), region.Height());

		}
		Bitmap::Bitmap(const Bitmap& other) :
			Bitmap() {

			_copy_assign(other);

		}
		Bitmap::Bitmap(Bitmap&& other) :
			Bitmap() {

			_move_assign(other);

		}

		Bitmap::~Bitmap() {

			if (_sub_bitmap != nullptr)
				al_destroy_bitmap(_sub_bitmap);

			_managed = false;
			_sub_bitmap = nullptr;

		}

		Bitmap Bitmap::Copy() const {

			if (_sub_bitmap != nullptr)
				return Copy(RectangleI(0, 0, Width(), Height()));

			Bitmap copy;
			copy._src_bitmap = bitmap_ptr_type(al_clone_bitmap(_src_bitmap.get()), al_destroy_bitmap);

			return copy;

		}
		Bitmap Bitmap::Copy(const RectangleI& region) const {

			Bitmap copy;
			copy._src_bitmap = bitmap_ptr_type(al_create_bitmap(region.Width(), region.Height()), al_destroy_bitmap);

			// #todo
			//Graphics(copy).DrawBitmap(0, 0, *this, region);

			return copy;

		}

		int Bitmap::Width() const {

			if (_sub_bitmap != nullptr)
				return al_get_bitmap_width(_sub_bitmap);

			if (_src_bitmap)
				return al_get_bitmap_width(_src_bitmap.get());

			return 0;

		}
		int Bitmap::Height() const {

			if (_sub_bitmap != nullptr)
				return al_get_bitmap_height(_sub_bitmap);

			if (_src_bitmap)
				return al_get_bitmap_height(_src_bitmap.get());

			return 0;

		}

		bool Bitmap::IsVideoBitmap() const {

			return al_get_bitmap_flags(_get_bitmap_ptr()) & ALLEGRO_VIDEO_BITMAP;

		}
		void Bitmap::ConvertToVideoBitmap() {

			al_convert_bitmap(_get_bitmap_ptr());

		}

		BitmapData Bitmap::Lock(IO::FileAccess access) {

			_perform_pre_write_operations();

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(_get_bitmap_ptr(), al_get_bitmap_format(_get_bitmap_ptr()), to_lock_flag(access));

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		BitmapData Bitmap::LockRegion(const RectangleI& region, IO::FileAccess access) {

			_perform_pre_write_operations();

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap_region(_get_bitmap_ptr(), region.X(), region.Y(), region.Width(), region.Height(), al_get_bitmap_format(_get_bitmap_ptr()), to_lock_flag(access));

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		void Bitmap::Unlock() {

			if (!IsLocked())
				return;

			_perform_pre_write_operations();

			al_unlock_bitmap(_get_bitmap_ptr());

		}
		bool Bitmap::IsLocked() const {

			return al_is_bitmap_locked(_get_bitmap_ptr());

		}

		void Bitmap::SetPixel(int x, int y, const Color& color) {

			_perform_pre_write_operations();

			Graphics(*this).DrawPoint(x, y, color);

		}
		Color Bitmap::GetPixel(int x, int y) const {

			ALLEGRO_COLOR px = al_get_pixel(_get_bitmap_ptr(), x, y);

			return Color::FromArgbf(px.r, px.g, px.b, px.a);

		}

		Bitmap& Bitmap::operator=(const Bitmap& other) {

			_copy_assign(other);

			return *this;

		}
		Bitmap& Bitmap::operator=(Bitmap&& other) {

			_move_assign(other);

			return *this;

		}
		bool Bitmap::operator==(const Bitmap& other) {
			return _src_bitmap.get() == other._src_bitmap.get();
		}
		bool Bitmap::operator!=(const Bitmap& other) {
			return !(*this == other);
		}
		Bitmap::operator bool() const {

			return static_cast<bool>(_src_bitmap);

		}

		Bitmap Bitmap::FromBuffer(uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format) {

			Bitmap bmp;

			if (buffer == nullptr || buffer_size <= 0)
				return bmp;

			ALLEGRO_FILE* file = al_open_memfile(buffer, buffer_size, "r");
			bmp._src_bitmap = bitmap_ptr_type(al_load_bitmap_f(file, Imaging::ImageFormatToFileExtension(format).c_str()), al_destroy_bitmap);

			al_fclose(file);

			return bmp;

		}
		Bitmap Bitmap::FromFile(const std::string& file) {

			Bitmap bmp;

			bmp._src_bitmap = bitmap_ptr_type(al_load_bitmap(file.c_str()), al_destroy_bitmap);

			return bmp;

		}
		Bitmap Bitmap::FromFile(const std::string& file, const Color& alpha_color) {

			Bitmap bmp = FromFile(file);

			ConvertMaskToAlpha(bmp, alpha_color);

			return bmp;

		}

		BitmapFlags Bitmap::NewBitmapFlags() {

			return System::AllegroAdapter::FromBitmapFlags(al_get_new_bitmap_flags());

		}
		void Bitmap::SetNewBitmapFlags(BitmapFlags flags) {

			al_set_new_bitmap_flags(System::AllegroAdapter::ToBitmapFlags(flags));

		}
		void Bitmap::ConvertMemoryBitmapsToVideoBitmaps() {

			al_convert_memory_bitmaps();

		}


		bool Bitmap::_copy_required() const {

			// Managed bitmaps are always copied when assigned to new objects, so unmanaged bitmaps can always write without making a copy.

			return _managed && _src_bitmap && _src_bitmap.use_count() > 1;

		}
		ALLEGRO_BITMAP* Bitmap::_get_bitmap_ptr() const {

			if (_sub_bitmap != nullptr)
				return _sub_bitmap;

			return _src_bitmap.get();

		}
		void Bitmap::_perform_pre_write_operations() {

			if (_copy_required())
				if (_sub_bitmap != nullptr)
					*this = std::move(Copy(RectangleI(0, 0, Width(), Height())));
				else
					*this = std::move(Copy());

		}

		void Bitmap::_copy_assign(const Bitmap& other) {

			// Free existing sub-bitmap.
			if (_sub_bitmap != nullptr)
				al_destroy_bitmap(_sub_bitmap);

			// If the other bitmap is managed, we can share memory with it. Otherwise, it must be copied.
			if (other._managed)
				_src_bitmap = other._src_bitmap;
			else
				_src_bitmap = bitmap_ptr_type(al_clone_bitmap(other._src_bitmap.get()), al_destroy_bitmap);

			// Copy the other bitmap's sub-bitmap.
			if (other._sub_bitmap != nullptr)
				_sub_bitmap = al_clone_sub_bitmap(other._sub_bitmap);

		}
		void Bitmap::_move_assign(Bitmap& other) {

			if (_sub_bitmap != nullptr)
				al_destroy_bitmap(_sub_bitmap);

			_src_bitmap = std::move(other._src_bitmap);
			_sub_bitmap = other._sub_bitmap;
			_managed = other._managed;

			other._managed = false;
			other._sub_bitmap = nullptr;

		}



		BitmapData::BitmapData() {

			Scan0 = nullptr;
			PixelFormat = -1;
			Stride = 0;
			BytesPerPixel = 0;

		}

	}
}