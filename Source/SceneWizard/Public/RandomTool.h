#pragma once
#include "SceneWizardModeTool.h"

class FRandomTool : public FSceneWizardModeTool
{
public:
	FRandomTool(FSceneWizardEdMode* EdMode);

	
	
	/** Returns the name that gets reported to the editor. */
	virtual FString GetName() const		{ return TEXT("RandomTool"); }
};


