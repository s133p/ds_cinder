#ifndef CI_LIBROCKET_RENDERER_SYSTEM_INTERFACE_H_
#define CI_LIBROCKET_RENDERER_SYSTEM_INTERFACE_H_

#include <Rocket/Core/SystemInterface.h>

#include <cinder/app/AppNative.h>

namespace cinder {
namespace librocket {

class SystemInterface : public Rocket::Core::SystemInterface {
public:
    float GetElapsedTime() { return static_cast<float>(ci::app::getElapsedSeconds()); }
};

}} //!ci::librocket

#endif //!CI_LIBROCKET_RENDERER_SYSTEM_INTERFACE_H_
