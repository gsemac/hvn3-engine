#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/graphics/CopyOnWriteBitmap.h"
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



		CopyOnWriteBitmap::CopyOnWriteBitmap() {

			_sub_bitmap = nullptr;
			_managed = true;

		}
		CopyOnWriteBitmap::CopyOnWriteBitmap(int width, int height) :
			CopyOnWriteBitmap() {

			if (width <= 0 || height <= 0)
				return;

			_src_bitmap = bitmap_ptr_type(al_create_bitmap(width, height), al_destroy_bitmap);

		}
		CopyOnWriteBitmap::CopyOnWriteBitmap(const std::string& file) :
			CopyOnWriteBitmap() {

			_src_bitmap = bitmap_ptr_type(al_load_bitmap(file.c_str()), al_destroy_bitmap);

		}
		CopyOnWriteBitmap::CopyOnWriteBitmap(const std::string& file, const Color& alpha_color) :
			CopyOnWriteBitmap(file) {

			ConvertMaskToAlpha(alpha_color);

		}
		CopyOnWriteBitmap::CopyOnWriteBitmap(ALLEGRO_BITMAP* bitmap, bool managed) :
			CopyOnWriteBitmap() {

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

		CopyOnWriteBitmap::CopyOnWriteBitmap(const CopyOnWriteBitmap& other, const RectangleI& region) :
			CopyOnWriteBitmap() {

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
		CopyOnWriteBitmap::CopyOnWriteBitmap(const CopyOnWriteBitmap& other) :
			CopyOnWriteBitmap() {

			_copy_assign(other);

		}
		CopyOnWriteBitmap::CopyOnWriteBitmap(CopyOnWriteBitmap&& other) :
			CopyOnWriteBitmap() {

			_move_assign(other);

		}

		CopyOnWriteBitmap::~CopyOnWriteBitmap() {

			if (_sub_bitmap != nullptr)
				al_destroy_bitmap(_sub_bitmap);

			_managed = false;
			_sub_bitmap = nullptr;

		}

		CopyOnWriteBitmap CopyOnWriteBitmap::Copy() const {

			if (_sub_bitmap != nullptr)
				return Copy(RectangleI(0, 0, Width(), Height()));

			CopyOnWriteBitmap copy;
			copy._src_bitmap = bitmap_ptr_type(al_clone_bitmap(_src_bitmap.get()), al_destroy_bitmap);

			return copy;

		}
		CopyOnWriteBitmap CopyOnWriteBitmap::Copy(const RectangleI& region) const {

			CopyOnWriteBitmap copy;
			copy._src_bitmap = bitmap_ptr_type(al_create_bitmap(region.Width(), region.Height()), al_destroy_bitmap);

			// #todo
			//Graphics(copy).DrawBitmap(0, 0, *this, region);

			return copy;

		}

		int CopyOnWriteBitmap::Width() const {

			if (_sub_bitmap != nullptr)
				return al_get_bitmap_width(_sub_bitmap);

			if (_src_bitmap)
				return al_get_bitmap_width(_src_bitmap.get());

			return 0;

		}
		int CopyOnWriteBitmap::Height() const {

			if (_sub_bitmap != nullptr)
				return al_get_bitmap_height(_sub_bitmap);

			if (_src_bitmap)
				return al_get_bitmap_height(_src_bitmap.get());

			return 0;

		}

		bool CopyOnWriteBitmap::IsVideoBitmap() const {

			return al_get_bitmap_flags(_get_bitmap_ptr()) & ALLEGRO_VIDEO_BITMAP;

		}
		void CopyOnWriteBitmap::ConvertToVideoBitmap() {

			al_convert_bitmap(_get_bitmap_ptr());

		}

		BitmapData CopyOnWriteBitmap::Lock(IO::FileAccess access) {

			_perform_pre_write_operations();

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(_get_bitmap_ptr(), al_get_bitmap_format(_get_bitmap_ptr()), to_lock_flag(access));

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		BitmapData CopyOnWriteBitmap::LockRegion(const RectangleI& region, IO::FileAccess access) {

			_perform_pre_write_operations();

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap_region(_get_bitmap_ptr(), region.X(), region.Y(), region.Width(), region.Height(), al_get_bitmap_format(_get_bitmap_ptr()), to_lock_flag(access));

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		void CopyOnWriteBitmap::Unlock() {

			if (!IsLocked())
				return;

			_perform_pre_write_operations();

			al_unlock_bitmap(_get_bitmap_ptr());

		}
		bool CopyOnWriteBitmap::IsLocked() const {

			return al_is_bitmap_locked(_get_bitmap_ptr());

		}

		void CopyOnWriteBitmap::SetPixel(int x, int y, const Color& color) {

			_perform_pre_write_operations();

			// #todo
			//Graphics(*this).DrawPoint(x, y, color);

		}
		Color CopyOnWriteBitmap::GetPixel(int x, int y) const {

			ALLEGRO_COLOR px = al_get_pixel(_get_bitmap_ptr(), x, y);

			return Color::FromArgbf(px.r, px.g, px.b, px.a);

		}

		void CopyOnWriteBitmap::ConvertMaskToAlpha(const Color& color) {

			ALLEGRO_COLOR al_color = System::AllegroAdapter::ToColor(color);

			al_convert_mask_to_alpha(_src_bitmap.get(), al_color);

		}

		CopyOnWriteBitmap& CopyOnWriteBitmap::operator=(CopyOnWriteBitmap& other) {

			_copy_assign(other);

			return *this;

		}
		CopyOnWriteBitmap& CopyOnWriteBitmap::operator=(CopyOnWriteBitmap&& other) {

			_move_assign(other);

			return *this;

		}
		CopyOnWriteBitmap::operator bool() const {

			return static_cast<bool>(_src_bitmap);

		}

		CopyOnWriteBitmap CopyOnWriteBitmap::FromBuffer(uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format) {

			CopyOnWriteBitmap copy;

			if (buffer == nullptr || buffer_size <= 0)
				return copy;

			ALLEGRO_FILE* file = al_open_memfile(buffer, buffer_size, "r");

			copy._src_bitmap = bitmap_ptr_type(al_load_bitmap_f(file, Imaging::ImageFormatToFileExtension(format).c_str()), al_destroy_bitmap);

			al_fclose(file);

			return copy;

		}



		bool CopyOnWriteBitmap::_copy_required() const {

			// Managed bitmaps are always copied when assigned to new objects, so unmanaged bitmaps can always write without making a copy.

			return _managed && _src_bitmap && _src_bitmap.use_count() > 1;

		}
		ALLEGRO_BITMAP* CopyOnWriteBitmap::_get_bitmap_ptr() const {

			if (_sub_bitmap != nullptr)
				return _sub_bitmap;

			return _src_bitmap.get();

		}
		void CopyOnWriteBitmap::_perform_pre_write_operations() {

			if (_copy_required())
				if (_sub_bitmap != nullptr)
					*this = std::move(Copy(RectangleI(0, 0, Width(), Height())));
				else
					*this = std::move(Copy());

		}

		void CopyOnWriteBitmap::_copy_assign(const CopyOnWriteBitmap& other) {

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
		void CopyOnWriteBitmap::_move_assign(CopyOnWriteBitmap& other) {

			if (_sub_bitmap != nullptr)
				al_destroy_bitmap(_sub_bitmap);

			_src_bitmap = std::move(other._src_bitmap);
			_sub_bitmap = other._sub_bitmap;
			_managed = other._managed;

			other._managed = false;
			other._sub_bitmap = nullptr;

		}

	}
}