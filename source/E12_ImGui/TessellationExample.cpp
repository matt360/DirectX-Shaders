#include "TessellationExample.h"

TessellationExample::TessellationExample()
{
	shader_ = nullptr;
}

TessellationExample::~TessellationExample()
{
	if (shader_)
	{
		delete shader_;
		shader_ = 0;
	}
}

void TessellationExample::init(D3D * renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
}

void TessellationExample::initShader(D3D * renderer, HWND hwnd)
{
	shader_ = new TessellationShader(renderer->getDevice(), hwnd);
}

void TessellationExample::initVariables()
{
	over_time_ = 0.0f;
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
}

void TessellationExample::render(D3D * renderer, Camera * camera, TextureManager * textureMgr)
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
	/*D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist_) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;*/

	// Send geometry data (from mesh)
	mesh_->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	
	// Set shader parameters (matrices and texture)
	shader_->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), camera);

	// Render object (combination of mesh geometry and shader process
	shader_->render(renderer->getDeviceContext(), mesh_->getIndexCount());
}

// Tessellation Example GUI window
void TessellationExample::gui(Camera * camera)
{
	// render only if the example is active
	if (example_)
	{
		ImGui::Begin("Tessellation", &example_);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		// wireframe
		ImGui::Checkbox("Wireframe", &wireframe_);
		// scale_
		ImGui::SliderFloat("Scale X", (float*)&scale_.x, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Z", (float*)&scale_.z, -15.0f, 15.0f);
		ImGui::SliderFloat3("Scale", (float*)&scale_, -15.0f, 15.0f);
		// reset scale_
		if (ImGui::Button("Reset Scale")) scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		/*if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist_))
		d3d11_primitive_topology_pointlist_ = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist_))
		d3d11_primitive_topology_trianglelist_ = false;*/
		ImGui::End();
	}
}

void TessellationExample::resetExample(Camera * camera)
{
	mesh_choice_ = MESH_CHOICE::TERRAIN;
	// set tessellation camera
	camera->setPosition(0.0f, 4.75f, -10.0f);
	camera->setRotation(0.0f, 30.0f, 0.0f);
	// reset terrain scale_
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset wireframe
	wireframe_ = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
}
