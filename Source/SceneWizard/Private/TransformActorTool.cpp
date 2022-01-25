#include "TransformActorTool.h"
#include "SceneWizard.h"
#include "LevelEditorActions.h"
#include "SceneWizardEdMode.h"
#include "SceneWizardEdModeToolkit.h"
#include "Engine/Selection.h"
#include "Editor/UnrealEd/Private/Editor/ActorPositioning.h"
#include "SceneWizardToolUtilities.h"


#define LOCTEXT_NAMESPACE "FTransformActorTool"


FTransformActorTool::FTransformActorTool(FSceneWizardEdMode* EdMode): FSceneWizardModeTool(EdMode)
{
	// initialization
	// ActorNewTransform = FTransform::Identity;
	bIsShiftPressed = false;
	bIsToolActive = false;

	// set the initialize status to selection
	CurrentStatus = ETransformActorToolStatus::ToSelection;
	
}

FTransformActorTool::~FTransformActorTool()
{
	
}


bool FTransformActorTool::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	
	if(bIsToolActive && CurrentStatus == ToMove && GEditor->GetSelectedActors()->Num()>0)
	{
		// ToMove Status
		
		GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);
		
		if(ViewportClient->IsShiftPressed()) // Shift Pressed Mode
		{
			FVector CursorVerticalPosition = FSceneWizardToolUtilities::CursorTraceVerticalPlaneTranslate(ViewportClient, Viewport, VerticalPlaneOrigin);
			CursorTransformEnd.SetLocation(CursorVerticalPosition);
			MoveActorsFollowMouse(CursorTransformEnd, &SelectedActors, false);
		}else                              // Shift Released Mode
		{
			FVector CursorHorizontalPosition = FSceneWizardToolUtilities::CursorTraceHorizontalPlaneTranslate(ViewportClient, Viewport, HorizontalPlaneOrigin);
			CursorTransformEnd.SetLocation(CursorHorizontalPosition);
			MoveActorsFollowMouse(CursorTransformEnd, &SelectedActors, true);
		}
		
		return true;
	}
	
	return false;
}

bool FTransformActorTool::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,
	EInputEvent Event)
{
	if(!bIsToolActive)
	{
		return bIsToolActive;
	}
	
	if(CurrentStatus == ToSelection)
	{
		if(GEditor->GetSelectedActors()->Num()==0)
		{
			UE_LOG(LogSceneWizard, Warning, TEXT("No Actor Selected !"));
			return true;
		}
		GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);
	
		// TODO : Move Actor Mode
		CurrentStatus = ToMove;
	
		// Initialize the tool
		UpdateActorsTransformOffset(ViewportClient, Viewport, &SelectedActors);
		UpdateHorizontalPlaneOrigin();
	
		// LOG
		static_cast<FSceneWizardEdModeToolkit*>(SWEdMode->GetToolkit().Get())->UpdateWizardWidget(
													FString(TEXT("Transform Actor Tool -> Move Actor Mode.")));
		UE_LOG(LogSceneWizard, Display, TEXT("Transform Actor Tool -> Move Actor Mode."));


		// Shift Pressed Mode!
		if((Key == EKeys::LeftShift || Key == EKeys::RightShift) && Event == EInputEvent::IE_Pressed && !Viewport->HasMouseCapture())
		{
			if(CurrentStatus == ToMove && !bIsShiftPressed)
			{
				UpdateVerticalPlaneOrigin();
				bIsShiftPressed = true;
			}
		}
		// Shift Released Mode!
		if((Key == EKeys::LeftShift || Key == EKeys::RightShift) && Event == EInputEvent::IE_Released && !Viewport->HasMouseCapture())
		{
			if(CurrentStatus == ToMove && bIsShiftPressed)
			{
				UpdateHorizontalPlaneOrigin();
				bIsShiftPressed = false;
			}
		}

		return true;
		
	}else{
		// Exit Sub Tool
		if(Key == EKeys::LeftMouseButton && Event == EInputEvent::IE_Pressed && !Viewport->HasMouseCapture())
		{
			CurrentStatus = ToSelection;
			return true;
		}


	}
	
	return false;
}

bool FTransformActorTool::IsToolActive()
{
	return bIsToolActive;
}

void FTransformActorTool::SetToolActive(const bool bInActive)
{
	CurrentStatus = ToSelection;
	bIsToolActive = bInActive;
}


bool FTransformActorTool::SetToolStatus(ETransformActorToolStatus NewStatus)
{
	CurrentStatus = NewStatus;
	return true;
}



void FTransformActorTool::StartTrans()
{
	GEditor->BeginTransaction(LOCTEXT("FTransformActorTool","Scene Wizard >> Transform Actor Tool >> Move Actors"));
	FSceneWizardModeTool::StartTrans();
}

void FTransformActorTool::EndTrans()
{
	GEditor->EndTransaction();
	FSceneWizardModeTool::EndTrans();
}


