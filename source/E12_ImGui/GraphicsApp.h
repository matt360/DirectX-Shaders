// hash # is a preprocessor statement - happens before the actual compilation happens
// include mean find a file, take all of the contents of that file and and paste in the current file
// header files don't get compiled, just cpp files.

#pragma once
#include "../DXFramework/DXF.h"
#include "GeometryExample.h"
#include "MultiLightExample.h"
#include "SpecularLightExample.h"
#include "TerrainExample.h"
#include "TessellationExample.h"
#include "TerrainTessellationExample.h"
#include "RenderToTextureExample.h"

enum class EXAMPLE_CHOICE {
	SPECULAR_LIGHT,
	TESSELLATION,
	TERRAIN,
	TERRAIN_TESSELLATION,
	MULTILIGHT,
	GEOMETRY,
	RENDER_TO_TEXTURE
};

class GraphicsApp : public BaseApplication
{
public:

	GraphicsApp();
	~GraphicsApp();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();
	void gui();

private:
	// CONSTRUCTOR FUNCTIONS
	void initGeometry();
	void initExamples();
	void loadTextures();

	// MESHES
	TriangleMesh *triangleMesh_;
	SphereMesh *sphereMesh_;
	CubeMesh *cubeMesh_;
	QuadMesh *quadMesh_;
	PlaneMesh *planeMesh_;
	TerrainMesh *terrainMesh_;
	// unique pointer - lives only in a certain scope
	// safe way to construct unique pointers
	// if the constructor happens to throw an exception
	// there won't be a dangling pointer with no reference 
	// and memory leak
	// {
	//    std::unique_ptr<PlaneMesh> up_planeMesh_ = std::make_unique<PlaneMesh>();
	// }

	// shared pointer - can hold many referenes to the memory
	// keeps track of ref count
	// std::shared_ptr<Example> example_ = std::make_shared<Example>();

	// weak pointer
	// assign a shader pointer to a weak pointer - doens't increase the ref count
	// use when you don't want to, e.g. take ownership of the entity
	// std::weak_ptr<RenderToTextureExample> renderToTextureExample_ - make_;

	Example* example_;

	// EXAMPLES
	SpecularLightExample* specularLightExample_;
	TessellationExample* tessellationExample_;
	TerrainExample* terrainExample_;
	TerrainTessellationExample* terrainTessellationExample_;
	MultiLightExample* multiLightExample_;
	GeometryExample* geometryExample_;
	RenderToTextureExample* renderToTextureExample_;
	//Example* example_;
	EXAMPLE_CHOICE example_choice_;

	// FUNCTIONS
	void setActiveExample(bool& activeEg, bool& inactiveEg1, bool& inactiveEg2, bool& inactiveEg3, bool& inactiveEg4, bool& inactiveEg5, bool& inactiveEg6);
	void chooseExample(EXAMPLE_CHOICE eg);
	BaseMesh* chooseMesh(const MESH_CHOICE& mesh_choice);
	float clamp(float n, float lower, float upper);
};
