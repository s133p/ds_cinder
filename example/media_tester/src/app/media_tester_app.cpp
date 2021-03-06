#include "media_tester_app.h"

#include <cinder/Clipboard.h>

#include <Poco/String.h>
#include <Poco/Path.h>
#include <Poco/File.h>

#include <ds/app/environment.h>
#include <ds/debug/logger.h>
#include <ds/app/engine/engine.h>

#include <ds/ui/sprite/pdf.h>
#include <ds/ui/sprite/video.h>
#include <ds/ui/sprite/web.h>
#include <ds/ui/sprite/image.h>

namespace test {

media_tester::media_tester()
	: inherited() // ortho view on top
	, mMedia(nullptr)
	, mHeader(nullptr)
	, mLabelText(nullptr)
{

	/*fonts in use */
	mEngine.editFonts().install(ds::Environment::getAppFile("data/fonts/NotoSans-Bold.ttf"), "noto-bold");

	enableCommonKeystrokes(true);
}

void media_tester::setupServer(){


	/* Settings */
	mEngine.loadSettings("layout", "layout.xml");
	mEngine.loadTextCfg("text.xml");

	mEngine.getRootSprite().clearChildren();

	ds::ui::Sprite &rootSprite = mEngine.getRootSprite();
	rootSprite.setTransparent(false);
	rootSprite.setColor(ci::Color(0.3f, 0.3f, 0.3f));

	const float headerHeight = mEngine.getSettings("layout").getFloat("header:height", 0, 100.0f);
	mHeader = new ds::ui::Sprite(mEngine);
	mHeader->setSize(mEngine.getWorldWidth(), headerHeight);
	mHeader->setColor(ci::Color(0.0f, 0.0f, 0.0f));
	mHeader->setTransparent(false);
	mHeader->enable(false);
	rootSprite.addChildPtr(mHeader);

	mLabelText = mEngine.getEngineCfg().getText("header:label").create(mEngine, mHeader);
	mLabelText->setPosition(20.0f, 20.0f);
	mLabelText->setText("Drop a file or paste a file path");
	

}

void media_tester::update() {
	inherited::update();

}

void media_tester::keyDown(ci::app::KeyEvent event){
	using ci::app::KeyEvent;
	inherited::keyDown(event);

	if(event.getChar() == KeyEvent::KEY_v && event.isControlDown() && ci::Clipboard::hasString()){
		loadMedia(ci::Clipboard::getString());
		
	} else if(event.getChar() == KeyEvent::KEY_r){ // R = reload all configs and start over without quitting app
		setupServer();
	} else if(event.getChar() == KeyEvent::KEY_l){
		loadMedia("c:/test.mp4");
	}
}

void media_tester::fileDrop(ci::app::FileDropEvent event){
	for(auto it = event.getFiles().begin(); it < event.getFiles().end(); ++it){
		loadMedia((*it).string());
		return; // only do the first one
	}
}

void media_tester::loadMedia(const std::string& newMedia){

	if(!mHeader || !mLabelText) return;

	if(mMedia){
		mMedia->release();
	}
	mMedia = nullptr;

	mLabelText->setText(newMedia);

	const float headerHeight = mHeader->getHeight();

	Poco::File filey = Poco::File(newMedia);
	std::string extensionay = Poco::Path(filey.path()).getExtension();
	std::cout << "File extension: " << extensionay << std::endl;
	std::transform(extensionay.begin(), extensionay.end(), extensionay.begin(), ::tolower);

	if(newMedia.find("http") != std::string::npos || extensionay.find("gif") != std::string::npos){
		ds::ui::Web* webby = new ds::ui::Web(mEngine, mEngine.getWorldWidth(), mEngine.getWorldHeight() - headerHeight);
		mEngine.getRootSprite().addChildPtr(webby);
		webby->setDrawWhileLoading(true);
		webby->setDragScrolling(true);
		webby->setDragScrollingMinimumFingers(1);
		webby->setUrl(newMedia);
		mMedia = webby;
	} else if(extensionay.find("pdf") != std::string::npos){
		ds::ui::Pdf* pdfy = new ds::ui::Pdf(mEngine);
		mEngine.getRootSprite().addChildPtr(pdfy);
		pdfy->setResourceFilename(newMedia);
		pdfy->enable(true);
		pdfy->enableMultiTouch(ds::ui::MULTITOUCH_INFO_ONLY);
		pdfy->setTapCallback([this, pdfy](ds::ui::Sprite* sp, const ci::Vec3f& pos){
			int curPage = pdfy->getPageNum();
			curPage++;
			if(curPage > pdfy->getPageCount()){
				curPage = 1;
			}
			pdfy->setPageNum(curPage);
		});
		mMedia = pdfy;
	} else if(extensionay.find("png") != std::string::npos
			|| extensionay.find("jpg") != std::string::npos
			|| extensionay.find("jpeg") != std::string::npos
			  ){
		ds::ui::Image* imgy = new ds::ui::Image(mEngine);
		mEngine.getRootSprite().addChildPtr(imgy);
		imgy->setImageFile(newMedia);
		mMedia = imgy;
	} else {
		DS_LOG_INFO("Guessing that the new media is a video or playable by gstreamer: " << newMedia);
		ds::ui::Video* vid = new ds::ui::Video(mEngine);
		vid->setVerboseLogging(true);
		vid->loadVideo(newMedia);
		vid->play();
		mEngine.getRootSprite().addChildPtr(vid);
		mMedia = vid;

	}

	fitSpriteInArea(ci::Rectf(0.0f, headerHeight, mEngine.getWorldWidth(), mEngine.getWorldHeight()), mMedia);
}



void media_tester::fitSpriteInArea(ci::Rectf area, ds::ui::Sprite* spriddy){
	if(!mMedia) return;

	const float engAsp = area.getWidth() / area.getHeight();
	const float vidAsp = spriddy->getWidth() / spriddy->getHeight();
	float vidScale = area.getHeight() / spriddy->getHeight();
	if(vidAsp > engAsp){
		vidScale = area.getWidth() / spriddy->getWidth();
	}
	spriddy->setScale(vidScale);
	spriddy->setPosition(area.getX1() + area.getWidth() / 2.0f - spriddy->getScaleWidth() / 2.0f, area.getY1() + area.getHeight() / 2.0f - spriddy->getScaleHeight() / 2.0f);
}

} // namespace test

// This line tells Cinder to actually create the application
CINDER_APP_BASIC(test::media_tester, ci::app::RendererGl(ci::app::RendererGl::AA_MSAA_4))