#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/graphics/Display.h"
#include "hvn3/io/Directory.h"
#include "hvn3/io/Path.h"
#include "hvn3/native/FileDialog.h"
#include <allegro5/allegro_native_dialog.h>

namespace hvn3 {

	FileDialog::FileDialog() :
		FileDialog(static_cast<FileDialogFlags>(0)) {
	}
	FileDialog::FileDialog(FileDialogFlags flags) :
		_title("Open"),
		_filter("*.*"),
		_flags(flags) {

		SetInitialDirectory(IO::Directory::GetCurrentDirectory());

	}
	const std::string& FileDialog::InitialDirectory() const {
		return _initial_directory;
	}
	void FileDialog::SetInitialDirectory(const std::string& value) {

		_initial_directory = value;

		// The path must end with a directory separator (everything after that is considered the filename).
		if (_initial_directory.size() > 0 && _initial_directory[_initial_directory.size() - 1u] != IO::Path::DirectorySeparatorChar())
			_initial_directory += IO::Path::DirectorySeparatorChar();

	}
	const std::string& FileDialog::FileName() const {
		return _filename;
	}
	void FileDialog::SetFileName(const std::string& value) {
		_filename = value;
	}
	const std::string& FileDialog::Title() const {
		return _title;
	}
	void FileDialog::SetTitle(const std::string& value) {
		_title = value;
	}
	const std::string& FileDialog::Filter() const {
		return _filter;
	}
	void FileDialog::SetFilter(const std::string& value) {

	}
	bool FileDialog::ShowDialog() {

		ALLEGRO_DISPLAY* display = Display::ActiveDisplay() == nullptr ? nullptr : System::AllegroAdapter::ToDisplay(*Display::ActiveDisplay());
		ALLEGRO_FILECHOOSER* dialog = _createDialog();

		bool result = al_show_native_file_dialog(display, dialog);

		_selected_files.clear();

		if (result) {

			size_t count = static_cast<size_t>(al_get_native_file_dialog_count(dialog));

			for (size_t i = 0; i < count; ++i)
				_selected_files.push_back(std::string(al_get_native_file_dialog_path(dialog, i)));

			if (_selected_files.size() > 0)
				_filename = _selected_files[0];

		}

		_freeDialog(dialog);

		return result;

	}

	ALLEGRO_FILECHOOSER* FileDialog::_createDialog() {

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
	void FileDialog::_freeDialog(ALLEGRO_FILECHOOSER* dialog) {

		if (dialog != nullptr)
			al_destroy_native_file_dialog(dialog);

	}

}