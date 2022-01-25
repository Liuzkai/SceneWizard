#pragma once

#include "CoreMinimal.h"
#include "EditorModeTools.h"

class FSceneWizardEdMode;

class FSceneWizardModeTool : public  FModeTool
{
public:
	FSceneWizardModeTool(FSceneWizardEdMode* EdMode)
	{
		SWEdMode = EdMode;
	}
	~FSceneWizardModeTool(){}

	// Add for Scene Wizar Mode Tool
	
	virtual bool IsToolActive(){return bIsToolActive;}
	virtual void SetToolActive(const bool bInActive){bIsToolActive = bInActive;}


protected:
	FSceneWizardEdMode* SWEdMode;
	
private:
	bool bIsToolActive;
	
	
};
