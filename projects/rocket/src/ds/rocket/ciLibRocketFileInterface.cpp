#include "ciLibRocketFileInterface.h"

#include <cstdio>

namespace cinder {
namespace librocket {

FileInterface::FileInterface(const Rocket::Core::String& root)
    : mRoot(root)
{}

FileInterface::~FileInterface()
{}

// Opens a file.
Rocket::Core::FileHandle FileInterface::Open(const Rocket::Core::String& path)
{
    // Attempt to open the file relative to the application's root.
    FILE* fp = ::fopen((mRoot + path).CString(), "rb");
    if (fp)
        return (Rocket::Core::FileHandle) fp;

    // Attempt to open the file relative to the current working directory.
    fp = ::fopen(path.CString(), "rb");
    return (Rocket::Core::FileHandle) fp;
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