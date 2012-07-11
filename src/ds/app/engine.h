#pragma once
#ifndef DS_APP_ENGINE_H_
#define DS_APP_ENGINE_H_

#include "ds/app/app_defs.h"
#include "ds/thread/work_manager.h"
#include "ds/ui/sprite/sprite.h"
#include "ds/params/update_params.h"
#include "ds/params/draw_params.h"
#include <memory>
#include "cinder/app/App.h"
#include "cinder/app/TouchEvent.h"
#include "cinder/app/AppBasic.h"
#include "TuioClient.h"
#include "ds/ui/touch/touch_manager.h"

using namespace ci;
using namespace ci::app;

namespace ds {
class WorkClient;

/**
 * \class ds::Engine
 * Container and manager for all views.
 */
class Engine {
  public:
    Engine();
    ~Engine();

    void					              update();

    // only valid after setup() is called
    ui::Sprite                 &getRootSprite();
    void                        draw();
    void                        loadCinderSettings( ci::app::App::Settings *setting );
    //called in app setup; loads settings files and what not.
    void                        setup();

    bool                        isIdling() const;
    void                        startIdling();

    tuio::Client               &getTuioClient();
    void                        mouseTouchBegin( MouseEvent event, int id );
    void                        mouseTouchMoved( MouseEvent event, int id );
    void                        mouseTouchEnded( MouseEvent event, int id );
    void                        touchesBegin( TouchEvent event );
    void                        touchesMoved( TouchEvent event );
    void                        touchesEnded( TouchEvent event );
  private:
#if defined DS_PLATFORM_SERVER || defined DS_PLATFORM_SERVERCLIENT
    friend class WorkClient;
    WorkManager				          mWorkManager;
#endif

    std::unique_ptr<ui::Sprite> mRootSprite;
    UpdateParams                mUpdateParams;
    DrawParams                  mDrawParams;
    float                       mLastTime;
    bool                        mIdling;
    float                       mLastTouchTime;
    float                       mIdleTime;

    tuio::Client                mTuio;
    ui::TouchManager            mTouchManager;
};

} // namespace ds

#endif // DS_APP_ENGINE_H_