#include "hvn3/utility/Version.h"

namespace hvn3 {

	Version::Version() :
		Version(0U, 0U, 0U, 0U) {}
	Version::Version(uint32_t major, uint32_t minor, uint32_t build, uint32_t revision) {

		_major = major;
		_minor = minor;
		_build = build;
		_revision = revision;

	}

	uint32_t Version::Major() const {

		return _major;

	}
	uint32_t Version::Minor() const {

		return _minor;

	}
	uint32_t Version::Build() const {

		return _build;

	}
	uint32_t Version::Revision() const {

		return _revision;

	}
	uint16_t Version::MajorRevision() const {

		return static_cast<uint16_t>(Revision() >> 16);

	}
	uint16_t Version::MinorRevision() const {

		return static_cast<uint16_t>(Revision() & 0x0000FFFF);

	}

	std::ostream& operator<< (std::ostream& stream, const Version& value) {

		stream << value.Major() << '.' 
			<< value.Minor() << '.'
			<< value.Build() << '.'
			<< value.Revision();

		return stream;

	}

}