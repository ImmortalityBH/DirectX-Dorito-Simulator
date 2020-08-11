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
	Model();
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	~Model();

	void create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		std::vector<Vertex>& vertices);
	void create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		std::vector<Vertex>& vertices, std::vector<DWORD>& indices);

	bool isColliding(Model& model);

	void bind(VertexShader& vs, PixelShader& ps, Texture& tex);
	void draw();
	void drawImGUI() {}
	void unbind();
public:
	void updateMatrix(Camera& camera);
	void resetMatrix();
	void adjustPos(float x, float y, float z);
	void setPos(float x, float y, float z);
	void adjustRot(float x, float y, float z, float angle);
	void setRot(float x, float y, float z);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);
	Transform transform;
private:
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
	ConstantBuffer<CB_WVP> constantBuffer;
private:
	DirectX::XMMATRIX translation;
public:
	DirectX::XMMATRIX rotation;
private:
	DirectX::XMMATRIX sca;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX WVP;
	ID3D11DeviceContext* pContext = nullptr;
	bool isIndexed = false;
};
