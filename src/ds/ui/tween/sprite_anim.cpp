#include "ds/ui/tween/sprite_anim.h"

#include "ds/ui/sprite/sprite.h"
#include "ds/ui/sprite/sprite_engine.h"
#include "ds/ui/tween/tweenline.h"

namespace ds {
namespace ui {

/**
 * \class ds::ui::SpriteAnimatable
 */
SpriteAnimatable::SpriteAnimatable(Sprite& s, SpriteEngine& e)
		: mOwner(s)
		, mEngine(e) {
}

SpriteAnimatable::~SpriteAnimatable() {
}

const SpriteAnim<ci::Color>& SpriteAnimatable::ANIM_COLOR() {
  static ds::ui::SpriteAnim<ci::Color>  ANIM(
    [](ds::ui::Sprite& s)->ci::Anim<ci::Color>& { return s.mAnimColor; },
    [](ds::ui::Sprite& s)->ci::Color { return s.getColor(); },
    [](const ci::Color& v, ds::ui::Sprite& s) { s.setColor(v); });
  return ANIM;
}

const SpriteAnim<float>& SpriteAnimatable::ANIM_OPACITY() {
  static ds::ui::SpriteAnim<float>  ANIM(
          [](ds::ui::Sprite& s)->ci::Anim<float>& { return s.mAnimOpacity; },
          [](ds::ui::Sprite& s)->float { return s.getOpacity(); },
          [](const float& v, ds::ui::Sprite& s) { s.setOpacity(v); });
  return ANIM;
}

const SpriteAnim<glm::vec3>& SpriteAnimatable::ANIM_POSITION() {
  static ds::ui::SpriteAnim<glm::vec3>  ANIM(
          [](ds::ui::Sprite& s)->ci::Anim<glm::vec3>& { return s.mAnimPosition; },
          [](ds::ui::Sprite& s)->glm::vec3 { return s.getPosition(); },
          [](const glm::vec3& v, ds::ui::Sprite& s) { s.setPosition(v); });
  return ANIM;
}

const SpriteAnim<glm::vec3>& SpriteAnimatable::ANIM_SCALE() {
  static ds::ui::SpriteAnim<glm::vec3>  ANIM(
          [](ds::ui::Sprite& s)->ci::Anim<glm::vec3>& { return s.mAnimScale; },
          [](ds::ui::Sprite& s)->glm::vec3 { return s.getScale(); },
          [](const glm::vec3& v, ds::ui::Sprite& s) { s.setScale(v); });
  return ANIM;
}

const SpriteAnim<glm::vec3>& SpriteAnimatable::ANIM_SIZE() {
  static ds::ui::SpriteAnim<glm::vec3>  ANIM(
          [](ds::ui::Sprite& s)->ci::Anim<glm::vec3>& { return s.mAnimSize; },
          [](ds::ui::Sprite& s)->glm::vec3 { return glm::vec3(s.getWidth(), s.getHeight(), s.getDepth()); },
          [](const glm::vec3& v, ds::ui::Sprite& s) { s.setSizeAll(v.x, v.y, v.z); });
  return ANIM;
}

const SpriteAnim<glm::vec3>& SpriteAnimatable::ANIM_ROTATION() {
  static ds::ui::SpriteAnim<glm::vec3>  ANIM(
    [](ds::ui::Sprite& s)->ci::Anim<glm::vec3>& { return s.mAnimRotation; },
    [](ds::ui::Sprite& s)->glm::vec3 { return s.getRotation(); },
    [](const glm::vec3& v, ds::ui::Sprite& s) { s.setRotation(v); });
  return ANIM;
}

void SpriteAnimatable::tweenColor(	const ci::Color& c, const float duration, const float delay,
									const ci::EaseFn& ease, const std::function<void(void)>& finishFn) {
	mAnimColor.stop();
	mEngine.getTweenline().apply(mOwner, ANIM_COLOR(), c, duration, ease, finishFn, delay);
}

void SpriteAnimatable::tweenOpacity(const float opacity, const float duration, const float delay,
									const ci::EaseFn& ease, const std::function<void(void)>& finishFn) {
	mAnimOpacity.stop();
	mEngine.getTweenline().apply(mOwner, ANIM_OPACITY(), opacity, duration, ease, finishFn, delay);
}

void SpriteAnimatable::tweenPosition(const glm::vec3& pos, const float duration, const float delay,
									const ci::EaseFn& ease, const std::function<void(void)>& finishFn) {
	mAnimPosition.stop();
	mEngine.getTweenline().apply(mOwner, ANIM_POSITION(), pos, duration, ease, finishFn, delay);
}

void SpriteAnimatable::tweenRotation(const glm::vec3& rot, const float duration, const float delay,
									const ci::EaseFn& ease, const std::function<void(void)>& finishFn) {
	mAnimRotation.stop();
	mEngine.getTweenline().apply(mOwner, ANIM_ROTATION(), rot, duration, ease, finishFn, delay);
}

void SpriteAnimatable::tweenScale(	const glm::vec3& scale, const float duration, const float delay,
									const ci::EaseFn& ease, const std::function<void(void)>& finishFn) {
	mAnimScale.stop();
	mEngine.getTweenline().apply(mOwner, ANIM_SCALE(), scale, duration, ease, finishFn, delay);
}

void SpriteAnimatable::tweenSize(	const glm::vec3& size, const float duration, const float delay,
									const ci::EaseFn& ease, const std::function<void(void)>& finishFn) {
	mAnimSize.stop();
	mEngine.getTweenline().apply(mOwner, ANIM_SIZE(), size, duration, ease, finishFn, delay);
}

void SpriteAnimatable::animStop() {
	mAnimColor.stop();
	mAnimOpacity.stop();
	mAnimPosition.stop();
	mAnimScale.stop();
	mAnimSize.stop();
}

} // namespace ui
} // namespace ds
