#include "TerrainExample.h"

TerrainExample::TerrainExample()
{
	shader_ = nullptr;
}

TerrainExample::~TerrainExample()
{
	if (shader_)
	{
		delete shader_;
		shader_ = 0;
	}

	if (light_)
	{
		delete light_;
		light_ = 0;
	}
}

void TerrainExample::init(D3D* renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
	initLight();
	
}

// create shader handlers
void TerrainExample::initShader(D3D* renderer, HWND hwnd)
{
	shader_ = new TerrainShader(renderer->getDevice(), hwnd);
}

void TerrainExample::initVariables()
{
	over_time_ = 0.0f;
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
	height_texture_ = "height";
	mapping_texture_1_ = "bunny";
	mapping_texture_2_ = "brick";
	frequency_ = 0.5f;
	choice_ = 0.0f;
}

void TerrainExample::initLight()
{
	// specular light example
	light_ = new Light;
	light_->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light_->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_->setDirection(0.0, 0.0f, 0.0f);
	light_->setSpecularPower(16.f);
	light_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_->setPosition(0.0f, 0.1f, 0.0f);
}

void TerrainExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();
	// translation and rotation
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrixTranslation = XMMatrixTranslation(-20.0f, 0.0, 0.0f);
	XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(180.0f));
	worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	worldMatrix *= matrixScaling;

	// wave's:
	float height = 1.0f;

	// wireframe mode
	renderer->setWireframeMode(wireframe_);

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist_) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	light_->setPosition(0.0f, sinf(over_time_ * 3.0f), 0.0f);
	// Send geometry data (from mesh)
	mesh_->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);

	// Set shader parameters (matrices and texture)
	//terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	shader_->setShaderParameters(
		renderer->getDeviceContext(), 
		worldMatrix, 
		viewMatrix, 
		projectionMatrix, 
		textureMgr->getTexture(height_texture_),
		textureMgr->getTexture(mapping_texture_1_),
		textureMgr->getTexture(mapping_texture_2_),
		light_, 
		over_time_, 
		height, 
		frequency_, 
		choice_);

	// Render object (combination of mesh geometry and shader process
	shader_->render(renderer->getDeviceContext(), mesh_->getIndexCount()); // output data from the shader programme
}

// Terrain Example GUI window
void TerrainExample::gui(Camera* camera)
{
	// render only if the example is active
	if (example_)
	{
		ImGui::Begin("Terrain", &example_);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		// wireframe
		ImGui::Checkbox("Wireframe", &wireframe_);
		// scale_
		ImGui::SliderFloat("Scale X", (float*)&scale_.x, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Y", (float*)&scale_.y, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Z", (float*)&scale_.z, -15.0f, 15.0f);
		// reset scale_
		if (ImGui::Button("Reset Scale")) scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist_))
			d3d11_primitive_topology_pointlist_ = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist_))
			d3d11_primitive_topology_trianglelist_ = false;
		// set height map texture
		if (ImGui::Button("Height Tex: brick")) height_texture_ = "brick";
		if (ImGui::Button("Height Tex: bunny")) height_texture_ = "bunny";
		if (ImGui::Button("Height Tex: height")) height_texture_ = "height";
		if (ImGui::Button("Height Tex: checkerboard")) height_texture_ = "checkerboard";
		if (ImGui::Button("Height Tex: grass")) height_texture_ = "grass";
		if (ImGui::Button("Height Tex: rock")) height_texture_ = "rock";
		if (ImGui::Button("Height Tex: slope")) height_texture_ = "slope";
		// set mapping texture 1
		if (ImGui::Button("Map Tex1: brick")) mapping_texture_1_ = "brick";
		if (ImGui::Button("Map Tex1: bunny")) mapping_texture_1_ = "bunny";
		if (ImGui::Button("Map Tex1: height")) mapping_texture_1_ = "height";
		if (ImGui::Button("Map Tex1: checkerboard")) mapping_texture_1_ = "checkerboard";
		if (ImGui::Button("Map Tex1: grass")) mapping_texture_1_ = "grass";
		if (ImGui::Button("Map Tex1: rock")) mapping_texture_1_ = "rock";
		if (ImGui::Button("Map Tex1: slope")) mapping_texture_1_ = "slope";
		// set mapping texture 2
		if (ImGui::Button("Map Tex2: brick")) mapping_texture_2_ = "brick";
		if (ImGui::Button("Map Tex2: bunny")) mapping_texture_2_ = "bunny";
		if (ImGui::Button("Map Tex2: height")) mapping_texture_2_ = "height";
		if (ImGui::Button("Map Tex2: checkerboard")) mapping_texture_2_ = "checkerboard";
		if (ImGui::Button("Map Tex2: grass")) mapping_texture_2_ = "grass";
		if (ImGui::Button("Map Tex2: rock")) mapping_texture_2_ = "rock";
		if (ImGui::Button("Map Tex2: slope")) mapping_texture_2_ = "slope";
		// Blend
		ImGui::Text("TEX ATTRIBUTES");
		ImGui::SliderFloat("Blend", (float*)&frequency_, 0.0f, 1.0f);
		// choose different texture effects
		if (ImGui::Button("Tex1"))            choice_ = 0.0f;
		if (ImGui::Button("Tex2"))            choice_ = 1.0f;
		if (ImGui::Button("Lerp Textures"))   choice_ = 2.0f;
		if (ImGui::Button("Invert Tex 1"))    choice_ = 3.0f;
		if (ImGui::Button("Invert Tex 2"))    choice_ = 4.0f;
		if (ImGui::Button("Invert Lerp Tex")) choice_ = 5.0f;
		if (ImGui::Button("Shift Col Tex 1")) choice_ = 6.0f;
		if (ImGui::Button("Shift Col Tex 2")) choice_ = 7.0f;
		if (ImGui::Button("Shift Col Lerp"))  choice_ = 8.0f;
		ImGui::End();
	}
}

void TerrainExample::resetExample(Camera * camera)
{
	mesh_choice_ = MESH_CHOICE::TERRAIN;
	// set terrain camera
	camera->setPosition(0.0f, 2.0f, -10.0f);
	camera->setRotation(0.0f, -200.0f, 0.0f);
	// reset terrain scale_
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset terrain wireframe mode
	wireframe_ = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
}
