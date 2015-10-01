#pragma once
#ifndef DS_UI_TOUCH_MANAGER_H
#define DS_UI_TOUCH_MANAGER_H

#include <map>
#include <memory>
#include <cinder/app/TouchEvent.h>
#include <cinder/app/MouseEvent.h>
#include <cinder/Color.h>
#include <cinder/Rect.h>
#include "touch_mode.h"
#include "touch_info.h"

namespace ds {
class Engine;

namespace ui {
class RotationTranslator;
class Sprite;

class TouchManager {
public:
class Capture {
public:
	Capture()				{ }
	virtual ~Capture()		{ }
	virtual void			touchBegin(const TouchInfo&) = 0;
	virtual void			touchMoved(const TouchInfo&) = 0;
	virtual void			touchEnd(const TouchInfo&) = 0;
};

public:
	TouchManager(Engine&, const TouchMode::Enum&);

	void						setTouchMode(const TouchMode::Enum&);

	void                        mouseTouchBegin(const ci::app::MouseEvent&, int id );
	void                        mouseTouchMoved(const ci::app::MouseEvent&, int id );
	void                        mouseTouchEnded(const ci::app::MouseEvent&, int id );

	void                        touchesBegin(const ci::app::TouchEvent&);
	void                        touchesMoved(const ci::app::TouchEvent&);
	void                        touchesEnded(const ci::app::TouchEvent&);

	void                        drawTouches() const;

	void                        clearFingers( const std::vector<int> &fingers );

	void						setSpriteForFinger( const int fingerId, ui::Sprite* theSprite );
	Sprite*						getSpriteForFinger( const int fingerId );

	void						setOverrideTranslation( const bool doOverride ){ mOverrideTranslation = doOverride; }
	void						setOverrideDimensions( const glm::vec2& dimensions ){ mTouchDimensions = dimensions; }
	void						setOverrideOffset( const glm::vec2& offset ){ mTouchOffset = offset; }
	void						setTouchFilterRect( const ci::Rectf &filterRect ){ mTouchFilterRect = filterRect; }

	bool						getOverrideEnabled(){ return mOverrideTranslation; }

	// If you've set the override for translation, actually do that translation
	void						overrideTouchTranslation(glm::vec2& inOutPoint);

	// If we have a rect defined to discard touches, discard that shit!
	bool						shouldDiscardTouch( const glm::vec2& p );

	void						setCapture(Capture*);

  private:
    // Utility to get the hit sprite in either the othorganal or perspective root sprites
    Sprite*                     getHit(const glm::vec3 &point);

	// If the window is stretched, the mouse points will be off. Fix that shit!
	glm::vec2					translateMousePoint(const glm::ivec2);

    Engine &mEngine;

    std::map<int, ui::Sprite*>	mFingerDispatcher;
    std::map<int, glm::vec3>	mTouchStartPoint;
    std::map<int, glm::vec3>	mTouchPreviousPoint;
	std::map<int, bool>			mDiscardTouchMap;

	glm::vec2					mTouchDimensions;
	glm::vec2					mTouchOffset;
	bool						mOverrideTranslation;
	ci::Rectf					mTouchFilterRect;

	TouchMode::Enum				mTouchMode;
	// If system multitouch is on, Cinder will get both mouse and touch events for the first touch.
	// So we track the first touch id to ignore that finger (cause the mouse will count for that)
	int							mIgnoreFirstTouchId;
	// Hack to support the touch trails
	Capture*					mCapture;

	// This is overkill but done this way so I can make changes to
	// the rotation translator without causing a recompile.
	std::shared_ptr<RotationTranslator>	mRotationTranslatorPtr;
	RotationTranslator&					mRotationTranslator;
};

} // namespace ui
} // namespace ds

#endif//DS_UI_TOUCH_MANAGER_H
