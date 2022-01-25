#include "SceneWizardToolUtilities.h"
#include "CoreMinimal.h"
#include "Engine/Selection.h"


FVector FSceneWizardToolUtilities::CursorTraceHorizontalPlaneTranslate(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector Origin)
{
	int32 X = Viewport->GetMouseX();
	int32 Y = Viewport->GetMouseY();
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
																				ViewportClient->Viewport, 
																				ViewportClient->GetScene(),
																				ViewportClient->EngineShowFlags)
																				.SetRealtimeUpdate( true ));
	FSceneView* View = ViewportClient->CalcSceneView( &ViewFamily );
	FViewportCursorLocation Cursor(View, ViewportClient, X, Y);

	const auto ViewportType = ViewportClient->GetViewportType();
	
	FVector RayStart = Cursor.GetOrigin();
	FVector Direction = Cursor.GetDirection();
	if (ViewportType == LVT_OrthoXY || ViewportType == LVT_OrthoXZ || ViewportType == LVT_OrthoYZ ||
	ViewportType == LVT_OrthoNegativeXY || ViewportType == LVT_OrthoNegativeXZ || ViewportType == LVT_OrthoNegativeYZ)
	{
		RayStart -= Direction * HALF_WORLD_MAX/2;
	}

	FVector PlaneNormal = FVector(0.f, 0.f,1.f);
	FPlane HorizontalPlane = FPlane(Origin, PlaneNormal);

	return FMath::RayPlaneIntersection(RayStart, Direction, HorizontalPlane);
	
}

FVector FSceneWizardToolUtilities::CursorTraceVerticalPlaneTranslate(FEditorViewportClient* ViewportClient,
	FViewport* Viewport, FVector Origin)
{
	int32 X = Viewport->GetMouseX();
	int32 Y = Viewport->GetMouseY();
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
																				ViewportClient->Viewport, 
																				ViewportClient->GetScene(),
																				ViewportClient->EngineShowFlags)
																				.SetRealtimeUpdate( true ));
	FSceneView* View = ViewportClient->CalcSceneView( &ViewFamily );
	FViewportCursorLocation Cursor(View, ViewportClient, X, Y);
	
	const auto ViewportType = ViewportClient->GetViewportType();
	
	FVector RayStart = Cursor.GetOrigin();
	FVector Direction = Cursor.GetDirection();
	if (ViewportType == LVT_OrthoXY || ViewportType == LVT_OrthoXZ || ViewportType == LVT_OrthoYZ ||
	ViewportType == LVT_OrthoNegativeXY || ViewportType == LVT_OrthoNegativeXZ || ViewportType == LVT_OrthoNegativeYZ)
	{
		RayStart -= Direction * HALF_WORLD_MAX/2;
	}
	FVector Up = FVector(0.f, 0.f,1.f);
	FVector Right = FVector::CrossProduct(Up, Direction);
	FVector PlaneNormal = FVector::CrossProduct(Up, Right);
	PlaneNormal.Normalize();
	FPlane VerticalPlane = FPlane(Origin, PlaneNormal);

	return FMath::RayPlaneIntersection(RayStart, Direction, VerticalPlane);
	
}

void FSceneWizardToolUtilities::GetSelectionBounds(FVector& Origin, FVector& BoxExtent, float& SphereRadius)
{
	bool bFirstItem = true;

	FBoxSphereBounds Extents;
	for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
	{
		if (AActor* Actor = Cast<AActor>(*It))
		{
			if (bFirstItem)
			{
				Extents = Actor->GetRootComponent()->Bounds;
			}
			else
			{
				Extents = Extents + Actor->GetRootComponent()->Bounds;
			}

			bFirstItem = false;
		}
	}

	Origin = Extents.Origin;
	BoxExtent = Extents.BoxExtent;
	SphereRadius = Extents.SphereRadius;
}
