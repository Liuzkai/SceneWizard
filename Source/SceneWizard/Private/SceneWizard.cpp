// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneWizard.h"
#include "SceneWizardEdMode.h"




#define LOCTEXT_NAMESPACE "FSceneWizardModule"


void FSceneWizardModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FSceneWizardEdMode>(FSceneWizardEdMode::EM_SceneWizardEdModeId, LOCTEXT("SceneWizardEdModeName", "SceneWizardEdMode"), FSlateIcon(), true);
}

void FSceneWizardModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FSceneWizardEdMode::EM_SceneWizardEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSceneWizardModule, SceneWizard)