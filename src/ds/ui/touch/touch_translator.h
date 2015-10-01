#pragma once
#ifndef DS_UI_TOUCH_TOUCHTRANSLATOR_H_
#define DS_UI_TOUCH_TOUCHTRANSLATOR_H_

#include <cinder/Rect.h>
#include <cinder/Vector.h>

namespace ds {
namespace ui {

/**
 * \class ds::ui::TouchTranslator
 * \brief Translate coordinates from screen space to world space.
 */
class TouchTranslator {
public:
	TouchTranslator();

	glm::ivec2		toWorldi(const int x, const int y) const;
	glm::vec2		toWorldf(const float x, const float y) const;

	void			setTranslation(const float x, const float y);
	void			setScale(const float x, const float y);

	glm::vec2		getTranslate() const		{ return glm::vec2(mTx, mTy); }
	glm::vec2		getScale() const			{ return glm::vec2(mSx, mSy); }

	void			setTouchOverlay(const ci::Rectf &src, const ci::Rectf &dst);

private:
	bool			mHasTouch;
	ci::Rectf		mTouchSrc,
					mTouchDst;
	float			mTx, mTy,
					mSx, mSy;
};

} // namespace ui
} // namespace ds

// Make the TouchTranslator available to standard stream operators
std::ostream&			operator<<(std::ostream&, const ds::ui::TouchTranslator&);
std::wostream&			operator<<(std::wostream&, const ds::ui::TouchTranslator&);

#endif
