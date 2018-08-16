#pragma once
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/utility/BitFlags.h"
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
		FileDialog();
		FileDialog(FileDialogFlags flags);

		const std::string& InitialDirectory() const;
		void SetInitialDirectory(const std::string& value);
		const std::string& FileName() const;
		void SetFileName(const std::string& value);
		const std::string& Title() const;
		void SetTitle(const std::string& value);
		const std::string& Filter() const;
		void SetFilter(const std::string& value);

		void SetDefaultExtension(const std::string& value);
		void SetAddExtensionEnabled(bool value);

		bool ShowDialog();

	private:
		std::string _initial_directory;
		std::string _filename;
		std::string _title;
		std::string _filter;
		std::string _default_extension;
		bool _add_extension_enabled;
		std::vector<std::string> _selected_files;
		FileDialogFlags _flags;

		ALLEGRO_FILECHOOSER* _createDialog();
		void _freeDialog(ALLEGRO_FILECHOOSER* dialog);

	};

}