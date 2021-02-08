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

	if(Owner->bIsDisabled)
	{Owner->OnClickedWhenDisabled.Broadcast();
		return  FReply::Unhandled();
	}
	
	NumofTouches++;
	if(NumofTouches==1)
	{
		

	Index1Location=MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );
	
	}else if(NumofTouches==2)
	{//two fingers
		
		Index2Location=	MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition());
		const FVector2D Delta=Index1Location-Index2Location;
		LastPinchSize=	(Delta).Size();
		LastAngle=FMath::Atan2(Delta.Y,Delta.X)*180/PI;
		
	
	}

	return FReply::Handled().CaptureMouse(SharedThis(this));

}

FReply STitanPanner::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	if(Owner->bIsDisabled)
	{//Owner->OnClickedWhenDisabled.Broadcast();
		return  FReply::Unhandled();
	}
const FVector2D TouchLocation=	MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition());

	
		if(NumofTouches==1)
		{
			Result=	Index1Location-TouchLocation ;
			HandleEvent=true;
			Index1Location= TouchLocation;
		}else if(NumofTouches==2)
		{
			if(Index1Location.Equals(TouchLocation,200))
			{
				Index1Location=TouchLocation;

			}
			if(Index2Location.Equals(TouchLocation,200))
			{
				Index2Location=TouchLocation;
			}
			
		}
		
		
	
 if(NumofTouches==2)
	{
 	
 const FVector2D Delta=Index1Location-Index2Location;
 	PinchResult= (Index1Location-Index2Location) .Size()-LastPinchSize;
	LastPinchSize=(Index1Location-Index2Location) .Size();
 	
	AngleResult= FMath::Atan2(Delta.Y,Delta.X)*180/PI-LastAngle;
 	LastAngle=FMath::Atan2(Delta.Y,Delta.X)*180/PI;


	
	
	}

	
	return FReply::Handled();
	
}

FReply STitanPanner::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event)
{if(NumofTouches==2)
{
	PinchResult=0;
}


	
	if(NumofTouches>1)
	{
		Result=FVector2D::ZeroVector;
	}
	NumofTouches--;



			
		
	
	return FReply::Handled().ReleaseMouseCapture();
	
}

void STitanPanner::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	
	
if(Owner->PressInputKey.IsValid())
{
	if(NumofTouches!=0)
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

			if(NumofTouches==2)
			{	if(Owner->PinchInputKey.IsValid())
			{
				const FGamepadKeyNames::Type Pinch = Owner->PinchInputKey.GetFName() ;
				FSlateApplication::Get().OnControllerAnalog(Pinch, 0, PinchResult);
				PinchResult=0;
			}
				if(Owner->PinchRotateInputKey.IsValid())
				{
					const FGamepadKeyNames::Type PinchRotate = Owner->PinchInputKey.GetFName() ;
					FSlateApplication::Get().OnControllerAnalog(PinchRotate, 0, AngleResult);
					AngleResult=0;
				}
				
			}
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
	if (Owner->Image1.IsValid())
	{
		FSlateDrawElement::MakeBox(
            OutDrawElements,
            RetLayerId++,
            AllottedGeometry.ToPaintGeometry(
            Owner->VisualCenter - FVector2D(CorrectedVisualSize.X * 0.5f, CorrectedVisualSize.Y * 0.5f),
            CorrectedVisualSize),
            Owner->Image1->GetSlateBrush(),
          static_cast<ESlateDrawEffect>(Owner->BackGroundDrawEffect),NumofTouches>0?Owner->ActiveColor:Owner->DeActiveColor
            
            );
		
	}

	if(Owner->TextToShow!=FString("NULL"))
	{
		FSlateDrawElement::MakeText(OutDrawElements,
        RetLayerId++,AllottedGeometry.ToPaintGeometry(
            Owner->VisualCenter -Owner->FontInfo.Size* FVector2D(CorrectedVisualSize.X , CorrectedVisualSize.Y )*Owner->FontCenterCorrection,
            CorrectedVisualSize),Owner->TextToShow,Owner->FontInfo,static_cast<ESlateDrawEffect>(Owner->TextDrawEffect),Owner->TextColor);
	}
	
	return RetLayerId;
}
