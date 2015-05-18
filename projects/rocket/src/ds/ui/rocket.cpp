#include "rocket.h"

namespace ds {
namespace ui {

Rocket::Rocket(SpriteEngine& engine)
    : Sprite(engine)
    , mRocketInstance(*this)
{
    mRocketInstance.setup();
    setTransparent(false);
    setUseShaderTextuer(true);
}

void Rocket::onSizeChanged()
{
    Sprite::onSizeChanged();
    mRocketInstance.resize();
}

void Rocket::drawLocalClient()
{
    // DO NOT call Sprite::drawLocalClient(). LibRocket does not like it
    mRocketInstance.render();
}

void Rocket::updateServer(const ds::UpdateParams& up)
{
    Sprite::updateServer(up);
    mRocketInstance.update();
}

void Rocket::loadHtml(const std::string& path)
{
    mRocketInstance.loadDocument(path);
}

}
} //!ds::Ui