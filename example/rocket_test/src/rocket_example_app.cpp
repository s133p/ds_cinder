#include <cinder/app/AppNative.h>

#include <ds/app/engine/engine.h>
#include <ds/app/app.h>

class RocketExampleApp : public ds::App {
public:
    void				setupServer();
};

void RocketExampleApp::setupServer()
{
    ds::ui::Sprite &rootSprite = mEngine.getRootSprite();
    // add sprites...
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC(RocketExampleApp, ci::app::RendererGl(ci::app::RendererGl::AA_MSAA_4))
