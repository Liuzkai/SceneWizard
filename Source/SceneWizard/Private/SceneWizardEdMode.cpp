// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneWizardEdMode.h"
#include "SceneWizardEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "Renderer/Private/ScenePrivate.h"

#include "TransformActorTool.h"
#include "RandomTool.h"
#include "SpawnActorTool.h"

DEFINE_LOG_CATEGORY(LogSceneWizard)

const FEditorModeID FSceneWizardEdMode::EM_SceneWizardEdModeId = TEXT("EM_SceneWizardEdMode");

FSceneWizardEdMode::FSceneWizardEdMode()
{

	// Create Tool for Mode
	Tools.Add(new FSceneWizardModeTool(this));
	// 1 Transform Actor Tool
	Tools.Add(new FTransformActorTool(this));
	// 2 Spawn Actor Tool
	Tools.Add(new FSpawnActorTool(this));
	// 3 Random Tool
	Tools.Add(new FRandomTool(this));

	SetCurrentTool(Tools[ESceneWizardTools::DefaultTool]);
}

FSceneWizardEdMode::~FSceneWizardEdMode()
{

}


void FSceneWizardEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FSceneWizardEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	
}

void FSceneWizardEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	
	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FSceneWizardEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	
	// the first level short key capture
	if(Key == EKeys::T && Event == EInputEvent::IE_Pressed && !Viewport->HasMouseCapture())
	{
		if(GetCurrentTool()->GetName() != "TransformActorTool")
		{
			SwitchTool(ESceneWizardTools::TransformTool);
			return true;
		}else
		{
			return GetCurrentTool()->InputKey(ViewportClient, Viewport, Key, Event);
		}
		
	}
	if(Key == EKeys::F && Event == EInputEvent::IE_Pressed && !Viewport->HasMouseCapture())
	{
		// TODO: Switch To Spawn Actor Tool
		if(GetCurrentTool()->GetName() != "SpawnActorTool")
		{
			SwitchTool(ESceneWizardTools::SpawnActorTool);
			
			return true;
		}else
		{
			return GetCurrentTool()->InputKey(ViewportClient, Viewport, Key, Event);
		}
	}
	if(Key == EKeys::R && Event == EInputEvent::IE_Pressed && !Viewport->HasMouseCapture())
	{
		// TODO: Switch To Random Tool
		if(GetCurrentTool()->GetName() != "RandomTool")
		{
			SwitchTool(ESceneWizardTools::RandomTool);
			
			return true;
		}else
		{
			return GetCurrentTool()->InputKey(ViewportClient, Viewport, Key, Event);
		}
	}
	
	
	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FSceneWizardEdMode::UsesToolkits() const
{
	return true;
}

void FSceneWizardEdMode::SwitchTool(ESceneWizardTools ETool)
{
	if(Tools.IsValidIndex(ETool))
	{
		if(GetCurrentTool() != Tools[ETool] )
		{
			
			FSceneWizardModeTool* CurTool = static_cast<FSceneWizardModeTool*>(GetCurrentTool());
			if(CurTool)
			{
				CurTool->SetToolActive(false);
			}
			SetCurrentTool(Tools[ETool]);
			static_cast<FSceneWizardModeTool*>(Tools[ETool])->SetToolActive(true);

			// Update Log and UI
			FString CurToolName = GetCurrentTool()->GetName();
			FString LogString = FString::Printf(TEXT("Switch To %s."), *CurToolName);
			UE_LOG(LogSceneWizard, Display, TEXT("Switch To %s."), *CurToolName);
			static_cast<FSceneWizardEdModeToolkit*>(GetToolkit().Get())->UpdateWizardWidget(LogString);
		}
	}

}




