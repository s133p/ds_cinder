#include "ds/gl/uniform.h"
#include <cinder/gl/GlslProg.h>

namespace ds {
namespace gl {

namespace {
static std::string EMPTY_SZ("");
static UniformData EMPTY_DATA;
} // !anonymous namespace

UniformData::UniformData(int count /*= 0*/, bool transpose /*= false*/)
	: mCount(count)
	, mTranspose(transpose)
{}

bool UniformData::operator==(const UniformData& rhs) const
{
	return rhs.mTranspose == mTranspose && rhs.mCount == mCount;
}

UniformVisitor::UniformVisitor(ci::gl::GlslProg& shader)
	: mShader(shader)
	, mData(EMPTY_DATA)
	, mName(EMPTY_SZ)
{}

void UniformVisitor::operator()(int data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const glm::ivec2 &data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const int *data)
{
	mShader.uniform(mName, data, mData.mCount);
}

void UniformVisitor::operator()(const glm::ivec2 *data)
{
	mShader.uniform(mName, data, mData.mCount);
}

void UniformVisitor::operator()(float data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const glm::vec2 &data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const glm::vec3 &data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const glm::vec4 &data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const ci::Color &data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const ci::ColorA &data)
{
	mShader.uniform(mName, data);
}

void UniformVisitor::operator()(const glm::mat2 &data)
{
	mShader.uniform(mName, data, mData.mTranspose);
}

void UniformVisitor::operator()(const glm::mat3 &data)
{
	mShader.uniform(mName, data, mData.mTranspose);
}

void UniformVisitor::operator()(const glm::mat4 &data)
{
	mShader.uniform(mName, data, mData.mTranspose);
}

void UniformVisitor::operator()(const float *data)
{
	mShader.uniform(mName, data, mData.mCount);
}

void UniformVisitor::operator()(const glm::vec2 *data)
{
	mShader.uniform(mName, data, mData.mCount);
}

void UniformVisitor::operator()(const glm::vec3 *data)
{
	mShader.uniform(mName, data, mData.mCount);
}

void UniformVisitor::operator()(const glm::vec4 *data)
{
	mShader.uniform(mName, data, mData.mCount);
}

void UniformVisitor::operator()(const glm::mat2 *data)
{
	mShader.uniform(mName, data, mData.mCount, mData.mTranspose);
}

void UniformVisitor::operator()(const glm::mat3 *data)
{
	mShader.uniform(mName, data, mData.mCount, mData.mTranspose);
}

void UniformVisitor::operator()(const glm::mat4 *data)
{
	mShader.uniform(mName, data, mData.mCount, mData.mTranspose);
}

/**
 * \class ds::gl::Uniform
 */
Uniform::Uniform()
	: mIsEmpty(true)
	, mEmptyDirty(true)
{}

bool Uniform::operator==(const Uniform& o) const {
	if (mVariantUniforms != o.mVariantUniforms) return false;
	return true;
}

bool Uniform::empty() const {
	if (mEmptyDirty) {
		mIsEmpty = true;
		if (!mVariantUniforms.empty()) return false;
		mEmptyDirty = false;
	}
	return mIsEmpty;
}

void Uniform::clear() {
	mVariantUniforms.clear();
	mIsEmpty = true;
	mEmptyDirty = false;
}

void Uniform::setFloat(const std::string& key, const float value) {
	if (key.empty()) return;
	set(key, value);
}

void Uniform::setFloats(const std::string& key, const std::vector<float>& value) {
	if (key.empty()) return;
	setInternal(key, UniformVisitor::SupportedVariants((float*)value.data()), UniformData(value.size()));
}

void Uniform::setInt(const std::string& key, const int value) {
	if (key.empty()) return;
	set(key, value);
}

void Uniform::setMatrix44f(const std::string& key, const glm::mat4& value) {
	if (key.empty()) return;
	set(key, value);
}

void Uniform::setVec2i(const std::string& key, const glm::ivec2& value) {
	if (key.empty()) return;
	set(key, value);
}

void Uniform::setVec4f(const std::string& key, const glm::vec4& value) {
	if (key.empty()) return;
	set(key, value);
}

void Uniform::applyTo(ci::gl::GlslProg& shader) const {
	if (!mVariantUniforms.empty())
	{
		// constructing this is really cheap. so no worries.
		auto local_visitor = UniformVisitor(shader);

		for (auto it = mVariantUniforms.cbegin(), end = mVariantUniforms.cend(); it != end; ++it)
		{
			local_visitor.mData = it->second.second;
			local_visitor.mName = it->first;
			boost::apply_visitor(local_visitor, it->second.first);
		}
	}
}

void Uniform::setInternal(const std::string& key, const UniformVisitor::SupportedVariants& val, const UniformData& data)
{
	if (key.empty()) return;
	mVariantUniforms[key] = std::make_pair(val, data);
	mEmptyDirty = true;
}

// set overloads

void Uniform::set(const std::string &name, int data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const glm::ivec2 &data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const int *data, int count)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count));
}

void Uniform::set(const std::string &name, const glm::ivec2 *data, int count)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count));
}

void Uniform::set(const std::string &name, float data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const glm::vec2 &data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const glm::vec3 &data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const glm::vec4 &data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const ci::Color &data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const ci::ColorA &data)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData());
}

void Uniform::set(const std::string &name, const glm::mat2 &data, bool transpose /*= false*/)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(0, transpose));
}

void Uniform::set(const std::string &name, const glm::mat3 &data, bool transpose /*= false*/)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(0, transpose));
}

void Uniform::set(const std::string &name, const glm::mat4 &data, bool transpose /*= false*/)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(0, transpose));
}

void Uniform::set(const std::string &name, const float *data, int count)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count));
}

void Uniform::set(const std::string &name, const glm::vec2 *data, int count)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count));
}

void Uniform::set(const std::string &name, const glm::vec3 *data, int count)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count));
}

void Uniform::set(const std::string &name, const glm::vec4 *data, int count)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count));
}

void Uniform::set(const std::string &name, const glm::mat2 *data, int count, bool transpose /*= false*/)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count, transpose));
}

void Uniform::set(const std::string &name, const glm::mat3 *data, int count, bool transpose /*= false*/)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count, transpose));
}

void Uniform::set(const std::string &name, const glm::mat4 *data, int count, bool transpose /*= false*/)
{
	setInternal(name, UniformVisitor::SupportedVariants(data), UniformData(count, transpose));
}

} // namespace gl
} // namespace ds
