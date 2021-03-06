#include "file_meta_data.h"

#include <Poco/Path.h>

namespace ds {

namespace {
const std::string		EMPTY_SZ("");
}

/**
 * \class ds::FileMetaData
 */
FileMetaData::FileMetaData() {
}

FileMetaData::FileMetaData(const std::string& filename) {
	parse(filename);
}

void FileMetaData::parse(const std::string& filename) {
	mAttrib.clear();

	Poco::Path			p(filename);
	std::vector<std::string> splitString = ds::split(p.getBaseName(), ".");
	for (auto it = splitString.begin(), it2 = splitString.end(); it != it2; ++it ) {
		// First item is base name
		if (it == splitString.begin()) continue;
		std::string		str = *it;
		const size_t	pos = str.find("_");
		if (pos == std::string::npos) continue;

		std::vector<std::string> meta = split(str, "_");
		if (meta.size() != 2) continue;
		mAttrib.push_back(std::pair<std::string, std::string>(meta.front(), meta.back()));
	}
}

size_t FileMetaData::keyCount() const {
	return mAttrib.size();
}

bool FileMetaData::contains(const std::string& key) const {
	for (auto it=mAttrib.begin(), end=mAttrib.end(); it!=end; ++it) {
		if (it->first == key) return true;
	}
	return false;
}

const std::string& FileMetaData::keyAt(const size_t index) const {
	if (index >= mAttrib.size()) return EMPTY_SZ;
	return mAttrib[index].first;
}

const std::string& FileMetaData::valueAt(const size_t index) const {
	if (index >= mAttrib.size()) return EMPTY_SZ;
	return mAttrib[index].second;
}

const std::string& FileMetaData::findValue(const std::string& key) const {
	for (auto it=mAttrib.begin(), end=mAttrib.end(); it!=end; ++it) {
		if (it->first == key) return it->second;
	}
	return EMPTY_SZ;
}

} // namespace ds