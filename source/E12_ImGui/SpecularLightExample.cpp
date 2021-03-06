#include "SpecularLightExample.h"

SpecularLightExample::SpecularLightExample()
{

	shader_ = nullptr;
}

SpecularLightExample::~SpecularLightExample()
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

void SpecularLightExample::init(D3D* renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
	initLight();
}

// create shader handlers
void SpecularLightExample::initShader(D3D* renderer, HWND hwnd)
{
	shader_ = new SpecularLightShader(renderer->getDevice(), hwnd);
}

void SpecularLightExample::initVariables()
{
	mesh_choice_ = MESH_CHOICE::SPHERE;
	over_time_ = 0.0f;
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;

	texture = "grass";
}

void SpecularLightExample::initLight()
{
	// specular light example
	light_ = new Light;
	light_->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light_->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_->setDirection(0.5, -0.5f, 0.0f);
	light_->setSpecularPower(16.f);
	light_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
}

void SpecularLightExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	// WORLD MATRIX
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrix1Translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	//XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(overt_time);
	XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(0.0f);
	// orbit
	//worldMatrix = XMMatrixMultiply(matrix1Translation, matrix1Rotation);
	// translate and rotate
	worldMatrix = XMMatrixMultiply(matrix1Rotation, matrix1Translation);
	// scaling
	XMMATRIX matrix1Scaling = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	worldMatrix *= matrix1Scaling;
	// VIEW MATRIX
	viewMatrix = camera->getViewMatrix();
	// PROJECTION MATRIX
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe_);

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist_) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Send geometry data (from mesh)
	mesh_->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	// Set shader parameters (matrices and texture)
	shader_->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(texture), light_, camera);
	// Render object (combination of mesh geometry and shader process
	shader_->render(renderer->getDeviceContext(), mesh_->getIndexCount());
}

// Specular Light Example GUI window
void SpecularLightExample::gui(Camera* camera)
{
	// render only if the example is active
	if (example_)
	{
		ImGui::Begin("Specular Light", &example_);

		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		ImGui::Checkbox("Wireframe", &wireframe_);
		// scale_
		ImGui::SliderFloat3("Scale", (float*)&scale_, -25.0f, 25.0f);
		// reset scale_
		if (ImGui::Button("Reset Scale")) scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist_))
			d3d11_primitive_topology_pointlist_ = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist_))
			d3d11_primitive_topology_trianglelist_ = false;
		if (ImGui::Button("Tex: brick")) texture = "brick";
		if (ImGui::Button("Tex: bunny")) texture = "bunny";
		if (ImGui::Button("Tex: height")) texture = "height";
		if (ImGui::Button("Tex: checkerboard")) texture = "checkerboard";
		if (ImGui::Button("Tex: grass")) texture = "grass";
		if (ImGui::Button("Tex: rock")) texture = "rock";
		if (ImGui::Button("Tex: slope")) texture = "slope";

		ImGui::End();
	}
}

void SpecularLightExample::resetExample(Camera * camera)
{
	// reset camera
	camera->resetCamera();
	// reset scale_
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset wireframe
	wireframe_ = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
}
