#include "Planet.h"
#include "TechniqueProbe.h"

Planet::Planet(const std::string & name, Graphics & gfx, const std::string & pathString, float sphereScale, double e, double a, double i, double omega, double w, double T, double t, double b, double offset, size_t pathDivisions, DirectX::XMFLOAT3 pathColor)
	:
	name(name),
	sphere(gfx, pathString, sphereScale),
	e(e),
	a(a),
	i(i),
	omega(omega),
	w(w),
	T(T),
	t(t),
	b(b),
	offset(offset),
	pathSize(pathDivisions),
	pathColor(pathColor)
{
	text = "---";
	//calculating tilt
	///angle to earth-orbit
	const double tiltAngle = b + i;
	///rotational axis
	const DirectX::XMFLOAT3 xAxis = { 1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR axis = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&xAxis), DirectX::XMMatrixRotationRollPitchYaw(0.0f, -(float)omega, 0.0f));
	tilt = DirectX::XMMatrixRotationAxis(axis, (float)tiltAngle);
	CalculateEllipse(gfx);
}

void Planet::Submit(FrameCommander & fc) const
{
	sphere.Submit(fc);
}

void Planet::SetPos(const DirectX::XMFLOAT3 & position)
{
	pos = position;
}

void Planet::SetRotation(const DirectX::XMMATRIX & rot)
{
	rotation = rot;
}

double Planet::GetA() const
{
	return a;
}

void Planet::ApplyTransforms()
{
	sphere.SetRootTransform(rotation * tilt * DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos)));
}

void Planet::CalculatePosition(double M)
{
	SetPos(ReturnPosition(M));
}

DirectX::XMFLOAT3 Planet::ReturnPosition(double M) const
{
	const double v = SolveKepler((M + offset) / T, e);

	const double sinv = sin(v);
	const double cosv = cos(v);

	const DirectX::XMFLOAT3 P = {
		(float)(cosw * cosomega - sinw * cosi * sinomega),
		(float)(cosw * sinomega + sinw * cosi * cosomega),
		(float)(sinw * sini) };
	const DirectX::XMFLOAT3 Q = {
		(float)(-sinw * cosomega - cosw * cosi * sinomega),
		(float)(-sinw * sinomega + cosw * cosi * cosomega),
		(float)(cosw * sini) };

	const double x = radiusScale * a * (cosv * (double)P.x + sinv * (double)Q.x);
	const double y = radiusScale * a * (cosv * (double)P.y + sinv * (double)Q.y);
	const double z = radiusScale * a * (cosv * (double)P.z + sinv * (double)Q.z);

	return { (float)x,(float)z,(float)y };
}

void Planet::CalculateRotation(double dt) //dt in siderischen Tagen
{
	const DirectX::XMFLOAT3 axis = { 0.0f, 1.0f, 0.0f };
	SetRotation(DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&axis), -(float)dt * 2.0f * PI / ((float)t)));
}

float * Planet::GetRadiusScale()
{
	return &radiusScale;
}

void Planet::Highlight()
{
	highlighted = true;
	//Outline
	class TP : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override
		{
			if (pTech->GetName() == "Outline")
			{
				pTech->SetActiveState(true);
			}
		}
		bool OnVisitBuffer(class Dcb::Buffer&) override
		{
			return false;
		}
	};
	TP probe;
	sphere.Accept(probe);
}

void Planet::DeHighlight()
{
	highlighted = false;
	class TP : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override
		{
			if (pTech->GetName() == "Outline")
			{
				pTech->SetActiveState(false);
			}
		}
		bool OnVisitBuffer(class Dcb::Buffer&) override
		{
			return false;
		}
	};
	TP probe;
	sphere.Accept(probe);
}

void Planet::SubmitHighlighted(FrameCommander & fc) const
{
	if (highlighted)
	{
		if (showDots)
		{
			for (const auto& s : path)
			{
				s->Submit(fc);
			}
		}
		ellipsePlane->Submit(fc);
	}
}

void Planet::SpawnInfoWindow()
{
	if (ImGui::Begin(name.c_str()))
	{
		std::string s;
		ImGui::PushStyleColor(NULL, { 0, 1.0f, 1.0f, 1.0f });
		if (ImGui::TreeNodeEx("Datensatz:"))
		{
			ImGui::PopStyleColor();
			s = "e: " + std::to_string(e);
			ImGui::Text(s.c_str());
			s = "a: " + std::to_string(a);
			ImGui::Text(s.c_str());
			s = "Umlaufzeit: " + std::to_string(T);
			ImGui::Text(s.c_str());
			s = "Rotationsperiode: " + std::to_string(t);
			ImGui::Text(s.c_str());
			s = "Neigung zur Bahnebene: " + std::to_string(b * 180.0 / PI_D) + "°";
			ImGui::Text(s.c_str());
			s = "Inklination: " + std::to_string(i * 180.0 / PI_D) + "°";
			ImGui::Text(s.c_str());
			s = "Periapsis: " + std::to_string(w * 180.0 / PI_D) + "°";
			ImGui::Text(s.c_str());
			s = "Aufsteigender Knoten: " + std::to_string(omega * 180.0 / PI_D) + "°";
			ImGui::Text(s.c_str());
			ImGui::TreePop();
		}
		else
		{
			ImGui::PopStyleColor();
		}
		if (ImGui::SliderFloat("Bahnradius", &radiusScale, 0.1f, 10.0f, "%.1f"))
		{
			RescaleEllipse();
		}
		ImGui::Checkbox("Bahnkugeln", &showDots);
	}
	ImGui::End();
}

std::string Planet::GetName() const
{
	return name;
}

DirectX::XMFLOAT3 Planet::GetPosition() const
{
	return pos;
}

void Planet::SetInfo(std::string newInfo)
{
	text = newInfo;
}

void Planet::AddInfo(std::string addInfo)
{
	text += addInfo;
}

void Planet::CalculateEllipse(Graphics& gfx)
{
	//clear all former data
	path.clear();
	ellipsePlane.release();
	//calculate all positions of path positions
	std::vector<DirectX::XMFLOAT3> ellipse;
	for (size_t j = 0; j < pathSize; j++)
	{
		path.emplace_back(std::move(std::make_unique<PointLight>(gfx, pathColor, 0.03f)));
		const DirectX::XMFLOAT3 pathPos = ReturnPosition(T * (double)j * 2.0 * PI_D / ((double)pathSize));
		path.back()->SetPos(pathPos);
		path.back()->SetStartPos(pathPos);
		ellipse.push_back(pathPos);
	}
	///create ellipse
	ellipsePlane = std::move(std::make_unique<TransparentPlane>(gfx, std::move(ellipse), pathColor));
}

void Planet::RescaleEllipse()
{
	const float scale = radiusScale / startRadiusScale;
	for (const auto& s : path)
	{
		DirectX::XMFLOAT3 sPos = s->GetStartPos();
		sPos.x *= scale;
		sPos.y *= scale;
		sPos.z *= scale;
		s->SetPos(sPos);
	}
	ellipsePlane->SetRootTransform(DirectX::XMMatrixScaling(scale, scale, scale));
}

double Planet::SolveKepler(double M, double e)
{
	double E0 = M;
	double diff = 1.0;
	while (diff > 0.0000001)
	{
		double E1 = M + e * sin(E0);
		diff = E1 - E0;
		E0 = E1;
	}
	return E0;
}
