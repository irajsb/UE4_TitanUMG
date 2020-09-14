// Copyright Epic Games, Inc. All Rights Reserved.
 
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "STitanPanner.h"
#include "Input/Reply.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class FPaintArgs;
class FSlateWindowElementList;
class UTitanJoystick;
/**
 * A virtual joystsick
 */
class TITANUMG_API STitanVirtualJoystick : public STitanPanner
{

public:


	SLATE_BEGIN_ARGS( STitanVirtualJoystick )
		{}
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs );

	/**
	 * Static function to return if external users should create/activate/etc a touch interface
	 * Note that this function is also used internally, so even if this returns false but an SVirtualJoystick
	 * is created, it won't actually show any controls
	 */
	static bool ShouldDisplayTouchInterface();

	/**
	 * Shows or hides the controls (for instance during cinematics
	 */
	void SetJoystickVisibility(const bool bVisible, const bool bFade);


	/**
	 * Sets parameters that control all controls
	 */

	virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
	
	virtual FVector2D ComputeDesiredSize(float) const override;
void SetWaitState()
{
	State =EVirtualJoystickState:: State_WaitForStart;
}
	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event) override;

	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;

	virtual bool SupportsKeyboardFocus() const override;

protected:
	/** Callback for handling display metrics changes. */
	virtual void HandleDisplayMetricsChanged(const FDisplayMetrics& NewDisplayMetric);

	void AlignBoxIntoScreen(FVector2D& Position, const FVector2D& Size, const FVector2D& ScreenSize);

	/**
	 * Process a touch event (on movement and possibly on initial touch)
	 *
	 * @return true if the touch was successful
	 */
	virtual bool HandleTouch(int32 ControlIndex, const FVector2D& LocalCoord, const FVector2D& ScreenSize);

	/** 
	 * Return the target opacity to lerp to given the current state
	 */
	FORCEINLINE FLinearColor GetBaseColor();
	void Reset();
	enum EVirtualJoystickState
	{
		State_Active,
        State_CountingDownToInactive,
        State_CountingDownToReset,
        State_Inactive,
        State_WaitForStart,
        State_CountingDownToStart,
    };


public:
	/** Global settings from the UTouchInterface */


	/** For recentered joysticks, this is the re-center location */
	FVector2D VisualCenter;

	/** The corrected actual center of the control */
	FVector2D CorrectedCenter;



	/** The corrected size of the thumb that can be re-centered within InteractionSize area */
	FVector2D CorrectedThumbSize;

	/** The corrected size of a the interactable area around Center */
	FVector2D CorrectedInteractionSize;

	/** The corrected scale for control input */
	FVector2D CorrectedInputScale;

	/** Which pointer index is interacting with this control right now, or -1 if not interacting */
	int8 CapturedPointerIndex;

	/** Time to activate joystick **/
	float ElapsedTime;

	/** Visual center to be updated */
	FVector2D NextCenter;



	/** Whether or not we need position the control against the geometry */
	bool bHasBeenPositioned;

	/** Whether or not to update center position */
	bool bNeedUpdatedCenter;


	/** The current state of all controls */
	EVirtualJoystickState State;

	/** True if the joystick should be visible */
	uint32 bVisible:1;

	
	
	
	/** Target opacity */
	FLinearColor CurrentColor;

	/* Countdown until next state change */
	float Countdown;

	/** Last used scaling value for  */
	float PreviousScalingFactor = 0.0f;
	public:
	UTitanJoystick* Owner;
};
