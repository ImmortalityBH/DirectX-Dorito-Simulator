#pragma once

#include "Win.h"
#include "Window/Window.h"
#include "Graphics/Graphics.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Texture.h"
#include "Camera.h"
#include "Transform.h"

#include <vector>

//namespace XD {
class Model
{
public:
	Model(Graphics& gfx);
	Model(const Model&) = delete;
	~Model();

	void create(std::vector<Vertex>& vertices);
	void create(std::vector<Vertex>& vertices, std::vector<UINT>& indices);
	void createFromOBJ(LPCWSTR fileName);

	void resetMatrix();
	void update(float dt, Camera& camera);

	void move(float x, float y, float z);
	void setPos(float x, float y, float z);
	void rotate(float x, float y, float z, float Angle);
	void setRotation(float x, float y, float z, float Angle);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);

	void draw();

	void bind(VertexShader& vs, PixelShader& ps, Texture& tex);
	void unbind();

	Transform transform;
private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pConstantBuffer;

	ID3D11ShaderResourceView* pTexture;
	ID3D11SamplerState* pTexSamplerState;
private:
	DirectX::XMMATRIX sca;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX WVP;
	ConstantBuffer cb;

	Graphics* pGfx;

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	UINT vertexCount;
	UINT indexCount;
};


//}

