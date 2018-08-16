#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/graphics/Display.h"
#include "hvn3/io/Directory.h"
#include "hvn3/io/Path.h"
#include "hvn3/native/FileDialog.h"
#include "hvn3/utility/StringUtils.h"
#include <allegro5/allegro_native_dialog.h>
#include <sstream>

namespace hvn3 {

	FileDialog::FileDialog() :
		FileDialog(static_cast<FileDialogFlags>(0)) {
	}
	FileDialog::FileDialog(FileDialogFlags flags) :
		_title("Open"),
		_filter("*.*"),
		_flags(flags) {

		SetInitialDirectory(IO::Directory::GetCurrentDirectory());

		_add_extension_enabled = HasFlag(_flags, FileDialogFlags::Save);

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

		// Expects a filter string of the form: label|*.ext|label|*.ext|...
		// #todo Allegro doesn't support showing the file types in the file type drop-down.

		std::vector<std::string> split = StringUtils::Split(value, '|');
		std::stringstream formatting_string;

		for (size_t i = 1; i < split.size(); i += 2) {

			if (split[i] == "*.*") // Allegro expects "*".
				formatting_string << "*;";
			else
				formatting_string << split[i] << ';';

		}

		_filter = formatting_string.str();

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

			// Add the default file extension to all files if enabled.
			if (_add_extension_enabled && _default_extension.length() > 0)
				for (auto i = _selected_files.begin(); i != _selected_files.end(); ++i)
					if (!StringUtils::EndsWith(*i, _default_extension))
						*i += _default_extension;

			// Set the filename value to the first selected file (note that multiple may be selected if relevant flag is enabled).
			if (_selected_files.size() > 0)
				_filename = _selected_files[0];

		}

		_freeDialog(dialog);

		return result;

	}
	void FileDialog::SetDefaultExtension(const std::string& value) {
		_default_extension = value;
	}
	void FileDialog::SetAddExtensionEnabled(bool value) {
		_add_extension_enabled = value;
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