#pragma once

#include "CoreMinimal.h"
#include "EditorModeTools.h"
#include "SceneWizardModeTool.h"



enum ETransformActorToolStatus
{
	ToSelection,
	ToMove,
	ToSnapMove,
	ToAlign
};

class FTransformActorTool: public FSceneWizardModeTool
{
public:
	FTransformActorTool(FSceneWizardEdMode* EdMode);
	~FTransformActorTool();
	
	// FModeTool Interface
	/** Returns the name that gets reported to the editor. */
	virtual FString GetName() const	override	{ return TEXT("TransformActorTool"); }

	// user input

	virtual bool MouseEnter( FEditorViewportClient* ViewportClient,FViewport* Viewport,int32 x, int32 y ) { return false; }

	virtual bool MouseLeave( FEditorViewportClient* ViewportClient,FViewport* Viewport ) { return false; }

	virtual bool MouseMove(FEditorViewportClient* ViewportClient,FViewport* Viewport,int32 x, int32 y) override;
	
	// virtual bool ReceivedFocus(FEditorViewportClient* ViewportClient,FViewport* Viewport) { return false; }

	// virtual bool LostFocus(FEditorViewportClient* ViewportClient,FViewport* Viewport) { return false; }
	
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;

	
	/** FSceneWizardModeTool Interface */
	
	virtual bool IsToolActive() override;
	
	virtual void SetToolActive(const bool bInActive) override;

	/** Transform Actor Tool Function */
	ETransformActorToolStatus GetToolStatus(){ return CurrentStatus; }
	bool SetToolStatus(ETransformActorToolStatus NewStatus);

	//@{
	// virtual bool StartModify()	{ return 0; }
	// virtual bool EndModify()	{ return 0; }
	//@}

	//@{
	virtual void StartTrans() override;
	virtual void EndTrans()	override;
	
private:

	
	// get the world transform from mouse cursor; 
	FTransform GetNewTransformFromCursor(FEditorViewportClient* ViewportClient, FViewport* Viewport, const TArray<AActor*>* IgnoreActors);
	FTransform GetNewTransformFromCursor(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 X, int32 Y, const TArray<AActor*>* IgnoreActors);

	// get the world transform from mouse cursor; 
	FTransform GetNewTransformFromCursorHorizontal(FEditorViewportClient* ViewportClient, FViewport* Viewport, const TArray<AActor*>* IgnoreActors);
	
	void UpdateActorsTransformStart();
	
	void UpdateActorsTransformOffset(FEditorViewportClient* ViewportClient, FViewport* Viewport, const TArray<AActor*>* IgnoreActors);

	void UpdateHorizontalPlaneOrigin();

	void UpdateVerticalPlaneOrigin();

	/** TO MOVE Sub Tool Function **/
	// Move Actor Follow Mouse
	void MoveActorsFollowMouse(const FTransform NewTransform, const TArray<AActor*>* Actors, const bool Horizontal);
	
private:
	bool bIsToolActive;

	bool bIsShiftPressed;
	
	// Transform Actor Tool Status
	ETransformActorToolStatus CurrentStatus;
	
	// FTransform ActorNewTransform;
	FTransform CursorTransformStart;
	FTransform CursorTransformEnd;
	
	TArray<FTransform> CursorOffsetToActor;
	TArray<FTransform> ActorsTransformStart;

	TArray<AActor*> SelectedActors;

	FVector HorizontalPlaneOrigin;
	FVector VerticalPlaneOrigin;

	
};
