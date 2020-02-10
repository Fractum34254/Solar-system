#include "Planet.h"
#include "TechniqueProbe.h"

void Planet::Highlight()
{
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
