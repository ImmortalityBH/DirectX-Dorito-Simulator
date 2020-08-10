#pragma once

#include <DirectXMath.h>
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Vertex.h"

class Skybox
{
public:
	Skybox() = default;
	~Skybox() {}

private:
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
};

