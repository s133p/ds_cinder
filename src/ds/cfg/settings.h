#pragma once
#ifndef DS_CFG_SETTINGS_H_
#define DS_CFG_SETTINGS_H_

#include <map>
#include <vector>
#include <cinder/Color.h>
#include <cinder/Rect.h>
#include "ds/data/resource.h"

namespace cinder {
class XmlTree;
}

namespace ds {
namespace cfg {

/**
 * \class ds::cfg::Settings
 * \brief Store generic settings info.
 */
class Settings {
public:
	Settings();
	
	// Load the supplied file/string.  Currently only XML format is supported.
	// If append is true, merge all results into my existing data.  If it's
	// false, clear me out first (although only clear if the file actually
	// exists, otherwise leave me alone).
	void								readFrom(const std::string&, const bool append = true, const bool rawXmlText = false);

	bool								empty() const;
	void							  	clear();

	int								  	getBoolSize(const std::string& name) const;
	// This applies to both RGB and RGBA colours, which are always identical.
	int								  	getColorSize(const std::string& name) const;
	int								  	getFloatSize(const std::string& name) const;
	int								  	getIntSize(const std::string& name) const;
	int								  	getRectSize(const std::string& name) const;
	int									getResourceIdSize(const std::string& name) const;
	int								  	getTextSize(const std::string& name) const;
	int								  	getTextWSize(const std::string& name) const;
	int									getPointSize(const std::string& name) const;

	// Throw errors if not found
	float								getFloat(const std::string& name, const int index = 0) const;
	const cinder::Rectf&				getRect(const std::string& name, const int index = 0) const;
	int									getInt(const std::string& name, const int index = 0) const;
	const Resource::Id&					getResourceId(const std::string& name, const int index = 0) const;
	const ci::Color&					getColor(const std::string& name, const int index = 0) const;
	const ci::ColorA&					getColorA(const std::string& name, const int index = 0) const;
	const glm::vec2&					getSize(const std::string& name, const int index = 0) const;
	const std::string&					getText(const std::string& name, const int index = 0) const;
	const std::wstring&					getTextW(const std::string& name, const int index = 0) const;
	const glm::vec3&					getPoint(const std::string& name, const int index = 0) const;
	// Bools are a convenience on text fields that will be either "true" or "false"
	bool								getBool(const std::string& name, const int index = 0) const;

	// Answer the defaults if not found
	float								getFloat(const std::string& name, const int index, const float defaultValue) const;
	cinder::Rectf						getRect(const std::string& name, const int index, const ci::Rectf& defaultValue) const;
	int									getInt(const std::string& name, const int index, const int defaultValue) const;
	Resource::Id						getResourceId(const std::string& name, const int index, const Resource::Id& defaultValue) const;
	ci::Color					    	getColor(const std::string& name, const int index, const ci::Color& defaultValue) const;
	ci::ColorA						    getColorA(const std::string& name, const int index, const ci::ColorA& defaultValue) const;
	glm::vec2					    	getSize(const std::string& name, const int index, const glm::vec2& defaultValue) const;
	std::string							getText(const std::string& name, const int index, const std::string& defaultValue) const;
	std::wstring						getTextW(const std::string& name, const int index, const std::wstring& defaultValue) const;
	const glm::vec3&					getPoint(const std::string& name, const int index, const glm::vec3& defaultValue) const;
	// Bools are a convenience on text fields that will be either "true" or "false"
	bool							  	getBool(const std::string& name, const int index, const bool defaultValue) const;

	// Iterate over the keys
	void								forEachFloatKey(const std::function<void(const std::string&)>&) const;
	void								forEachRectKey(const std::function<void(const std::string&)>&) const;
	void								forEachIntKey(const std::function<void(const std::string&)>&) const;
	void								forEachColorKey(const std::function<void(const std::string&)>&) const;
	void								forEachColorAKey(const std::function<void(const std::string&)>&) const;
	void								forEachSizeKey(const std::function<void(const std::string&)>&) const;
	void								forEachTextKey(const std::function<void(const std::string&)>&) const;
	void								forEachTextWKey(const std::function<void(const std::string&)>&) const;
	void								forEachPointKey(const std::function<void(const std::string&)>&) const;

private:
	std::map<std::string, std::vector<float>>			mFloat;
	std::map<std::string, std::vector<ci::Rectf>>		mRect;
	std::map<std::string, std::vector<int>>				mInt;
	std::map<std::string, std::vector<Resource::Id>>	mRes;
	std::map<std::string, std::vector<ci::Color>>		mColor;
	std::map<std::string, std::vector<ci::ColorA>>		mColorA;
	std::map<std::string, std::vector<glm::vec2>>		mSize;
	std::map<std::string, std::vector<std::string>>		mText;
	std::map<std::string, std::vector<std::wstring>>	mTextW;
	std::map<std::string, std::vector<glm::vec3>>		mPoints;

	void								directReadFrom(const std::string& filename, const bool clear, const bool rawXmlText = false);
	void								directReadXmlFrom(const std::string& filename, const bool clear);
	void								directReadXmlFromString(const std::string& xmlStr, const bool clear);
	void								directReadXmlFromTree(const cinder::XmlTree& tree, const bool clear);

public:
	class Editor {
	public:
		static const int  SET_MODE = 0;			// Always replace/create the value
		static const int  IF_MISSING_MODE = 1;	// Only create the value if it doesn't exist

	public:
		Editor(Settings&, const int mode = SET_MODE);

		Editor&							clear();

		Editor&							setMode(const int);

		Editor&							setColor(const std::string& name, const ci::Color&);
		Editor&							setColorA(const std::string& name, const ci::ColorA&);
		Editor&							setFloat(const std::string& name, const float);
		Editor&							setInt(const std::string& name, const int);
		Editor&							setResourceId(const std::string& name, const Resource::Id&);
		Editor&							setRect(const std::string& name, const ci::Rectf&);
		Editor&							setSize(const std::string& name, const glm::vec2&);
		Editor&							setText(const std::string& name, const std::string&);
		Editor&							setPoint(const std::string& name, const glm::vec3&);

		Editor&							addInt(const std::string& name, const int);
		Editor&							addResourceId(const std::string& name, const Resource::Id&);
		Editor&							addTextW(const std::string& name, const std::wstring&);

	private:
		Settings&						mSettings;
		int							  	mMode;
	};
};

} // namespace cfg
} // namespace ds

#endif // DS_CFG_SETTINGS_H_
