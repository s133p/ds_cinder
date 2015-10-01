#pragma once
#ifndef DS_PHYSICS_COLLISION_H_
#define DS_PHYSICS_COLLISION_H_

#include <ds/app/app_defs.h>

namespace ds {
namespace physics {

/**
 * \class ds::physics::Collision
 * \brief Information about a collision.
 */
class Collision {
public:
	// Information about what I collided with.
	static const enum Type { EMPTY, WORLD_BOUNDS, SPRITE };
	static const enum Edge { LEFT, TOP, RIGHT, BOTTOM };

public:
	Collision();

	void			setToWorldBounds(const Edge);
	void			setToSprite(const ds::sprite_id_t);

	// Amount of force at this collision
	float			mForce;

	Type			mType;
	// If type is WORLD_BOUNDS
	Edge			mEdge;
	// If type is SPRITE
	ds::sprite_id_t	mSpriteId;

	// Contact positions are in world space
	glm::vec3		mContactOne;
	glm::vec3		mContactTwo;
	glm::vec2		mNormal;
};

} // namespace physics
} // namespace ds

#endif // DS_PHYSICS_COLLISION_H_
