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
    rocket_sprite->setCenter(0.5f, 0.5f);
    rocket_sprite->setSize(300.0f, 225.0f);
    rocket_sprite->setPosition(mEngine.getWorldWidth() * 0.5f, mEngine.getWorldHeight() * 0.5f);
    rocket_sprite->loadHtml("%APP%/assets/demo.rml");
    rocket_sprite->enable(true);
    rocket_sprite->enableMultiTouch(ds::ui::MULTITOUCH_CAN_POSITION | ds::ui::MULTITOUCH_CAN_SCALE);
    
    /*rocket_sprite = rootSprite.addChildPtr(new ds::ui::Rocket(mEngine));
    rocket_sprite->setCenter(1.0f, 0);
    rocket_sprite->setSize(1024.0f, 768.0f);
    rocket_sprite->setPosition(mEngine.getWorldWidth(), 0);
    rocket_sprite->loadHtml("%APP%/data/sepehr.html");
    rocket_sprite->enable(true);
    rocket_sprite->enableMultiTouch(ds::ui::MULTITOUCH_CAN_POSITION | ds::ui::MULTITOUCH_CAN_SCALE);*/
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC(RocketExampleApp, ci::app::RendererGl(ci::app::RendererGl::AA_MSAA_4))
