#include "ds/app/engine_clientserver.h"

#include "ds/app/app.h"

namespace ds {

/**
 * \class ds::EngineClientServer
 */
EngineClientServer::EngineClientServer(const ds::cfg::Settings& settings)
    : inherited(settings)
    , mLoadImageService(mLoadImageThread)
{
}

void EngineClientServer::setup()
{
  inherited::setup();

  mLoadImageThread.start(true);
}

void EngineClientServer::setupTuio(ds::App& a)
{
  tuio::Client &tuioClient = getTuioClient();
  tuioClient.registerTouches(&a);
  tuioClient.connect();
}

void EngineClientServer::update()
{
  updateServer();
}

void EngineClientServer::draw()
{
  drawClient();
}

} // namespace ds
