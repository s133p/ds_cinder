#pragma once
#ifndef DS_APP_ENGINE_ENGINESTANDALONE_H_
#define DS_APP_ENGINE_ENGINESTANDALONE_H_

#include "ds/app/engine/engine.h"
#include "ds/thread/gl_thread.h"
#include "ds/thread/work_manager.h"
#include "ds/ui/service/load_image_service.h"
#include "ds/ui/service/render_text_service.h"

namespace ds {

/**
 * \class ds::EngineStandalone
 * The Standalone engine contains all behaviour found in both the client
 * and server, and no communication pipe replicating sprite changes.
 */
class EngineStandalone : public Engine {
public:
	EngineStandalone(ds::App&, const ds::cfg::Settings&, ds::EngineData&, const ds::RootList&);
	~EngineStandalone();

	virtual ds::WorkManager&		getWorkManager()		{ return mWorkManager; }
	virtual ui::LoadImageService&	getLoadImageService()	{ return mLoadImageService; }
	virtual ui::RenderTextService&	getRenderTextService()	{ return mRenderTextService; }

	virtual void					installSprite(const std::function<void(ds::BlobRegistry&)>& asServer,
													const std::function<void(ds::BlobRegistry&)>& asClient);

	virtual void					setup(ds::App&);
	virtual void					setupTuio(ds::App&);
	virtual void					update();
	virtual void					draw();

	virtual void					stopServices();
	virtual int						getMode() const { return STANDALONE_MODE; }

private:
	typedef Engine inherited;
	WorkManager					mWorkManager;
	GlThread					mLoadImageThread;
	ui::LoadImageService		mLoadImageService;
	GlThread					mRenderTextThread;
	ui::RenderTextService		mRenderTextService;
};

} // namespace ds

#endif