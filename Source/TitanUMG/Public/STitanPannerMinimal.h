// Copyright Epic Games, Inc. All Rights Reserved.
 
#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class FPaintArgs;
class FSlateWindowElementList;
class UTitanPannerMinimal;
/**
* Light weight Panner
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
    virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;


  
    bool DoubleClicked=false;
    bool CurrentClickPressed;
    bool CurrentDoubleClickPressed;
    uint8 NumofTouches;
    bool HandleEvent;
    FVector2D Result;
    FVector2D Index1Location;
    FVector2D CorrectedVisualSize;
    
    virtual bool SupportsKeyboardFocus() const override;
   
    //X=-1000 is considered null
    



    /** The corrected size of a joystick that can be re-centered within InteractionSize area */
 
    UTitanPannerMinimal * Owner;
    public:

    void SimulateTouch(FVector2D in);
};
