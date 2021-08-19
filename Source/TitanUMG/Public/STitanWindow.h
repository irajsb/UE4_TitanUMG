// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Widget.h"
#include "UObject/NoExportTypes.h"


/**
 * 
 */
namespace ETitanTransformDirection
{
	enum Type
	{
		TopLeft = 0,
		TopCenter,
		TopRight,

		CenterLeft,
		CenterRight,

		BottomLeft,
		BottomCenter,
		BottomRight,

		MAX
	};
}
class UTitanWindow;
class FPaintArgs;
class FSlateWindowElementList;
class TITANUMG_API STitanWindow : public SCompoundWidget
{	


	SLATE_BEGIN_ARGS( STitanWindow )
	{}
	SLATE_END_ARGS()
	
	void Construct( const FArguments& InArgs );

	ETitanTransformDirection::Type	TransformDirection ;
	void CalculateTransformDirection(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	float CornerDetectionTolerance=10;
	FVector2D ComputeDragDirection(ETitanTransformDirection::Type InTransformDirection) const;
	/** Sets the color and opacity of the background image of this border. */
	void SetBorderBackgroundColor(FSlateColor InColor,FSlateColor InColorWhenHovered) ;
	void SetOuterBorderHoveredCosmetic(bool InBool);
	FSlateColor OuterBorderColor;
	FSlateColor OuterBorderColorWhenHovered;
	/** See BorderImage attribute */
	void SetBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage) const;

	void SetBorderPadding(FMargin Margin) const;

	/** Sets the color and opacity of the background image of this border. */
	void SetTopBorderBackgroundColor(FSlateColor InColor,FSlateColor InHoverColor) ;
	FSlateColor TopColor;
	FSlateColor TopHoverColor;
	void SetTopHoveredCosmeticOnly(bool bInput);
	/** See BorderImage attribute */
	void SetTopBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage) const;

	void SetQuitBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage) const;

	void SetButtonStyle(FButtonStyle* ButtonStyle) const;
	void SetContent(TSharedRef<SWidget> Widget) const;

	void HoveringOuterBorder(const FGeometry& InGeometry, const FPointerEvent& InEvent, const bool Hovering);
	void HoveringOuterBorder(const FPointerEvent& InEvent, const bool bInHoveringThumbnail);
	void HoveringInnerContent(const FGeometry& InGeometry, const FPointerEvent& InEvent, const bool Hovering);
	void HoveringInnerContent(const FPointerEvent& InEvent, const bool bInHoveringThumbnail);
	void HoverTopBorder(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	void HoverTopBorderEnd(const FPointerEvent& InEvent);


	FReply TopBorderMouseDown(const FGeometry& InGeometry, const FPointerEvent& InEvent) ;
	FReply TopBorderMouseUp(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	FReply TopBorderMouseMove(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	
	//
	bool IsTopBorderHovered;
	bool IsMoving;
	//
	FReply OuterBorderMouseDown(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	FReply OuterBorderMouseUp(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	FReply OuterBorderMouseMove(const FGeometry& InGeometry, const FPointerEvent& InEvent);
	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
	bool bOuterHovered;
	bool bInnerHovered;
	void SetIsCaputringMouseForResize(bool InBool);
	bool IsCapturingMouseForResize;
	

	virtual bool SupportsKeyboardFocus() const override;
	public:
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

	TSharedPtr<SBorder> MyBorder;
	TSharedPtr<SBorder> MyTopBorder;
	TSharedPtr<SBorder> MyContentBorder;
	TSharedPtr<SImage> MyQuitButtonImage;
	TSharedPtr<SButton>MyQuitButton;
	TSharedPtr<STextBlock>MyTextBlock;
	UTitanWindow* TitanWindow;
	
};
