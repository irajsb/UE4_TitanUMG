// Fill out your copyright notice in the Description page of Project Settings.


#include "STitanWindow.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SSpacer.h"
#include "TitanWindow.h"
#include "Widgets/Images/SImage.h"





void STitanWindow::Construct(const FArguments& InArgs)
{


	const TSharedRef<SImage> QuitButtonImage=	SNew(SImage).Visibility(EVisibility::SelfHitTestInvisible);
	MyQuitButtonImage=QuitButtonImage;

	const TSharedRef<SBorder> ContentBorder=	SNew(SBorder);
	MyContentBorder=ContentBorder;

	const TSharedRef<SButton>Button=	SNew(SButton)[
		
				QuitButtonImage
				
				].VAlign(VAlign_Center).HAlign(HAlign_Center).IsFocusable(false);
	MyQuitButton=Button;
	Button->SetOnClicked(FOnClicked::CreateLambda([this]()
	{
		if(this->TitanWindow)
		{
			this->TitanWindow->RemoveFromParent();
		}
		return FReply::Handled();
	}));

	TSharedRef<STextBlock>TextBlock=	SNew(STextBlock).Justification(ETextJustify::Center).Visibility(EVisibility::HitTestInvisible);
	MyTextBlock=TextBlock;

	
	const TSharedRef<SHorizontalBox> HorizontalBoxTopButtons=SNew(SHorizontalBox)
	
	+SHorizontalBox::Slot().VAlign(VAlign_Fill).HAlign(HAlign_Fill).FillWidth(0.9)[
			
	TextBlock

		]
	+SHorizontalBox::Slot().VAlign(VAlign_Fill).HAlign(HAlign_Fill).AutoWidth()[
			
	Button
		];

	const TSharedRef<SBorder> TopBorder=SNew(SBorder).BorderBackgroundColor(FLinearColor(1,1,1,1))[HorizontalBoxTopButtons
		];
	MyTopBorder= TopBorder;


	const TSharedRef<SVerticalBox> VerticalBox =SNew(SVerticalBox)
		+SVerticalBox::Slot()[
			TopBorder].VAlign(VAlign_Fill).HAlign(HAlign_Fill).AutoHeight()
		//Content Border
		+SVerticalBox::Slot().VAlign(VAlign_Fill).HAlign(HAlign_Fill).FillHeight(1.0)[
		ContentBorder

		];
	

	const TSharedRef<SBorder> Border=	SNew(SBorder)
		.BorderBackgroundColor(FLinearColor::White)
		.ColorAndOpacity(FLinearColor(1,1,1,1))[VerticalBox].HAlign(HAlign_Fill).VAlign(VAlign_Fill).Visibility(EVisibility::Visible);
	//
	MyBorder = Border;

	ChildSlot[
		Border
		];

	MyBorder->SetOnMouseEnter(FNoReplyPointerEventHandler::CreateSP(this,&STitanWindow::HoveringOuterBorder,true));
	MyBorder->SetOnMouseLeave(FSimpleNoReplyPointerEventHandler::CreateSP(this,&STitanWindow::HoveringOuterBorder,false));
	MyBorder->SetOnMouseButtonDown(FPointerEventHandler::CreateSP(this,&STitanWindow::OuterBorderMouseDown));
	MyBorder->SetOnMouseMove(FPointerEventHandler::CreateSP(this,&STitanWindow::OuterBorderMouseMove));
	MyBorder->SetOnMouseButtonUp(FPointerEventHandler::CreateSP(this,&STitanWindow::OuterBorderMouseUp));

	
	VerticalBox->SetOnMouseEnter(FNoReplyPointerEventHandler::CreateSP(this,&STitanWindow::HoveringInnerContent,true));
	VerticalBox->SetOnMouseLeave(FSimpleNoReplyPointerEventHandler::CreateSP(this,&STitanWindow::HoveringInnerContent,false));


	TopBorder->SetOnMouseEnter(FNoReplyPointerEventHandler::CreateSP(this,&STitanWindow::HoverTopBorder));
	TopBorder->SetOnMouseLeave(FSimpleNoReplyPointerEventHandler::CreateSP(this,&STitanWindow::HoverTopBorderEnd));
	TopBorder->SetOnMouseButtonDown(FPointerEventHandler::CreateSP(this,&STitanWindow::TopBorderMouseDown));
	TopBorder->SetOnMouseMove(FPointerEventHandler::CreateSP(this,&STitanWindow::TopBorderMouseMove));
	TopBorder->SetOnMouseButtonUp(FPointerEventHandler::CreateSP(this,&STitanWindow::TopBorderMouseUp));
	
}

