#ifndef CI_LIBROCKET_RENDERER_CILIBROCKET_H_
#define CI_LIBROCKET_RENDERER_CILIBROCKET_H_

#include <memory>
#include <string>

#include <Rocket/Core/Plugin.h>

#include <cinder/Function.h>

#include <ds/ui/sprite/sprite.h>

namespace Rocket {
namespace Core {
template<typename T>
class StringBase;
}}

namespace cinder {
namespace app {
class MouseEvent;
class KeyEvent;
} //!app
namespace librocket {
class SystemInterface;
class RenderInterface;
class FileInterface;

class LibRocket : public Rocket::Core::Plugin {
public:
    LibRocket(ds::ui::Sprite& holder);
    virtual ~LibRocket();

    void setup();
    void loadDocument(const std::string& filePath);

    void render();
    void update();

    void registerCallbacks();
    void unregisterCallbacks();

    void toggleDebugger();
    void hideDebugger();
    void showDebugger();

public:
    bool mouseDown(const cinder::app::MouseEvent& event);
    bool mouseUp(const cinder::app::MouseEvent& event);
    bool mouseMove(const cinder::app::MouseEvent& event);
    bool mouseDrag(const cinder::app::MouseEvent& event);
    bool mouseWheel(const cinder::app::MouseEvent& event);
    bool keyDown(const cinder::app::KeyEvent& event);
    bool keyUp(const cinder::app::KeyEvent& event);
    void resize();

private:
    virtual int GetEventClasses() override;
    virtual void OnInitialise() override;
    virtual void OnShutdown() override;
    virtual void OnContextCreate(Rocket::Core::Context* context) override;
    virtual void OnContextDestroy(Rocket::Core::Context* context) override;
    virtual void OnDocumentOpen(Rocket::Core::Context* context, const Rocket::Core::StringBase<char>& document_path) override;
    virtual void OnDocumentLoad(Rocket::Core::ElementDocument* document) override;
    virtual void OnDocumentUnload(Rocket::Core::ElementDocument* document) override;
    virtual void OnElementCreate(Rocket::Core::Element* element) override;
    virtual void OnElementDestroy(Rocket::Core::Element* element) override;

private:
    ds::ui::Sprite&                 mHolder;
    std::shared_ptr<RenderInterface>    mRenderer;
    std::shared_ptr<SystemInterface>    mSystem;
    std::shared_ptr<FileInterface>      mFileSystem;

    ci::signals::scoped_connection  mCbMouseDown,
                                    mCbMouseDrag,
                                    mCbMouseUp,
                                    mCbMouseMove,
                                    mCbMouseWheel,
                                    mCbKeyDown,
                                    mCbKeyUp,
                                    mCbResize;

    Rocket::Core::Context           *mContext;
    Rocket::Core::ElementDocument   *mDocument;
};

}} //!ci::librocket

#endif //!CI_LIBROCKET_RENDERER_CILIBROCKET_H_
