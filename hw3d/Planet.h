#pragma once
#include <string>
#include "Model.h"
#include "FrameCommander.h"
#include "imgui/imgui.h"

class Planet
{
public:
	Planet(const std::string& name, Graphics& gfx, const std::string& pathString, float sphereScale, double e, double a, double i, double omega, double w, double T)
		:
		name(name),
		sphere(gfx, pathString, sphereScale),
		e(e),
		a(a),
		i(i),
		omega(omega),
		w(w),
		T(T)
	{}
	void Submit(FrameCommander& fc) const
	{
		sphere.Submit(fc);
	}
	void SetPos(const DirectX::XMFLOAT3& position)
	{
		sphere.SetRootTransform(DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position)));
	}
	double GetA() const
	{
		return a;
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

		pos = { (float)x,(float)z,(float)y };
		SetPos(pos);
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
			s = "e: " + std::to_string(e);
			ImGui::Text(s.c_str());
			s = "a: " + std::to_string(a);
			ImGui::Text(s.c_str());
			s = "Umlaufzeit: " + std::to_string(T);
			ImGui::Text(s.c_str());
			s = "Inklination: " + std::to_string(i);
			ImGui::Text(s.c_str());
			s = "Periapsis: " + std::to_string(w);
			ImGui::Text(s.c_str());
			s = "Aufsteigender Knoten: " + std::to_string(omega);
			ImGui::Text(s.c_str());
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
	//data
	double e; ///Exzentrizit‰t
	double a; ///groﬂe Halbachse
	double i; ///Inklination
	double omega; ///Winkel des aufsteigenden Knotens
	double w; ///Periapsis
	double T; ///Umlaufzeit
	float radiusScale = 2.0f;
	//calculated values
	const double sini = sin(i);
	const double cosi = cos(i);
	const double sinomega = sin(omega);
	const double cosomega = cos(omega);
	const double sinw = sin(w);
	const double cosw = cos(w);
	DirectX::XMFLOAT3 pos;
};