void STitanWindow::CalculateTransformDirection(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{

	if(bInnerHovered)
	return;
	
	FVector2D RelativePosition=InEvent.GetScreenSpacePosition()-InGeometry.GetAbsolutePosition();
	FVector2D Size=InGeometry.GetAbsoluteSize();
	FVector2D PositionRelativeToCenter=RelativePosition-Size/2;
	const FVector2D FVectorTopLeft=FVector2D(0,0);
	const FVector2D FVectorTopRight=FVector2D(Size.X,0);
	const FVector2D FVectorBottomLeft=FVector2D(0,Size.Y);
	const FVector2D FVectorBottomRight=FVector2D(Size.X,Size.Y);

	if((RelativePosition-FVectorTopLeft).IsNearlyZero(CornerDetectionTolerance))
	{
		TransformDirection=ETitanTransformDirection::TopLeft;
	}else if ((RelativePosition-FVectorTopRight).IsNearlyZero(CornerDetectionTolerance))
	{
		TransformDirection=ETitanTransformDirection::TopRight;
	}else if ((RelativePosition-FVectorBottomLeft).IsNearlyZero(CornerDetectionTolerance))
	{
		TransformDirection=ETitanTransformDirection::BottomLeft;
	}else if ((RelativePosition-FVectorBottomRight).IsNearlyZero(CornerDetectionTolerance))
	{
		TransformDirection=ETitanTransformDirection::BottomRight;
	}else if(FMath::Abs(PositionRelativeToCenter.X)>FMath::Abs(PositionRelativeToCenter.Y))
	{
		if(PositionRelativeToCenter.X<0)
		{
			TransformDirection=ETitanTransformDirection::CenterLeft;
		}else
		{
			TransformDirection=ETitanTransformDirection::CenterRight;
		}
	}else
	{

		if(PositionRelativeToCenter.Y<0)
		{
			TransformDirection=ETitanTransformDirection::TopCenter;
		}else
		{
			TransformDirection=ETitanTransformDirection::BottomCenter;
		}
	}
	
	
	

	
}

FVector2D STitanWindow::ComputeDragDirection(ETitanTransformDirection::Type InTransformDirection) const
{
	switch ( InTransformDirection )
	{
	case ETitanTransformDirection::TopLeft:
		return FVector2D(-1, -1);
	case ETitanTransformDirection::TopCenter:
		return FVector2D(0, -1);
	case ETitanTransformDirection::TopRight:
		return FVector2D(1, -1);

	case ETitanTransformDirection::CenterLeft:
		return FVector2D(-1, 0);
	case ETitanTransformDirection::CenterRight:
		return FVector2D(1, 0);

	case ETitanTransformDirection::BottomLeft:
		return FVector2D(-1, 1);
	case ETitanTransformDirection::BottomCenter:
		return FVector2D(0, 1);
	case ETitanTransformDirection::BottomRight:
		return FVector2D(1, 1);
	default:
		return FVector2D(0, 0);;
	}


}

void STitanWindow::SetBorderBackgroundColor(FSlateColor InColor,FSlateColor InColorWhenHovered) 
{
	this->OuterBorderColor=InColor;
	this->OuterBorderColorWhenHovered=InColorWhenHovered;

	SetOuterBorderHoveredCosmetic(IsCapturingMouseForResize);
	
	
	MyBorder->SetColorAndOpacity(FLinearColor::White);
}

void STitanWindow::SetOuterBorderHoveredCosmetic(bool InBool)
{
	MyBorder->SetBorderBackgroundColor(InBool?OuterBorderColorWhenHovered:OuterBorderColor);
}

void STitanWindow::SetBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage) const
{
	MyBorder->SetBorderImage(InBorderImage);
}

void STitanWindow::SetBorderPadding(FMargin Margin) const
{
	
MyBorder->SetPadding(Margin);
	
}

void STitanWindow::SetTopBorderBackgroundColor(FSlateColor InColor,FSlateColor InHoverColor) 
{
	TopColor=InColor;
	TopHoverColor=InHoverColor;
	SetTopHoveredCosmeticOnly(IsTopBorderHovered);
	
}

void STitanWindow::SetTopHoveredCosmeticOnly(bool bInput)
{
	MyTopBorder->SetBorderBackgroundColor(bInput?TopHoverColor:TopColor);
	MyTopBorder->SetColorAndOpacity(FLinearColor::White);
}

void STitanWindow::SetTopBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage) const
{
	MyTopBorder->SetBorderImage(InBorderImage);
}

void STitanWindow::SetQuitBorderImage(const TAttribute<const FSlateBrush*>& InBorderImage) const
{
	MyQuitButtonImage->SetImage(InBorderImage);
}

void STitanWindow::SetButtonStyle(FButtonStyle* ButtonStyle) const
{
	MyQuitButton->SetButtonStyle(ButtonStyle);
}

void STitanWindow::SetContent(TSharedRef<SWidget> Widget) const
{
	
	MyContentBorder->SetContent(Widget);
}

void STitanWindow::HoveringOuterBorder(const FGeometry& InGeometry, const FPointerEvent& InEvent, const bool Hovering)
{
	bOuterHovered=true;
}

void STitanWindow::HoveringOuterBorder(const FPointerEvent& InEvent, const bool bInHoveringThumbnail)
{
	bOuterHovered=false;
	if(!IsCapturingMouseForResize)
	{
		SetOuterBorderHoveredCosmetic(false);
	}
}

