#ifndef DS_ROCKET_SPRITE_ROCKET_H_
#define DS_ROCKET_SPRITE_ROCKET_H_

#include <ds/ui/sprite/sprite.h>
#include <ds/rocket/ciLibRocket.h>

namespace ds {
namespace ui {

class Rocket : public Sprite
{
public:
    Rocket(SpriteEngine&);

    // Loads and renders an HTML + CSS page acceptable by LibRocket.
    // For guidelines in what kind of HTML is supported, consult LibRocket
    // documentation.
    void        loadHtml(const std::string& path);

protected:
    virtual void onSizeChanged() override;
    virtual void drawLocalClient() override;
    virtual void updateServer(const ds::UpdateParams&) override;

protected:
    ci::librocket::LibRocket    mRocketInstance;
};

}} //!ds::ui

#endif //!DS_ROCKET_SPRITE_ROCKET_H_