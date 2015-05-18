#include "ciLibRocket.h"
#include "ciLibRocketKeyMaps.h"

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

#include "ciLibRocketRenderInterface.h"
#include "ciLibRocketSystemInterface.h"
#include "ciLibRocketFileInterface.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace {
template <typename T>
int ciModifierKeyToLibRocket(T event)
{
    int ret = 0;

    if (event.isControlDown())
        ret |= Rocket::Core::Input::KM_CTRL;
    if (event.isShiftDown())
        ret |= Rocket::Core::Input::KM_SHIFT;
    if (event.isAltDown())
        ret |= Rocket::Core::Input::KM_ALT;
    if (event.isMetaDown())
        ret |= Rocket::Core::Input::KM_META;

    return ret;
}

// Returns the character code for a key identifier / key modifier combination.
Rocket::Core::word getCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state)
{
    // Check if we have a key code capable of generating characters on the main keyboard (ie, not on the numeric
    // keypad; that is dealt with below).
    if (key_identifier <= Rocket::Core::Input::KI_OEM_102)
    {
        // Get modifier states
        bool shift = (key_modifier_state & Rocket::Core::Input::KM_SHIFT) > 0;
        bool capslock = (key_modifier_state & Rocket::Core::Input::KM_CAPSLOCK) > 0;

        // Return character code based on identifier and modifiers
        if (shift && !capslock)
            return ascii_map[1][key_identifier];

        if (shift && capslock)
            return ascii_map[2][key_identifier];

        if (!shift && capslock)
            return ascii_map[3][key_identifier];

        return ascii_map[0][key_identifier];
    }

    // Check if we have a key code from the numeric keypad.
    else if (key_identifier <= Rocket::Core::Input::KI_OEM_NEC_EQUAL)
    {
        if (key_modifier_state & Rocket::Core::Input::KM_NUMLOCK)
            return keypad_map[0][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
        else
            return keypad_map[1][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
    }

    else if (key_identifier == Rocket::Core::Input::KI_RETURN)
        return '\n';

    return 0;
}}

namespace cinder {
namespace librocket {

LibRocket::LibRocket()
    : mContext(nullptr)
    , mDocument(nullptr)
    , mFileSystem(nullptr)
{}

LibRocket::~LibRocket()
{
    unregisterCallbacks();
    
    if (mContext)
        mContext->RemoveReference();

    Rocket::Core::Shutdown();
}

void LibRocket::setup()
{
    mRenderer = std::make_shared<RenderInterface>();
    mSystem = std::make_shared<SystemInterface>();
    mFileSystem = std::make_shared<FileInterface>(ci::app::getAssetPath("/").generic_string().c_str());

    Rocket::Core::SetFileInterface(mFileSystem.get());
    Rocket::Core::SetRenderInterface(mRenderer.get());
    Rocket::Core::SetSystemInterface(mSystem.get());

    Rocket::Core::Initialise();

    Rocket::Core::FontDatabase::LoadFontFace("Delicious-Bold.otf");
    Rocket::Core::FontDatabase::LoadFontFace("Delicious-BoldItalic.otf");
    Rocket::Core::FontDatabase::LoadFontFace("Delicious-Italic.otf");
    Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");

    Rocket::Controls::Initialise();

    mContext = nullptr;
    mContext = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(ci::app::getWindowWidth(), ci::app::getWindowHeight()));

    if (!mContext)
    {
        Rocket::Core::Shutdown();
        console() << "no LibRocket context" << std::endl;
    }

    Rocket::Debugger::Initialise(mContext);
    Rocket::Core::RegisterPlugin(this);

    registerCallbacks();
    initKeyMap();
}

void LibRocket::registerCallbacks()
{
    ci::app::WindowRef window = app::App::get()->getWindow();
	
    mCbMouseDown    = window->getSignalMouseDown().connect([this](const ci::app::MouseEvent& ev){ mouseDown(ev); });
	mCbMouseUp      = window->getSignalMouseUp().connect( [this](const ci::app::MouseEvent& ev){ mouseUp(ev); } );
	mCbMouseDrag    = window->getSignalMouseDrag().connect( [this](const ci::app::MouseEvent& ev){ mouseDrag(ev); } );
	mCbMouseMove    = window->getSignalMouseMove().connect( [this](const ci::app::MouseEvent& ev){ mouseMove(ev); } );
	mCbMouseWheel   = window->getSignalMouseWheel().connect( [this](const ci::app::MouseEvent& ev){ mouseWheel(ev); } );
	mCbKeyDown      = window->getSignalKeyDown().connect( [this](const ci::app::KeyEvent& ev){ keyDown(ev); } );
	mCbKeyUp        = window->getSignalKeyUp().connect( [this](const ci::app::KeyEvent& ev){ keyUp(ev); } );
    mCbResize       = window->getSignalResize().connect( [this](){ resize(); } );
}

void LibRocket::unregisterCallbacks()
{
    mCbMouseDown.disconnect();
    mCbMouseUp.disconnect();
    mCbMouseDrag.disconnect();
    mCbMouseMove.disconnect();
    mCbMouseWheel.disconnect();
    mCbKeyDown.disconnect();
    mCbKeyUp.disconnect();
    mCbResize.disconnect();
}

void LibRocket::loadDocument(const std::string& filePath)
{
    mDocument = mContext->LoadDocument(filePath.c_str());

    if (mDocument)
    {
        mDocument->Show();
        mDocument->RemoveReference();
    }
    else
    {
        mDocument = nullptr;
    }
}

bool LibRocket::mouseDown(const ci::app::MouseEvent& event)
{
    mContext->ProcessMouseButtonDown(getMouseButton(event), ciModifierKeyToLibRocket(event));
    return true;
}

bool LibRocket::mouseUp(const ci::app::MouseEvent& event)
{
    mContext->ProcessMouseButtonUp(getMouseButton(event), ciModifierKeyToLibRocket(event));
    return true;
}

bool LibRocket::mouseMove(const ci::app::MouseEvent& event)
{
    Vec2i pos = event.getPos();
    mContext->ProcessMouseMove(pos.x, pos.y, ciModifierKeyToLibRocket(event));
    return true;
}

bool LibRocket::mouseDrag(const ci::app::MouseEvent& event)
{
    Vec2i pos = event.getPos();
    mContext->ProcessMouseMove(pos.x, pos.y, ciModifierKeyToLibRocket(event));
    return true;
}

bool LibRocket::mouseWheel(const ci::app::MouseEvent& event)
{
    mContext->ProcessMouseWheel(static_cast<int>(event.getWheelIncrement()), ciModifierKeyToLibRocket(event));
    return true;
}

bool LibRocket::keyDown( const ci::app::KeyEvent& event )
{
    auto i = CINDER_KEY_TO_ROCKET_MAP.find(event.getCode());
    if (i != CINDER_KEY_TO_ROCKET_MAP.end())
    {
        int modifierState = ciModifierKeyToLibRocket(event);
        mContext->ProcessKeyDown(i->second, modifierState);
        Rocket::Core::word character = getCharacterCode(i->second, modifierState);
        if (character > 0)
            mContext->ProcessTextInput(character);
    }
    return true;
}

bool LibRocket::keyUp(const ci::app::KeyEvent& event)
{
    auto i = CINDER_KEY_TO_ROCKET_MAP.find(event.getCode());
    if (i != CINDER_KEY_TO_ROCKET_MAP.end())
    {
        mContext->ProcessKeyUp(i->second, ciModifierKeyToLibRocket(event));
    }
    return true;
}

void LibRocket::resize()
{
    mContext->SetDimensions(Rocket::Core::Vector2i(getWindowWidth(), getWindowHeight()));
}

int LibRocket::getMouseButton( const ci::app::MouseEvent& event )
{
    if (event.isLeftDown())       return 0;
    else if (event.isRightDown()) return 1;
    else                            return 2;
}

void LibRocket::toggleDebugger()
{
    Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
}

void LibRocket::hideDebugger()
{
    Rocket::Debugger::SetVisible(false);
}

void LibRocket::showDebugger()
{
    Rocket::Debugger::SetVisible(true);
}

void LibRocket::render()
{
    mContext->Render();
}

void LibRocket::update()
{
    mContext->Update();
}

int LibRocket::GetEventClasses()
{
    // to do.
    return 0;
}

void LibRocket::OnInitialise()
{
    // to do.
}

void LibRocket::OnShutdown()
{
    // to do.
}

void LibRocket::OnContextCreate(Rocket::Core::Context* context)
{
    // to do.
}

void LibRocket::OnContextDestroy(Rocket::Core::Context* context)
{
    // to do.
}

void LibRocket::OnDocumentOpen(Rocket::Core::Context* context, const Rocket::Core::String& document_path)
{
    // to do.
}

void LibRocket::OnDocumentLoad(Rocket::Core::ElementDocument* document)
{
    // to do.
}

void LibRocket::OnDocumentUnload(Rocket::Core::ElementDocument* document)
{
    // to do.
}

void LibRocket::OnElementCreate(Rocket::Core::Element* element)
{
    // to do.
}

void LibRocket::OnElementDestroy(Rocket::Core::Element* element)
{
    // to do.
}

}} //!ci::librocket
