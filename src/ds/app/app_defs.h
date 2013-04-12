#pragma once
#ifndef DS_APP_APPDEFS_H_
#define DS_APP_APPDEFS_H_

namespace ds {

typedef int                 sprite_id_t;
// System-defined illegal sprite
static const sprite_id_t    EMPTY_SPRITE_ID = 0;

// Blob termination for network traffic
static const char           TERMINATOR_CHAR = 0;

} // namespace ds

#endif // DS_APP_APPDEFS_H_