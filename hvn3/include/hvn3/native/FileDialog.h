#pragma once
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/graphics/Display.h"
#include "hvn3/io/Directory.h"
#include "hvn3/io/Path.h"
#include "hvn3/utility/BitFlags.h"
#include <allegro5/allegro_native_dialog.h>
#include <string>
#include <vector>

namespace hvn3 {

	enum class FileDialogFlags {
		FileMustExist = 1,
		Save = 2,
		Folder = 4,
		Pictures = 8,
		ShowHidden = 16,
		MultiSelect = 32
	};
	ENABLE_BITFLAG_OPERATORS(FileDialogFlags)

	class FileDialog {

	public:
		FileDialog() :
			FileDialog(static_cast<FileDialogFlags>(0)) {
		}
		FileDialog(FileDialogFlags flags) :
			_title("Open"),
			_filter("*.*"),
			_flags(flags) {

			SetInitialDirectory(IO::Directory::GetCurrentDirectory());

		}

		const std::string& InitialDirectory() const {
			return _initial_directory;
		}
		void SetInitialDirectory(const std::string& value) {

			_initial_directory = value;

			// The path must end with a directory separator (everything after that is considered the filename).
			if (_initial_directory.size() > 0 && _initial_directory[_initial_directory.size() - 1u] != IO::Path::DirectorySeparatorChar())
				_initial_directory += IO::Path::DirectorySeparatorChar();

		}
		const std::string& FileName() const {
			return _filename;
		}
		void SetFileName(const std::string& value) {
			_filename = value;
		}
		const std::string& Title() const {
			return _title;
		}
		void SetTitle(const std::string& value) {
			_title = value;
		}
		const std::string& Filter() const {
			return _filter;
		}
		void SetFilter(const std::string& value) {

		}

		bool ShowDialog() {

			ALLEGRO_DISPLAY* display = Display::ActiveDisplay() == nullptr ? nullptr : System::AllegroAdapter::ToDisplay(*Display::ActiveDisplay());
			ALLEGRO_FILECHOOSER* dialog = _createDialog();

			bool result = al_show_native_file_dialog(display, dialog);

			_selected_files.clear();

			if (result) {

				size_t count = static_cast<size_t>(al_get_native_file_dialog_count(dialog));

				for (int i = 0; i < count; ++i)
					_selected_files.push_back(std::string(al_get_native_file_dialog_path(dialog, i)));

				if (_selected_files.size() > 0)
					_filename = _selected_files[0];

			}

			_freeDialog(dialog);

			return result;

		}

	private:
		std::string _initial_directory;
		std::string _filename;
		std::string _title;
		std::string _filter;
		std::vector<std::string> _selected_files;
		FileDialogFlags _flags;

		ALLEGRO_FILECHOOSER* _createDialog() {

			int flags = 0;

			if (HasFlag(_flags, FileDialogFlags::FileMustExist))
				flags |= ALLEGRO_FILECHOOSER_FILE_MUST_EXIST;
			if (HasFlag(_flags, FileDialogFlags::Save))
				flags |= ALLEGRO_FILECHOOSER_SAVE;
			if (HasFlag(_flags, FileDialogFlags::Folder))
				flags |= ALLEGRO_FILECHOOSER_FOLDER;
			if (HasFlag(_flags, FileDialogFlags::Pictures))
				flags |= ALLEGRO_FILECHOOSER_PICTURES;
			if (HasFlag(_flags, FileDialogFlags::ShowHidden))
				flags |= ALLEGRO_FILECHOOSER_SHOW_HIDDEN;
			if (HasFlag(_flags, FileDialogFlags::MultiSelect))
				flags |= ALLEGRO_FILECHOOSER_MULTIPLE;

			return al_create_native_file_dialog((_initial_directory + _filename).c_str(), _title.c_str(), _filter.c_str(), flags);

		}
		void _freeDialog(ALLEGRO_FILECHOOSER* dialog) {

			if (dialog != nullptr)
				al_destroy_native_file_dialog(dialog);

		}

	};

}