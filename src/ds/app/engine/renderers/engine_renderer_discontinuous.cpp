#include "engine_renderer_discontinuous.h"

#include <ds/app/engine/engine.h>
#include <ds/app/engine/engine_data.h>
#include <ds/app/engine/engine_roots.h>

namespace ds
{

EngineRendererDiscontinuous::EngineRendererDiscontinuous(Engine& e)
	: EngineRenderer(e)
{
	ci::gl::Texture::Format textureFormat;
	textureFormat.setInternalFormat(GL_RGBA32F);
	ci::gl::Fbo::Format fmt;
	fmt.setColorTextureFormat(textureFormat);

	const auto w = static_cast<int>(e.getWidth());
	const auto h = static_cast<int>(e.getHeight());

	mFbo = ci::gl::Fbo::create(w, h, fmt);
}

void EngineRendererDiscontinuous::drawClient()
{
	mFbo->bindFramebuffer();
	ci::gl::enableAlphaBlending();
	clearScreen();
	for (auto it = mEngine.getRoots().cbegin(), end = mEngine.getRoots().cend(); it != end; ++it) {
		(*it)->drawClient(mEngine.getDrawParams(), mEngine.getAutoDrawService());
	}
	mFbo->unbindFramebuffer();

	clearScreen();
	for (const auto& world_slice : mEngine.getEngineData().mWorldSlices)
	{
		ci::gl::draw(mFbo->getTexture2d(0), world_slice.first, world_slice.second);
	}
}

void EngineRendererDiscontinuous::drawServer()
{
	/*
	glAlphaFunc(GL_GREATER, 0.001f);

	ci::gl::enable(GL_ALPHA_TEST);
	ci::gl::enableAlphaBlending();
	*/
	clearScreen();

	for (auto it = mEngine.getRoots().cbegin(), end = mEngine.getRoots().cend(); it != end; ++it)
	{
		(*it)->drawServer(mEngine.getDrawParams());
	}

	//glAlphaFunc(GL_ALWAYS, 0.001f);
}

}