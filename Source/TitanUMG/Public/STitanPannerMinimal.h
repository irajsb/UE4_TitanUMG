// Copyright Epic Games, Inc. All Rights Reserved.
 
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "TitanPannerMinimal.h"
#include "Input/Reply.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class FPaintArgs;
class FSlateWindowElementList;
class UTitanPannerMinimal;
/**
* A virtual joystsick
*/
class TITANUMG_API STitanPannerMinimal : public SLeafWidget
{

    public:


    SLATE_BEGIN_ARGS( STitanPannerMinimal )
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



    /** The corrected size of a joystick that can be re-centered within InteractionSize area */
    FVector2D CorrectedVisualSize;
    UTitanPannerMinimal * Owner;
    public:

    void SimulateTouch(FVector2D in);
};
