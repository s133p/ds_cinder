#include "full_starter_app.h"

#include <Poco/String.h>
#include <ds/app/environment.h>
#include <ds/debug/logger.h>
#include <ds/app/engine/engine.h>

#include "app/app_defs.h"
#include "app/globals.h"

#include "events/app_events.h"

namespace fullstarter {

FullStarterApp::FullStarterApp()
	: inherited(ds::RootList()
								.ortho() // sample ortho view
								.pickColor()

								.persp() // sample perp view
								.perspFov(60.0f)
								.perspPosition(glm::vec3(0.0, 0.0f, 10.0f))
								.perspTarget(glm::vec3(0.0f, 0.0f, 0.0f))
								.perspNear(0.0002f)
								.perspFar(20.0f)

								.ortho() ) // ortho view on top
	, mGlobals(mEngine , mAllData )
	, mQueryHandler(mEngine, mAllData)
	, mIdling( false )
{


	/*fonts in use */
	mEngine.editFonts().install(ds::Environment::getAppFile("data/fonts/FONT_FILE_HERE.ttf"), "font-name-here");

	enableCommonKeystrokes(true);
}

void FullStarterApp::setupServer(){


	/* Settings */
	mEngine.loadSettings(SETTINGS_LAYOUT, "layout.xml");
	mEngine.loadTextCfg("text.xml");

	mEngine.getRootSprite(0).clearChildren();
	mEngine.getRootSprite(1).clearChildren();
	mEngine.getRootSprite(2).clearChildren();

	ds::ui::Sprite &rootSprite = mEngine.getRootSprite();
	rootSprite.setTransparent(false);
	rootSprite.setColor(ci::Color(0.2f, 0.1f, 0.6f));
	// add sprites
}

void FullStarterApp::update() {
	inherited::update();

	if( mEngine.isIdling() && !mIdling ){
		//Start idling
		mIdling = true;
		mEngine.getNotifier().notify( IdleStartedEvent() );
	} else if ( !mEngine.isIdling() && mIdling ){
		//Stop idling
		mIdling = false;
		mEngine.getNotifier().notify( IdleEndedEvent() );
	}

}

void FullStarterApp::keyDown(ci::app::KeyEvent event){
	using ci::app::KeyEvent;
	inherited::keyDown(event);
	if(event.getChar() == KeyEvent::KEY_r){ // R = reload all configs and start over without quitting app
		setupServer();
	} else if(event.getCode() == KeyEvent::KEY_d){
		moveCamera(glm::vec3(1.0f, 0.0f, 0.0f));
	} else if(event.getCode() == KeyEvent::KEY_a){
		moveCamera(glm::vec3(-1.0f, 0.0f, 0.0f));
	} else if(event.getCode() == KeyEvent::KEY_w){
		moveCamera(glm::vec3(0.0f, -1.0f, 0.0f));
	} else if(event.getCode() == KeyEvent::KEY_s){
		moveCamera(glm::vec3(0.0f, 1.0f, 0.0f));
	} else if(event.getCode() == KeyEvent::KEY_RIGHTBRACKET){
		moveCamera(glm::vec3(0.0f, 0.0f, 1.0f));
	} else if(event.getCode() == KeyEvent::KEY_LEFTBRACKET){
		moveCamera(glm::vec3(0.0f, 0.0f, -1.0f));
	} else if(event.getCode() == KeyEvent::KEY_EQUALS){
		ds::PerspCameraParams p = mEngine.getPerspectiveCamera(1);
		p.mFarPlane += 1.0f;
		std::cout << "Clip Far camera: " << p.mFarPlane << std::endl;
		mEngine.setPerspectiveCamera(1, p);
	} else if(event.getCode() == KeyEvent::KEY_MINUS){
		ds::PerspCameraParams p = mEngine.getPerspectiveCamera(1);
		p.mFarPlane -= 1.0f;
		std::cout << "Clip Far camera: " << p.mFarPlane << std::endl;
		mEngine.setPerspectiveCamera(1, p);
	}
}

void FullStarterApp::moveCamera(const glm::vec3& deltaMove){
	ds::PerspCameraParams p = mEngine.getPerspectiveCamera(1);
	p.mPosition += deltaMove;
	std::cout << "Moving camera: " << p.mPosition.x << " " << p.mPosition.y << " " << p.mPosition.z << std::endl;
	mEngine.setPerspectiveCamera(1, p);
}

} // namespace fullstarter

// This line tells Cinder to actually create the application
CINDER_APP(fullstarter::FullStarterApp, ci::app::RendererGl(ci::app::RendererGl::AA_MSAA_4), &ds::App::prepareSettings)