#pragma once
#include <optional>
#include <vector>
#include <cassert>
#include "Vertex.h"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ChiliMath.h"

class ConvexPlane
{
public:
	//vertices have to be in order!
	static IndexedTriangleList Make(std::vector<DirectX::XMFLOAT3> vertices)
	{
		Dvtx::VertexBuffer vb{ std::move(Dvtx::VertexLayout{}.Append(Dvtx::VertexLayout::ElementType::Position3D)) };
		for (const auto& v : vertices)
		{
			vb.EmplaceBack(v);
		}
		std::vector<unsigned short> indices;
		assert(vertices.size() >= 3 && "Not enough vertices");
		for (size_t i = 1; i < vertices.size() - 1;)
		{
			indices.emplace_back(0);
			indices.emplace_back((unsigned short)i);
			indices.emplace_back((unsigned short)++i);
		}
		return { std::move(vb),std::move(indices) };
	}
};