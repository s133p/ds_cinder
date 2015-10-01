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
		CameraPick(	ci::Camera&, const glm::vec3& screenPt,
								const float screenWidth, const float screenHeight);

		const glm::vec3&		getScreenPt() const;
		glm::vec2				worldToScreen(const glm::vec3 &worldCoord) const;

  private:
		ci::Camera&				mCamera;
		const glm::vec3			mScreenPt;
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

		glm::vec3				translate(const glm::vec3&);

  private:
		glm::vec3				unproject(const glm::vec3&);

		glm::mat4			mModelView;
		glm::mat4			mProjection;
		ci::Area				mViewport;
		ci::Rectf				mWindowSize;
};

} // namespace ds

#endif // DS_APP_CAMERAUTILS_H