void STitanWindow::HoveringInnerContent(const FGeometry& InGeometry, const FPointerEvent& InEvent, const bool Hovering)
{
	bInnerHovered=true;
}

void STitanWindow::HoveringInnerContent(const FPointerEvent& InEvent, const bool bInHoveringThumbnail)
{
	bInnerHovered=false;
}

void STitanWindow::HoverTopBorder(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{
	if(!IsCapturingMouseForResize)
	{
		IsTopBorderHovered=true;
		SetTopHoveredCosmeticOnly(IsTopBorderHovered);
	}
}

void STitanWindow::HoverTopBorderEnd(const FPointerEvent& InEvent)
{
	IsTopBorderHovered=false;
	SetTopHoveredCosmeticOnly(IsTopBorderHovered);
}

FReply STitanWindow::TopBorderMouseDown(const FGeometry& InGeometry, const FPointerEvent& InEvent) 
{
	if(IsTopBorderHovered)
	{
		IsMoving=true;
		return  FReply::Handled().CaptureMouse(MyTopBorder.ToSharedRef());
	}
	return FReply::Unhandled();
}

FReply STitanWindow::TopBorderMouseUp(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{
	if(IsMoving)
	{
		IsMoving=false;
		return  FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

FReply STitanWindow::TopBorderMouseMove(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{
	if(IsMoving)
	{
		if(TitanWindow)
		{
			TitanWindow->ProcessResize(	FVector2D(0),InEvent.GetCursorDelta());	
		}

	return 	FReply::Handled();
	}
	return FReply::Unhandled();
}


FReply STitanWindow::OuterBorderMouseDown(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{

	if(InEvent.GetEffectingButton()!=EKeys::LeftMouseButton)
		return FReply::Unhandled();

	UE_LOG(LogTemp,Error,TEXT("Begin Resize"));
	if(bOuterHovered&&!bInnerHovered)
	{
		SetIsCaputringMouseForResize(true);
		return  FReply::Handled().CaptureMouse(MyBorder.ToSharedRef());
		
	}
	return FReply::Unhandled();
}

FReply STitanWindow::OuterBorderMouseUp(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{
	if(InEvent.GetEffectingButton()!=EKeys::LeftMouseButton)
		return FReply::Unhandled();
	
	UE_LOG(LogTemp,Error,TEXT("End Resize"));
	if(IsCapturingMouseForResize)
	{
		SetIsCaputringMouseForResize(false);
		return FReply::Handled().ReleaseMouseCapture();
	}
	
	return FReply::Unhandled();
}

FReply STitanWindow::OuterBorderMouseMove(const FGeometry& InGeometry, const FPointerEvent& InEvent)
{

if(!IsCapturingMouseForResize)
{
	CalculateTransformDirection(InGeometry,InEvent);
	if(!bInnerHovered&&bOuterHovered)
	{
		SetOuterBorderHoveredCosmetic(true);
	}else
	{
		SetOuterBorderHoveredCosmetic(false);
	}
}

const FVector2D DragDirection=	ComputeDragDirection(TransformDirection);
 FVector2D PositionMultiplier=FVector2D(0);
	if(DragDirection.X<0)
	{
		PositionMultiplier.X=DragDirection.X*-1;
	}
	if(DragDirection.Y<0)
	{
		PositionMultiplier.Y=DragDirection.Y*-1;
	}
	
	if(TitanWindow&&IsCapturingMouseForResize)
	{
		TitanWindow->ProcessResize(	InEvent.GetCursorDelta()*DragDirection,InEvent.GetCursorDelta()*PositionMultiplier);	
	}
	if(IsCapturingMouseForResize)
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FCursorReply STitanWindow::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if(bOuterHovered&&!bInnerHovered)
	{
		switch ( TransformDirection )
		{
		case ETitanTransformDirection::TopLeft:
		case ETitanTransformDirection::BottomRight:
			return FCursorReply::Cursor(EMouseCursor::ResizeSouthEast);
		case ETitanTransformDirection::TopRight:
		case ETitanTransformDirection::BottomLeft:
			return FCursorReply::Cursor(EMouseCursor::ResizeSouthWest);
		case ETitanTransformDirection::TopCenter:
		case ETitanTransformDirection::BottomCenter:
			return FCursorReply::Cursor(EMouseCursor::ResizeUpDown);
		case ETitanTransformDirection::CenterLeft:
		case ETitanTransformDirection::CenterRight:
			return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
		default: return FCursorReply::Cursor(EMouseCursor::Default); ;
		}
	}else
	{
		if(IsTopBorderHovered)
		{
			return FCursorReply::Cursor(EMouseCursor::GrabHand);
		}
		return FCursorReply::Cursor(EMouseCursor::Default);
		
	}

}


void STitanWindow::SetIsCaputringMouseForResize(bool InBool)
{
	IsCapturingMouseForResize=InBool;
	
}

bool STitanWindow::SupportsKeyboardFocus() const
{
	return false;
}

FVector2D STitanWindow::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return  FVector2D(500,600);
}
