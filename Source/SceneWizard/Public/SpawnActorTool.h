#pragma once
#include "SceneWizardModeTool.h"

class FSpawnActorTool: public FSceneWizardModeTool
{
public:
	FSpawnActorTool(FSceneWizardEdMode* EdMode);
	
	
	/** Returns the name that gets reported to the editor. */
	virtual FString GetName() const		{ return TEXT("SpawnActorTool"); }
};
