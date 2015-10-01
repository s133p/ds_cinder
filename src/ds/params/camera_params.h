#pragma once
#ifndef DS_PARAMS_CAMERAPARAMS_H_
#define DS_PARAMS_CAMERAPARAMS_H_

#include <cinder/Vector.h>

namespace ds {

/**
 * \class ds::PerspCameraParams
 */
class PerspCameraParams {
public:
	PerspCameraParams();

	bool				operator==(const PerspCameraParams&) const;

	glm::vec3			mPosition,
						mTarget;
	float				mFov,
						mNearPlane,
						mFarPlane,
						mLensShiftH,
						mLensShiftV;

};

}

#endif
