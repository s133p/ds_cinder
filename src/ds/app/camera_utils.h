#pragma once
#ifndef DS_APP_CAMERAUTILS_H
#define DS_APP_CAMERAUTILS_H

#include <cinder/Camera.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>
#include <cinder/Matrix22.h>
#include <cinder/Matrix33.h>
#include <cinder/Matrix44.h>

namespace ds {

/**
 * \class ds::CameraPick
 * Utility for picking a sprite.
 */
class CameraPick
{
	public:
		CameraPick(	ci::Camera&, const ci::Vec3f& screenPt,
								const float screenWidth, const float screenHeight);

		const ci::Vec3f&		getScreenPt() const;
		ci::Vec2f				worldToScreen(const ci::Vec3f &worldCoord) const;

  private:
		ci::Camera&				mCamera;
		const ci::Vec3f			mScreenPt;
		const float				mScreenW,
								mScreenH;
};

/**
 * \class ds::ScreenToWorld
 * Utility for converting screen points to world points.
 * Pulled from cinder forums:
 *	http://forum.libcinder.org/topic/converting-the-mouse-position-to-3d-world-cordinates
 * NOTE: Can't get it to work. So, it'd be nice, but...
 */
class ScreenToWorld
{
	public:
		ScreenToWorld();

		void					setScreenSize(const float width, const float height);
		void					update();

		ci::Vec3f				translate(const ci::Vec3f&);

  private:
		ci::Vec3f				unproject(const ci::Vec3f&);

		ci::Matrix44f			mModelView;
		ci::Matrix44f			mProjection;
		ci::Area				mViewport;
		ci::Rectf				mWindowSize;
};

} // namespace ds

#endif // DS_APP_CAMERAUTILS_H
