#include <cinder/app/AppNative.h>

#include <ds/app/engine/engine.h>
#include <ds/app/app.h>

#include <ds/ui/rocket.h>

class RocketExampleApp : public ds::App {
public:
    void				setupServer();
};

void RocketExampleApp::setupServer()
{
    ds::ui::Sprite &rootSprite = mEngine.getRootSprite();
    auto rocket_sprite = rootSprite.addChildPtr(new ds::ui::Rocket(mEngine));
    rocket_sprite->setSize(1024.0f, 768.0f);
    rocket_sprite->loadHtml("demo.rml");
    rocket_sprite->enable(true);
    rocket_sprite->enableMultiTouch(ds::ui::MULTITOUCH_CAN_POSITION | ds::ui::MULTITOUCH_CAN_SCALE);
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC(RocketExampleApp, ci::app::RendererGl(ci::app::RendererGl::AA_MSAA_4))
