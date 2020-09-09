// Copyright Epic Games, Inc. All Rights Reserved.
 
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Input/Reply.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class FPaintArgs;
class FSlateWindowElementList;
class UTitanPanner;
/**
 * A virtual joystsick
 */
class TITANUMG_API STitanPanner : public SLeafWidget
{

public:


	SLATE_BEGIN_ARGS( STitanPanner )
		{}
	SLATE_END_ARGS()


	void Construct( const FArguments& InArgs );
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;

	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;


	/** The brush to use to draw the background for joysticks, or unclicked for buttons */
	TSharedPtr< ISlateBrushSource > Image1;

uint8 NumofTouches;
	
bool HandleEvent;

virtual bool SupportsKeyboardFocus() const override;
FVector2D Result;

	//X=-1000 is considered null
FVector2D Index1Location;
FVector2D Index2Location;
float LastPinchSize;
float LastRotation;
float  PinchResult;
	float LastAngle;
	float AngleResult;


	/** The corrected size of a joystick that can be re-centered within InteractionSize area */
	FVector2D CorrectedVisualSize;
UTitanPanner * Owner;
public:

	void SimulateTouch(FVector2D in);
};
