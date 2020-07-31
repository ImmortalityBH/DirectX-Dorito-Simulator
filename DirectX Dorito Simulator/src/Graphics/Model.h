#pragma once

#include "Win.h"
#include "Window/Window.h"
#include "Graphics/Graphics.h"
#include "Graphics/VertexBuffer.h"
#include <Graphics/IndexBuffer.h>
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Texture.h"
#include "Camera.h"
#include "Component/Transform.h"

#include <vector>

class Model
{
public:
	Model(Graphics& gfx);
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	~Model();

	void create(std::vector<Vertex>& vertices);
	void create(std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
	
	void update(float dt, Camera& camera);
	void bind(VertexShader& vs, PixelShader& ps, Texture& tex);
	void draw();
	void unbind();
	
	void resetMatrix();
	void move(float x, float y, float z);
	void setPos(float x, float y, float z);
	void rotate(float x, float y, float z, float Angle);
	void setRotation(float x, float y, float z, float Angle);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);

	Transform transform;
private:
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
	ConstantBuffer<CB_WVP> constantBuffer;
private:
	DirectX::XMMATRIX sca;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX WVP;

	Graphics* pGfx;

	const UINT offset = 0;
	bool isIndexed = false;
};
