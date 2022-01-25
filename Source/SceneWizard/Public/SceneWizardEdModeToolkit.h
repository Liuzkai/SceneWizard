// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FSceneWizardEdModeToolkit : public FModeToolkit
{
public:

	FSceneWizardEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

public:
	void UpdateWizardWidget(FString Text);

private:

	TSharedPtr<SWidget> ToolkitWidget;
	TSharedPtr<STextBlock> SWTextBlock;
};
