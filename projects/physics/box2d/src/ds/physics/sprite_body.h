#pragma once
#ifndef DS_PHYSICS_SPRITEBODY_H_
#define DS_PHYSICS_SPRITEBODY_H_

#include <vector>
#include <functional>
#include <cinder/Vector.h>

class b2Body;
class b2DistanceJoint;

namespace ds {
namespace ui {
class Sprite;
class SpriteEngine;
struct TouchInfo;
}

namespace physics {
class BodyBuilder;
class Collision;
class Touch;
class World;

// The collision category for the world.
extern const int		WORLD_CATEGORY_BIT;

/**
 * \class ds::physics::SpriteBody
 * \brief This serves as the sprite entry point to the physics system.
 * Simply including this in a sprite subclass and calling create() is
 * enough to enable physics on a sprite.
 * By default this class will take over touch processing; if you want
 * to mix in your own behavior, take it back.
 */
class SpriteBody {
public:
	// Create a physics body on the sprite using the given world.
	// The default is the global world, which is only constructed
	// if the physics.xml setting "create_world_0" is true. Otherwise,
	// applications need to construct worlds as desired and reference them.
	SpriteBody(ds::ui::Sprite&, const int world_id = 0);
	~SpriteBody();

	bool					empty() const;
	void					create(const BodyBuilder&);

	// Distance joints will move the bodies to keep them at the length specified, but they can be at any angle.
	// damping ratio: 0.0 = no damping (faster), 1.0 = full damping (no movement)
	// frequency: how many times it's applied a second. higher frequency for smoother resolution, lower frequency for better performance (generally)
	void					createDistanceJoint(SpriteBody&, float length, float dampingRatio, float frequencyHz, const glm::vec3 bodyAOffset = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 bodyBOffset = glm::vec3(0.0f, 0.0f, 0.0f));
	void					resizeDistanceJoint(SpriteBody& body, float length);

	// Weld joints attempt to keep the two bodies at the same relative position. There will be a little bit of elasticness between the two.
	// To make a completely rigid connection, combine two fixtures into the same body (may need to add some API to handle that)
	// By default, the positioning will place the center of one body on the center of the other body, use the offsets to place the bodies somewhere else
	void					createWeldJoint(SpriteBody&, const float damping, const float frequencyHz, const glm::vec3 bodyAOffset = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 bodyBOffset = glm::vec3(0.0f, 0.0f, 0.0f));

	// Remove all joints associated with this body. destroy also does this.
	void					releaseJoints();

	// Removes the physics attached with this body, but doesn't remove the sprite.
	// Also releases any joints attached to this body (including joints initiated from a different body)
	void					destroy();
	void					setActive(bool flag);
	// Set this to false to turn the fixture into a sensor, which will cause collisions
	// on this body to stop. NOTE: This state is not tracked. If the body is empty(),
	// this does nothing, and will not effect the body when it's created.
	void					enableCollisions(const bool);


	void					processTouchInfo(ds::ui::Sprite*, const ds::ui::TouchInfo&);
	void					processTouchAdded(const ds::ui::TouchInfo&);
	void					processTouchMoved(const ds::ui::TouchInfo&);
	void					processTouchRemoved(const ds::ui::TouchInfo&);

	// Forces the physics body to this position, may result in non-natural movement. But who cares, right?
	void					setPosition(const glm::vec3&);

	void					clearVelocity();
	void					setLinearVelocity(const float x, const float y);
	void					applyForceToCenter(const float x, const float y);

	void					setRotation(const float degree);
	float					getRotation() const;
	// Set a collision callback, called whenever this body
	// collides with another physics object.
	void					setCollisionCallback(const std::function<void(const Collision&)>& fn);

	// The sprite owner is responsible for telling me when the
	// center changes. (Only necessary if the fixture is a box).
	void					onCenterChanged();

	// Returns true if the underlying b2World is locked
	// A locked world means that the physics service is
	// in the middle of a time step. it's not safe to
	// call create() or destroy() if world is locked.
	// generally it's not safe at all to manipulate world
	// while it's locked.
	bool					isWorldLocked() const;

private:
	friend class BodyBuilder;
	friend class BodyBuilderBox;
	friend class BodyBuilderCircle;
	friend class BodyBuilderPolygon;
	friend class World;
	friend class ds::physics::Touch;

	World&					mWorld;
	ds::ui::Sprite&			mSprite;
	b2Body*					mBody;
};

} // namespace physics
} // namespace ds

#endif // DS_PHYSICS_SPRITEBODY_H_
