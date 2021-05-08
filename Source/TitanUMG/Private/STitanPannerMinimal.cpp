// Copyright Epic Games, Inc. All Rights Reserved.
#include "STitanPannerMinimal.h"
#include "TitanPannerMinimal.h"




static FORCEINLINE float TMGetScaleFactor(const FGeometry& Geometry)
{
	const float DesiredWidth = 1024.0f;

	float UndoDPIScaling = 1.0f / Geometry.Scale;
	return (Geometry.GetDrawSize().GetMax() / DesiredWidth) * UndoDPIScaling;
}


static int32 TIResolveRelativePosition(float Position, float RelativeTo, float ScaleFactor)
{
	// absolute from edge
	if (Position < -1.0f)
	{
		return RelativeTo + Position * ScaleFactor;
	}
	// relative from edge
	else if (Position < 0.0f)
	{
		return RelativeTo + Position * RelativeTo;
	}
	// relative from 0
	else if (Position <= 1.0f)
	{
		return Position * RelativeTo;
	}
	// absolute from 0
	else
	{
		return Position * ScaleFactor;
	}

}






bool STitanPannerMinimal::SupportsKeyboardFocus() const
{
	return false;
}

void STitanPannerMinimal::SimulateTouch(FVector2D in)
{
	NumofTouches++;

Index1Location=in;

	
}


void STitanPannerMinimal::Construct(const FArguments& InArgs)
{

}

FReply STitanPannerMinimal::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	
	
	NumofTouches++;
	if(NumofTouches==1)
	{
		

	Index1Location=MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );
	
	}

	return FReply::Handled().CaptureMouse(SharedThis(this));

}

FReply STitanPannerMinimal::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
const FVector2D TouchLocation=	MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition());

	
		if(NumofTouches==1)
		{
			Result=	Index1Location-TouchLocation ;
			HandleEvent=true;
			Index1Location= TouchLocation;
		}
		
		
	
 if(NumofTouches==2)
	{
 	
	}

	
	return FReply::Handled();
	
}

FReply STitanPannerMinimal::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event)
{if(NumofTouches==2)
{
	
}


	
	if(NumofTouches>1)
	{
		Result=FVector2D::ZeroVector;
	}
	NumofTouches--;



	DoubleClicked=false;
		
	
	return FReply::Handled().ReleaseMouseCapture();
	
}

FReply STitanPannerMinimal::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnTouchMoved(MyGeometry,MouseEvent);
	return FReply::Unhandled(); 
}

FReply STitanPannerMinimal::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{

	if(MouseEvent.GetEffectingButton().IsValid()&&MouseEvent.GetEffectingButton()==FKey("LeftMouseButton"))
	{
	return  	OnTouchStarted(MyGeometry,MouseEvent);
	}
	return  FReply::Unhandled();
}

FReply STitanPannerMinimal::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{

	
	if(InMouseEvent.GetEffectingButton().IsValid()&&InMouseEvent.GetEffectingButton()==FKey("LeftMouseButton"))
	{
		DoubleClicked=true;
		return 	OnTouchStarted(InMyGeometry,InMouseEvent);
	}
	return  FReply::Unhandled();
}

FReply STitanPannerMinimal::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(MouseEvent.GetEffectingButton().IsValid()&&MouseEvent.GetEffectingButton()==FKey("LeftMouseButton"))
	{
		
		return  OnTouchEnded(MyGeometry,MouseEvent);
	}
	
	return  FReply::Unhandled();
}

void STitanPannerMinimal::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	
	
if(Owner->PressInputKey.IsValid())
{
	if(NumofTouches!=0)
	{
		const FGamepadKeyNames::Type Press =DoubleClicked?Owner->DoubleClickInputKey.GetFName(): Owner->PressInputKey.GetFName() ;
		if(DoubleClicked?!CurrentDoubleClickPressed:!CurrentClickPressed)
		{
			FSlateApplication::Get().OnControllerButtonPressed(Press,0,false);
			CurrentClickPressed=!DoubleClicked;
			CurrentDoubleClickPressed=DoubleClicked;
		}
		
		
	}else
	{
		
		const FGamepadKeyNames::Type Press = Owner->PressInputKey.GetFName() ;
		if(CurrentClickPressed)
		{
			CurrentClickPressed=false;
			FSlateApplication::Get().OnControllerButtonReleased(Press,0,false);
		}
		if(Owner->DoubleClickInputKey.IsValid()&&CurrentDoubleClickPressed)
		{
			CurrentDoubleClickPressed=false;
			const FGamepadKeyNames::Type DoubleClick = Owner->DoubleClickInputKey.GetFName() ;
			FSlateApplication::Get().OnControllerButtonReleased(DoubleClick,0,false);
		}
	}
}
	const FGamepadKeyNames::Type XAxis = (Owner->MainInputKey.IsValid() ? Owner->MainInputKey.GetFName() : ( FGamepadKeyNames::RightAnalogX  ));
	const FGamepadKeyNames::Type YAxis = (Owner->AltInputKey.IsValid() ? Owner->AltInputKey.GetFName() : (FGamepadKeyNames::RightAnalogY ));

if(HandleEvent)
{
	if(NumofTouches==1)
	{
		
			
		FSlateApplication::Get().OnControllerAnalog(XAxis, 0, Result.X*Owner->InputScale.X);
		FSlateApplication::Get().OnControllerAnalog(YAxis, 0, Result.Y*Owner->InputScale.Y);
		FSlateApplication::Get().SetAllUserFocusToGameViewport();
		
		HandleEvent=false;
	}else
	{
		FSlateApplication::Get().SetAllUserFocusToGameViewport();
		FSlateApplication::Get().OnControllerAnalog(XAxis, 0, 0);
		FSlateApplication::Get().OnControllerAnalog(YAxis, 0, 0);
	}
}else{
		

		FSlateApplication::Get().SetAllUserFocusToGameViewport();
		FSlateApplication::Get().OnControllerAnalog(XAxis, 0, 0);
		FSlateApplication::Get().OnControllerAnalog(YAxis, 0, 0);
	}
	float ScaleFactor = TMGetScaleFactor(AllottedGeometry);
	Owner->VisualCenter = FVector2D(TIResolveRelativePosition(0.5, AllottedGeometry.GetLocalSize().X, ScaleFactor), TIResolveRelativePosition(0.5, AllottedGeometry.GetLocalSize().Y, ScaleFactor));
	
	CorrectedVisualSize = FVector2D(TIResolveRelativePosition(Owner->VisualSize.X, AllottedGeometry.GetLocalSize().X, ScaleFactor), TIResolveRelativePosition(Owner->VisualSize.Y, AllottedGeometry.GetLocalSize().Y, ScaleFactor));

		
}



FVector2D STitanPannerMinimal::ComputeDesiredSize( float ) const
{
	return FVector2D(100, 100);
}

int32 STitanPannerMinimal::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{

	int RetLayerId=LayerId;
	if (Owner->Image1.IsValid())
	{
		FSlateDrawElement::MakeBox(
            OutDrawElements,
            RetLayerId++,
            AllottedGeometry.ToPaintGeometry(
            Owner->VisualCenter - FVector2D(CorrectedVisualSize.X * 0.5f, CorrectedVisualSize.Y * 0.5f),
            CorrectedVisualSize),
            Owner->Image1->GetSlateBrush(),
            ESlateDrawEffect::None,NumofTouches>0?Owner->ActiveColor:Owner->DeActiveColor
            
            );
	}


	
	return RetLayerId;
}
