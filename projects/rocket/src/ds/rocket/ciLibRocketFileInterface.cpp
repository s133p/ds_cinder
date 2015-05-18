#include "ciLibRocketFileInterface.h"

#include <cstdio>

#include <ds/app/environment.h>

#include <cinder/Filesystem.h>

namespace cinder {
namespace librocket {

FileInterface::FileInterface(const Rocket::Core::String& root)
    : mRoot(root.CString())
{}

FileInterface::~FileInterface()
{}

// Opens a file.
Rocket::Core::FileHandle FileInterface::Open(const Rocket::Core::String& path)
{
    std::string _path(path.CString());
    _path = ds::Environment::expand(_path);
        
    ci::fs::path _boost_path(_path);

    if (_boost_path.is_absolute()) {
        return (Rocket::Core::FileHandle)::fopen(_path.c_str(), "rb");
    } else {
        return (Rocket::Core::FileHandle)::fopen((mRoot + _path).c_str(), "rb");
    }
}

// Closes a previously opened file.
void FileInterface::Close(Rocket::Core::FileHandle file)
{
    ::fclose((FILE*)file);
}

// Reads data from a previously opened file.
size_t FileInterface::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
    return ::fread(buffer, 1, size, (FILE*)file);
}

// Seeks to a point in a previously opened file.
bool FileInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
    return ::fseek((FILE*)file, offset, origin) == 0;
}

// Returns the current position of the file pointer.
size_t FileInterface::Tell(Rocket::Core::FileHandle file)
{
    return ::ftell((FILE*)file);
}

}} //!ci::librocket