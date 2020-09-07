// Copyright Epic Games, Inc. All Rights Reserved.

#include "STitanPanner.h"
#include "TitanPanner.h"


static FORCEINLINE float TGetScaleFactor(const FGeometry& Geometry)
{
	const float DesiredWidth = 1024.0f;

	float UndoDPIScaling = 1.0f / Geometry.Scale;
	return (Geometry.GetDrawSize().GetMax() / DesiredWidth) * UndoDPIScaling;
}


static int32 TResolveRelativePosition(float Position, float RelativeTo, float ScaleFactor)
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






bool STitanPanner::SupportsKeyboardFocus() const
{
	return false;
}

void STitanPanner::Construct(const FArguments& InArgs)
{
}

FReply STitanPanner::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	NumofTouches++;
	if(Index1==-1)
	{
	Index1=	Event.GetTouchpadIndex();
	Index1Location=MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );
	GEngine->AddOnScreenDebugMessage(0,10,FColor::Red,FString("Index1"));
	}
	if(Index1!=-1&&Index2==-1&&NumofTouches==2)
	{
	Index2=Event.GetTouchpadIndex();
	Index2Location=MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );
	GEngine->AddOnScreenDebugMessage(0,10,FColor::Red,FString("Index2"));

	}
	return FReply::Handled().CaptureMouse(SharedThis(this));
	//return FReply::Unhandled();
}

FReply STitanPanner::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event)
{

	if(Event.GetTouchpadIndex()==Index1)
	{
		Result=	Index1Location-MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );
		Index1Location=MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );
		HandleEvent=true;
	}
	
	return FReply::Handled();
	
}

FReply STitanPanner::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	NumofTouches--;

	if(Event.GetTouchpadIndex()==Index1)
	{
		Index1=-1;
	}
	if(Event.GetTouchpadIndex()==Index2)
	{
		Index2=-1;
	}

			
		
	
	return FReply::Handled().ReleaseMouseCapture();
	
}

void STitanPanner::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

if(Owner->PressInputKey.IsValid())
{
	if(Index1!=-1)
	{
		const FGamepadKeyNames::Type Press = Owner->PressInputKey.GetFName() ;
		FSlateApplication::Get().OnControllerButtonPressed(Press,0,false);
	}else
	{
		const FGamepadKeyNames::Type Press = Owner->PressInputKey.GetFName() ;
		FSlateApplication::Get().OnControllerButtonReleased(Press,0,false);
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
	float ScaleFactor = TGetScaleFactor(AllottedGeometry);
	Owner->VisualCenter = FVector2D(TResolveRelativePosition(0.5, AllottedGeometry.GetLocalSize().X, ScaleFactor), TResolveRelativePosition(0.5, AllottedGeometry.GetLocalSize().Y, ScaleFactor));
	
	CorrectedVisualSize = FVector2D(TResolveRelativePosition(Owner->VisualSize.X, AllottedGeometry.GetLocalSize().X, ScaleFactor), TResolveRelativePosition(Owner->VisualSize.Y, AllottedGeometry.GetLocalSize().Y, ScaleFactor));

			
}



FVector2D STitanPanner::ComputeDesiredSize( float ) const
{
	return FVector2D(100, 100);
}

int32 STitanPanner::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{

	int RetLayerId=LayerId;
	if (Image1.IsValid())
	{
		FSlateDrawElement::MakeBox(
            OutDrawElements,
            RetLayerId++,
            AllottedGeometry.ToPaintGeometry(
            Owner->VisualCenter - FVector2D(CorrectedVisualSize.X * 0.5f, CorrectedVisualSize.Y * 0.5f),
            CorrectedVisualSize),
            Image1->GetSlateBrush(),
            ESlateDrawEffect::None,NumofTouches>0?Owner->ActiveColor:Owner->DeActiveColor
            
            );
	}


	
	return RetLayerId;
}
