#pragma once
#include <iostream>
#include <cstdint>

namespace hvn3 {

	class Version {

	public:
		Version();
		Version(uint32_t major, uint32_t minor, uint32_t build = 0U, uint32_t revision = 0U);

		// Returns the major component of the version number.
		uint32_t Major() const;
		// Returns the minor component of the version number.
		uint32_t Minor() const;		
		// Returns the build component of the version number.
		uint32_t Build() const;
		// Returns the revision component of the version number.
		uint32_t Revision() const;
		// Returns the high 16 bits of the revision number.
		uint16_t MajorRevision() const;
		// Returns the low 16 bits of the revision number.
		uint16_t MinorRevision() const;

	private:
		uint32_t _build;
		uint32_t _major;
		uint32_t _minor;
		uint32_t _revision;

	};

	std::ostream& operator<< (std::ostream& stream, const Version& value);

}