#include <cinder/app/AppBasic.h>
#include <cinder/Rand.h>

#include <ds/app/app.h>
#include <ds/app/engine/engine.h>
#include <ds/app/environment.h>
#include <ds/ui/sprite/image.h>

using namespace std;
using namespace ci;
using namespace ci::app;

class AnimationApp : public ds::App {
public:
	AnimationApp();

	void				      setupServer();
	void				      mouseDown(MouseEvent event);

private:
	typedef ds::App   inherited;
	ds::ui::Sprite&    mSprite1;
	ds::ui::Image&    mSprite2;
};

AnimationApp::AnimationApp()
	: mSprite1(*(new ds::ui::Sprite(mEngine)))// , ds::Environment::getAppFolder("data", "this_is_real.jpg"))))
	, mSprite2(*(new ds::ui::Image(mEngine, ds::Environment::getAppFolder("data", "this_is_real.jpg"))))
{
}

void AnimationApp::setupServer()
{
	const glm::vec2     cen(getWindowCenter());
	ds::ui::Sprite     &rootSprite = mEngine.getRootSprite();

	mSprite1.enable(false);
	mSprite1.setCenter(0.5f, 0.5f);
	mSprite1.setPosition(cen.x, cen.y);
	mSprite1.setScale(0.5f, 0.5f);
	mSprite1.setTransparent(false);
	mSprite1.setSize(100, 100);
	mSprite1.setColor(ci::Color(1.0f, 0.2f, 0.2f));
	rootSprite.addChild(mSprite1);

	mSprite2.enable(false);
	mSprite2.setCenter(0.5f, 0.5f);
	mSprite2.setPosition(cen.x*0.25f, cen.y*0.25f);
	mSprite2.setScale(0.25f, 0.25f);
	rootSprite.addChild(mSprite2);
}

void AnimationApp::mouseDown(MouseEvent event)
{
	inherited::mouseDown(event);

	// Tween to current mouse position
	const glm::vec3       posEnd(glm::vec3(float(event.getPos().x), float(event.getPos().y), 0));

	// Tween to a randomized scale
	const float           nextScale = ci::Rand::randFloat(10.25f, 1000.5f);
	const glm::vec3       scaleEnd(glm::vec3(nextScale, nextScale, 1));
	mSprite1.tweenSize(scaleEnd, 10.0f, 1.0f, ci::EaseInOutCubic());
	mSprite1.tweenPosition(posEnd, 5.0f, 0.0f, ci::EaseInOutCubic());

	// A custom tween parameter -- only position y (also could have been done by
	// setting the end position.x and .z values to the same as the start, this is
	// here just to illustrate how to do a custom)
	ds::ui::SpriteAnim<glm::vec3>  anim_pos([](ds::ui::Sprite& s)->ci::Anim<glm::vec3>& { return s.mAnimPosition; },
											[](ds::ui::Sprite& s)->glm::vec3 { return s.getPosition(); },
											[](const glm::vec3& v, ds::ui::Sprite& s) { s.setPosition(s.getPosition().x, v.y, s.getPosition().z); });
	mEngine.getTweenline().apply(mSprite2, anim_pos, posEnd, 1.0f, EaseInOutQuart());
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( AnimationApp, RendererGl )
