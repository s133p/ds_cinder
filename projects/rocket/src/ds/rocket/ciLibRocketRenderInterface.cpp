#include "ciLibRocketRenderInterface.h"
#include <Rocket/Core.h>
#include "cinder/gl/Vbo.h"
#include "cinder/app/AppNative.h"
#include "Rocket/Core/Platform.h"

#define GL_CLAMP_TO_EDGE 0x812F

using namespace ci;
using namespace ci::app;
using namespace std;

namespace cinder {
namespace librocket {

RenderInterface::RenderInterface()
{
    mWidth = ci::app::getWindowWidth();
    mHeight = ci::app::getWindowHeight();
}

void RenderInterface::SetViewport(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

// Called by Rocket when it wants to render geometry that it does not wish to optimize.
void RenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
    vector<uint32_t>    vbo_indices;
	vector<Vec3f>       vbo_positions;
	vector<Vec2f>       vbo_texCoords;
	vector<ColorA>      vbo_colors;
    
    gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
	layout.setStaticTexCoords2d();
    layout.setStaticColorsRGBA();
    
    ci::gl::VboMesh mesh = gl::VboMesh( num_vertices, num_indices, layout, GL_TRIANGLES );
    
    Vec3f pos;
    
	for( int i=0; i < num_vertices; i++ )
    {
        vbo_positions.push_back( Vec3f(vertices[i].position.x, vertices[i].position.y, 0) );
		vbo_colors.push_back( ColorA( vertices[i].colour.red/255.f, vertices[i].colour.green/255.f, vertices[i].colour.blue/255.f, vertices[i].colour.alpha/255.f ) );
		vbo_texCoords.push_back( Vec2f( vertices[i].tex_coord.x, vertices[i].tex_coord.y ) );
	}
    
	for( int i=0; i<num_indices; i++ )
		vbo_indices.push_back( indices[i] );
    
	mesh.bufferIndices( vbo_indices );
	mesh.bufferPositions( vbo_positions );
	mesh.bufferTexCoords2d( 0, vbo_texCoords );
    mesh.bufferColorsRGBA( vbo_colors );
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    gl::enableAlphaBlending();
    gl::pushMatrices();
    gl::translate(Vec2f(translation.x, translation.y));
    gl::draw( mesh );
    
    gl::popMatrices();
    glDisable(GL_TEXTURE_2D);
    gl::disableAlphaBlending();
    
    vbo_indices.clear();
    vbo_positions.clear();
    vbo_colors.clear();
    vbo_texCoords.clear();
}


// Called by Rocket when it wants to compile geometry it believes will be static for the foreseeable future.
Rocket::Core::CompiledGeometryHandle RenderInterface::CompileGeometry(Rocket::Core::Vertex* ROCKET_UNUSED_PARAMETER(vertices), int ROCKET_UNUSED_PARAMETER(num_vertices), int* ROCKET_UNUSED_PARAMETER(indices), int ROCKET_UNUSED_PARAMETER(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED_PARAMETER(texture))
{
	return (Rocket::Core::CompiledGeometryHandle) nullptr;
}

// Called by Rocket when it wants to render application-compiled geometry.
void RenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED_PARAMETER(geometry), const Rocket::Core::Vector2f& ROCKET_UNUSED_PARAMETER(translation))
{
}

// Called by Rocket when it wants to release application-compiled geometry.
void RenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED_PARAMETER(geometry))
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void RenderInterface::EnableScissorRegion(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.
void RenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	glScissor(x, mHeight - (y + height), width, height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.
bool RenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle)
	{
		return false;
	}
	
	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);
	
	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);
    
	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures
	
	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;
			
			write_index += 4;
			read_index += color_mode;
		}
	}
    
	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;
	
	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
	delete [] image_dest;
	delete [] buffer;
	
	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool RenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}
    
	glBindTexture(GL_TEXTURE_2D, texture_id);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	texture_handle = (Rocket::Core::TextureHandle) texture_id;
    
	return true;
}

// Called by Rocket when a loaded texture is no longer required.
void RenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	glDeleteTextures(1, (GLuint*) &texture_handle);
}

}} //!ci::librocket