FTransform FTransformActorTool::GetNewTransformFromCursor(FEditorViewportClient* ViewportClient, FViewport* Viewport, const TArray<AActor*>* IgnoreActors)
{
	const int32 x = Viewport->GetMouseX();
	const int32 y = Viewport->GetMouseY();
	
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
	ViewportClient->Viewport, 
	ViewportClient->GetScene(),
	ViewportClient->EngineShowFlags)
	.SetRealtimeUpdate( true ));
	FSceneView* View = ViewportClient->CalcSceneView( &ViewFamily );
	FViewportCursorLocation Cursor(View, ViewportClient, x, y);
	
	FActorPositionTraceResult TraceResult = FActorPositioning::TraceWorldForPositionWithDefault(Cursor, *View, IgnoreActors);

	if( TraceResult.HitSuccess == FActorPositionTraceResult::HitSuccess)
	{
		FTransform NewTransform = FTransform::Identity;
		NewTransform.SetLocation(TraceResult.Location);
		
		FVector RotationUpVector = TraceResult.SurfaceNormal;
		FRotator CursorWorldRotation = FRotationMatrix::MakeFromZX(RotationUpVector, FVector::ForwardVector).Rotator();
		
		NewTransform.SetRotation(CursorWorldRotation.Quaternion());

		return NewTransform;
	}
	
	return FTransform::Identity;
}

FTransform FTransformActorTool::GetNewTransformFromCursor(FEditorViewportClient* ViewportClient, FViewport* Viewport,
	int32 X, int32 Y, const TArray<AActor*>* IgnoreActors)
{
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
																				ViewportClient->Viewport, 
																				ViewportClient->GetScene(),
																				ViewportClient->EngineShowFlags)
																				.SetRealtimeUpdate( true ));
	FSceneView* View = ViewportClient->CalcSceneView( &ViewFamily );
	FViewportCursorLocation Cursor(View, ViewportClient, X, Y);
	
	FActorPositionTraceResult TraceResult = FActorPositioning::TraceWorldForPositionWithDefault(Cursor, *View, IgnoreActors);

	if( TraceResult.HitSuccess == FActorPositionTraceResult::HitSuccess)
	{
		FTransform NewTransform = FTransform::Identity;
		NewTransform.SetLocation(TraceResult.Location);
		
		FVector RotationUpVector = TraceResult.SurfaceNormal;
		FRotator CursorWorldRotation = FRotationMatrix::MakeFromZX(RotationUpVector, FVector::ForwardVector).Rotator();
		
		NewTransform.SetRotation(CursorWorldRotation.Quaternion());

		return NewTransform;
	}
	
	return FTransform::Identity;
}

FTransform FTransformActorTool::GetNewTransformFromCursorHorizontal(FEditorViewportClient* ViewportClient,
                                                                    FViewport* Viewport, const TArray<AActor*>* IgnoreActors)
{
	const int32 X = Viewport->GetMouseX();
	const FIntPoint ViewSize = Viewport->GetSizeXY();
	const int32 Y = ViewSize.Y/2;
	return GetNewTransformFromCursor(ViewportClient, Viewport, X, Y, IgnoreActors);
}

void FTransformActorTool::UpdateActorsTransformStart()
{
	SelectedActors.Empty();
	GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);
	
	ActorsTransformStart.Empty();
	for(int32 i = 0; i < SelectedActors.Num(); i++)
	{
		ActorsTransformStart.Add(SelectedActors[i]->GetActorTransform());
	}
}
void FTransformActorTool::UpdateActorsTransformOffset(FEditorViewportClient* ViewportClient, FViewport* Viewport, const TArray<AActor*>* IgnoreActors)
{
	CursorTransformStart = GetNewTransformFromCursor(ViewportClient, Viewport, IgnoreActors);

	UpdateActorsTransformStart();
	
	UpdateHorizontalPlaneOrigin();
	
	if(ActorsTransformStart.Num()>0 && IsToolActive())
	{
		CursorOffsetToActor.Empty();

		for(int32 i = 0; i < ActorsTransformStart.Num(); i++)
		{
			// FVector ActorTranslate = FTransform::SubtractTranslations(ActorsTransformStart[i], CursorTransformStart);
			FVector ActorTranslate = ActorsTransformStart[i].GetLocation() - HorizontalPlaneOrigin;
			FTransform OffsetTransform = FTransform(FQuat::Identity, ActorTranslate, FVector(0.0f));
			CursorOffsetToActor.Add(OffsetTransform);
		}
	}
}

void FTransformActorTool::UpdateHorizontalPlaneOrigin()
{
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	FSceneWizardToolUtilities::GetSelectionBounds(Origin, BoxExtent, SphereRadius);

	// Keep the bottom position
	// Origin.Z -= BoxExtent.Z;
	
	HorizontalPlaneOrigin = Origin;
}

void FTransformActorTool::UpdateVerticalPlaneOrigin()
{
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	FSceneWizardToolUtilities::GetSelectionBounds(Origin, BoxExtent, SphereRadius);

	VerticalPlaneOrigin = Origin;
}

void FTransformActorTool::MoveActorsFollowMouse(const FTransform NewTransform, const TArray<AActor*>* Actors, const bool Horizontal)
{
	for(int32 i = 0; i < Actors->Num(); i++)
	{
		FTransform TmpTransform = FTransform::Identity;
		// FVector NewTranlate = NewTransform.GetLocation();
		// if(Horizontal)
		// {
		// 	// TmpTransform.SetLocation(FVector(NewTranlate.X, NewTranlate.Y, 0.f));
		// }
		// else
		// {
		// 	// TmpTransform.SetLocation(FVector(0.f, 0.f, NewTranlate.Z));
		// }
		TmpTransform.SetLocation(NewTransform.GetLocation());
		FTransform ActorCurrentTransformEnd = TmpTransform + CursorOffsetToActor[i];
		SelectedActors[i]->SetActorLocation(ActorCurrentTransformEnd.GetLocation());
		
	}
} 
#undef LOCTEXT_NAMESPACE