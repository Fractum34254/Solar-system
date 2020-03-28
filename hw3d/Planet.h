#pragma once
#include <string>
#include <memory>
#include "ChiliMath.h"
#include "Model.h"
#include "PointLight.h"
#include "FrameCommander.h"
#include "TransparentPlane.h"
#include "imgui/imgui.h"

class Planet
{
public:
	Planet(const std::string& name, Graphics& gfx, const std::string& pathString, float sphereScale, double e, double a, double i, double omega, double w, double T, double t, double b, double offset, size_t pathDivisions, DirectX::XMFLOAT3 pathColor);
	void Submit(FrameCommander& fc) const;
	void SetPos(const DirectX::XMFLOAT3& position);
	void SetRotation(const DirectX::XMMATRIX& rot);
	double GetA() const;
	void ApplyTransforms();
	void CalculatePosition(double M);
	DirectX::XMFLOAT3 ReturnPosition(double M) const;
	void CalculateRotation(double dt);
	float* GetRadiusScale();
	void Highlight();
	void DeHighlight();
	void SubmitHighlighted(FrameCommander& fc) const;
	void SpawnInfoWindow();
	std::string GetName() const;
	DirectX::XMFLOAT3 GetPosition() const;
	void SetInfo(std::string newInfo);
	void AddInfo(std::string addInfo);
	void RescaleEllipse();
private:
	void CalculateEllipse(Graphics& gfx);
	static double SolveKepler(double M, double e);
private:
	Model sphere;
	std::string name;
	std::string text;
	std::vector<std::unique_ptr<PointLight>> path;
	std::unique_ptr<TransparentPlane> ellipsePlane;
	size_t pathSize;
	DirectX::XMFLOAT3 pathColor;
	bool highlighted = false;
	bool showDots = false;
	bool showEllipse = false;
	//data
	double e; ///Exzentrizität
	double a; ///große Halbachse
	double i; ///Inklination
	double omega; ///Winkel des aufsteigenden Knotens
	double w; ///Periapsis
	double T; ///Umlaufzeit
	double t; ///Rotationsdauer
	double b; ///Neigung des Äquators zum Orbit
	double offset; ///Startwinkel
	const float startRadiusScale = 2.0f;
	float radiusScale = startRadiusScale;
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
