// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"





enum ESceneWizardTools
{
	DefaultTool = 0,
	TransformTool = 1,
	SpawnActorTool = 2,
	RandomTool = 3
};


class FSceneWizardEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_SceneWizardEdModeId;
	
	FSceneWizardEdMode();
	virtual ~FSceneWizardEdMode();
	

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;

	// user input
	virtual bool InputKey(FEditorViewportClient* ViewportClient,FViewport* Viewport,FKey Key,EInputEvent Event);

	
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface


public:
	// Custom Function

	// Switch the Tool
	void SwitchTool(ESceneWizardTools ETool);


	
};
