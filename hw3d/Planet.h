#pragma once
#include <string>
#include "ChiliMath.h"
#include "Model.h"
#include "FrameCommander.h"
#include "imgui/imgui.h"

class Planet
{
public:
	Planet(const std::string& name, Graphics& gfx, const std::string& pathString, float sphereScale, double e, double a, double i, double omega, double w, double T, double t, double b)
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
		b(b)
	{
		text = "---";
		//calculating tilt
		///angle to earth-orbit
		const double tiltAngle = b + i;
		///rotational axis
		const DirectX::XMFLOAT3 xAxis = { 1.0f, 0.0f, 0.0f };
		const DirectX::XMVECTOR axis = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&xAxis), DirectX::XMMatrixRotationRollPitchYaw(0.0f, -(float)omega - 2.0f * PI * 37.0f / 366.25f, 0.0f));
		tilt = DirectX::XMMatrixRotationAxis(axis, (float)tiltAngle);
	}
	void Submit(FrameCommander& fc) const
	{
		sphere.Submit(fc);
	}
	void SetPos(const DirectX::XMFLOAT3& position)
	{
		pos = position;
	}
	void SetRotation(const DirectX::XMMATRIX& rot)
	{
		rotation = rot;
	}
	double GetA() const
	{
		return a;
	}
	void ApplyTransforms()
	{
		sphere.SetRootTransform(rotation * tilt * DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos)));
	}
	void CalculatePosition(double M)
	{
		const double v = SolveKepler(M / T, e);

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

		SetPos({ (float)x,(float)z,(float)y });
	}
	void CalculateRotation(double dt) //dt in siderischen Tagen
	{
		const DirectX::XMFLOAT3 axis = { 0.0f, 1.0f, 0.0f };
		SetRotation(DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&axis), -(float)dt * 2.0f * PI / ((float)t)));
	}
	float* GetRadiusScale()
	{
		return &radiusScale;
	}
	void Highlight();
	void DeHighlight();
	void SpawnInfoWindow() const
	{
		if (ImGui::Begin(name.c_str()))
		{
			std::string s;
			ImGui::PushStyleColor(NULL, { 0, 1.0f, 1.0f, 1.0f });
			if(ImGui::TreeNodeEx("Datensatz:"))
			{
				ImGui::PushStyleColor(NULL, { 1.0f, 1.0f, 1.0f, 1.0f });
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
			ImGui::PushStyleColor(NULL, { 0, 1.0f, 1.0f, 1.0f });
			if (ImGui::TreeNodeEx("Weitere Informationen:"))
			{
				ImGui::PushStyleColor(NULL, { 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::Text(text.c_str());
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
	std::string GetName() const
	{
		return name;
	}
	DirectX::XMFLOAT3 GetPosition() const
	{
		return pos;
	}
	void SetInfo(std::string newInfo)
	{
		text = newInfo;
	}
	void AddInfo(std::string addInfo)
	{
		text += addInfo;
	}
private:
	static double SolveKepler(double M, double e)
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
private:
	Model sphere;
	std::string name;
	std::string text;
	//data
	double e; ///Exzentrizität
	double a; ///große Halbachse
	double i; ///Inklination
	double omega; ///Winkel des aufsteigenden Knotens
	double w; ///Periapsis
	double T; ///Umlaufzeit
	double t; ///Rotationsdauer
	double b; ///Neigung des Äquators zum Orbit
	float radiusScale = 2.0f;
	//calculated values
	const double sini = sin(i);
	const double cosi = cos(i);
	const double sinomega = sin(omega);
	const double cosomega = cos(omega);
	const double sinw = sin(w);
	const double cosw = cos(w);
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX tilt = DirectX::XMMatrixIdentity();
};
