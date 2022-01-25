#pragma once

class FSceneWizardToolUtilities
{
public:
	static FVector CursorTraceHorizontalPlaneTranslate(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector Origin);

	static FVector CursorTraceVerticalPlaneTranslate(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector Origin);

	static void GetSelectionBounds(FVector& Origin, FVector& BoxExtent, float& SphereRadius);
};
