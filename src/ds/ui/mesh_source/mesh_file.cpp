#include "mesh_file.h"
#include "ds/ui/mesh_source/mesh_file_loader.h"

namespace ds {
namespace ui {

class MeshFile::FileData : public MeshSource::Data {
public:
	FileData(const std::string& filename)
			: mEngine(nullptr)
			, mFilename(filename)
			, mMeshBuilt(false) {
	}

	virtual bool					isEqual(const Data& o) const {
		const FileData*	ds = dynamic_cast<const FileData*>(&o);
		if (!ds) return false;
		return mFilename == ds->mFilename;
	}

	virtual const ci::gl::VboMeshRef	getMesh() {
		if (!mMeshBuilt) buildMesh();
		if (mMesh->getNumIndices() < 1) return nullptr;
		return mMesh;
	}

	virtual void					setEngine(SpriteEngine* e) {
		mEngine = e;
	}

private:
	void						buildMesh() {
		mMeshBuilt = true;
		mMesh = nullptr;
		
		ci::TriMesh							mesh;
	
		ds::ui::util::MeshFileLoader		loader;

		// TODO: This should be cached somewhere, or maybe just discard it? 
		// Eric, want to implement a caching layer?
		loader.Load(mFilename);

		// Convert ds::ui::util::MeshFile data into cinder::TriMesh data...
		std::vector<uint32_t> indices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texCoords;

		// Indices
		for (size_t i=0; i<loader.mNumIndices; i++ ) {
			indices.push_back( loader.mIndices[i] );
		}
		// Normals
		for (size_t i=0; i<loader.mNumNorm; i++ ) {
			normals.push_back(loader.mNorm[i]);
		}
		// Vertices
		for (size_t i=0; i<loader.mNumVert; i++ ) {
			vertices.push_back(loader.mVert[i]);
		}
		// Texture Coordinates
		for (size_t i=0; i<loader.mNumTex; i++ ) {
			texCoords.push_back(loader.mTex[i]);
		}

		// Populate the TriMesh
		if ( indices.size() > 0 ) {
			mesh.appendIndices( &indices[ 0 ], indices.size() );
		}
		if ( normals.size() > 0 ) {
			for ( auto iter = normals.begin(); iter != normals.end(); ++iter ) {
				mesh.appendNormal( *iter );
			}
		}
		if ( vertices.size() > 0 ) {
			mesh.appendPositions( &vertices[ 0 ], vertices.size() );
		}
		if ( texCoords.size() > 0 ) {
			for ( auto iter = texCoords.begin(); iter != texCoords.end(); ++iter ) {
				mesh.appendTexCoord( *iter );
			}
		}
		mMesh = ci::gl::VboMesh::create(mesh);
	}

	ds::ui::SpriteEngine*	mEngine;
	std::string				mFilename;
	// Building
	ci::gl::VboMeshRef		mMesh;
	bool					mMeshBuilt;
};

/**
 * \class ds::ui::MeshFile
 */
MeshFile::MeshFile(const std::string& filename)
		: MeshSource(std::shared_ptr<Data>(new FileData(filename))) {
}

} // namespace ui
} // namespace ds
