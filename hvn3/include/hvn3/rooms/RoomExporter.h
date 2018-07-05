#pragma once
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/tilesets/TileManager.h"
#include <iostream>
#include <string>
#include <sstream>

namespace hvn3 {

	class RoomExporter {

	public:
		RoomExporter(const IRoom& room) :
			_room(&room) {
		}

		bool ToFile(const std::string& file_path) const {
			return false;
		}
		std::string ToString() const {

			std::stringstream o;

			o << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			o << "<map version=\"1.0\">\n";
			o << "  <tiles>\n    ";
			for (int i = 0; i < _room->Tiles().Count(); ++i)
				o << _room->Tiles().AtIndex(i).id << ',';
			o << "\n  </tiles>\n";
			o << "</map>";

			return o.str();

		}

	private:
		const IRoom* _room;

	};

}