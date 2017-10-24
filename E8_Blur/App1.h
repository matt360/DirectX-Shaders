// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "LightShader.h"
#include "TextureShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();

	void initLight();

	void RenderToTexture(float time);
	void RenderScene(float time);

private:
	TriangleMesh* triangleMesh;
	SphereMesh* sphereMesh;
	CubeMesh* cubeMesh;
	QuadMesh* quadMesh;
	PlaneMesh* planeMesh;
	// Shader handler
	ColourShader* colourShader;
	LightShader* lightShader;
	TextureShader* textureShader;
	
	Light* light;

	RenderTexture* renderTexture;

	OrthoMesh* orthoMesh;

	float light_y;
};

#endif