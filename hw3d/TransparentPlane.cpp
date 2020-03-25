#include "TransparentPlane.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"
#include "ConvexPlane.h"
#include "Stencil.h"

TransparentPlane::TransparentPlane(Graphics & gfx, std::vector<DirectX::XMFLOAT3> vertices, DirectX::XMFLOAT3 color)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = ConvexPlane::Make(vertices);
	const auto geometryTag = "$transPlane." + std::to_string(vertices.at(0).x) + std::to_string(vertices.at(0).y) + std::to_string(vertices.at(0).z);
	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pTopology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		Technique solid;
		Step only(0);

		auto pvs = VertexShader::Resolve(gfx, "Solid_VS.cso");
		auto pvsbc = pvs->GetBytecode();
		only.AddBindable(std::move(pvs));

		only.AddBindable(PixelShader::Resolve(gfx, "Solid_PS.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,0.8f };
			float padding;
		} colorConst;
		colorConst.color = color;
		only.AddBindable(std::make_shared<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst, 1u));

		only.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

		only.AddBindable(std::make_shared<TransformCbuf>(gfx));

		only.AddBindable(Blender::Resolve(gfx, true, 0.2f));

		only.AddBindable(Rasterizer::Resolve(gfx, true));

		solid.AddStep(std::move(only));
		AddTechnique(std::move(solid));
	}
}

void TransparentPlane::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void TransparentPlane::SetRootTransform(DirectX::XMMATRIX matrix) noexcept
{
	transform = matrix;
}

void TransparentPlane::AddRootTransform(DirectX::XMMATRIX matrix) noexcept
{
	transform *= matrix;
}

DirectX::XMMATRIX TransparentPlane::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * transform;
}
