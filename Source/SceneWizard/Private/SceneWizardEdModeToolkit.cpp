// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneWizardEdModeToolkit.h"
#include "SceneWizardEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FSceneWizardEdModeToolkit"

FSceneWizardEdModeToolkit::FSceneWizardEdModeToolkit()
{
}

void FSceneWizardEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SAssignNew(SWTextBlock, STextBlock)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
				.Text(LOCTEXT("SWTextBlock", "No Tool Active !"))
			]
		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FSceneWizardEdModeToolkit::GetToolkitFName() const
{
	return FName("SceneWizardEdMode");
}

FText FSceneWizardEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("SceneWizardEdModeToolkit", "DisplayName", "SceneWizardEdMode Tool");
}

void FSceneWizardEdModeToolkit::UpdateWizardWidget(FString Text)
{
	if(SWTextBlock.IsValid())
	{
		SWTextBlock->SetText(FText::FromString(Text));
		
	}
}

class FEdMode* FSceneWizardEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FSceneWizardEdMode::EM_SceneWizardEdModeId);
}

#undef LOCTEXT_NAMESPACE
