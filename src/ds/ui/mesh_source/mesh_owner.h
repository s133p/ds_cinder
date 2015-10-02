#pragma once
#ifndef DS_UI_MESHSOURCE_MESHOWNER_H_
#define DS_UI_MESHSOURCE_MESHOWNER_H_

#include "mesh_source.h"

namespace ds {
namespace ui {
class SpriteEngine;

/**
 * \class ds::ui::MeshOwner
 */
class MeshOwner {
public:
	MeshOwner(SpriteEngine&);

	void					setMesh(const MeshSource&);
	void					clearMesh();
	const ci::gl::VboMeshRef	getMesh();

	void					setSphereMesh(const float radius, const int x_res=36, const int y_res=12);
	void					setFileMesh(const std::string& filename);

protected:
	virtual void			onMeshChanged();

private:
	MeshOwner();

	SpriteEngine&			mEngineForMesh;
	MeshSource				mMeshSource;
};

} // namespace ui
} // namespace ds

#endif
