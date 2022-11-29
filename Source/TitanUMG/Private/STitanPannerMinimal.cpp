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
			HandleControllerButtonReleased(Press,false);
			CurrentClickPressed=!DoubleClicked;
			CurrentDoubleClickPressed=DoubleClicked;
		}
		
		
	}else
	{
		
		const FGamepadKeyNames::Type Press = Owner->PressInputKey.GetFName() ;
		if(CurrentClickPressed)
		{
			CurrentClickPressed=false;
			HandleControllerButtonReleased(Press,false);
		}
		if(Owner->DoubleClickInputKey.IsValid()&&CurrentDoubleClickPressed)
		{
			CurrentDoubleClickPressed=false;
			const FGamepadKeyNames::Type DoubleClick = Owner->DoubleClickInputKey.GetFName() ;
			HandleControllerButtonReleased(DoubleClick,false);
		}
	}
}
	const FGamepadKeyNames::Type XAxis = (Owner->MainInputKey.IsValid() ? Owner->MainInputKey.GetFName() : ( FGamepadKeyNames::RightAnalogX  ));
	const FGamepadKeyNames::Type YAxis = (Owner->AltInputKey.IsValid() ? Owner->AltInputKey.GetFName() : (FGamepadKeyNames::RightAnalogY ));

if(HandleEvent)
{
	if(NumofTouches==1)
	{
		
			
		HandleControllerAnalog(XAxis, Result.X*Owner->InputScale.X);
		HandleControllerAnalog(YAxis, Result.Y*Owner->InputScale.Y);
		FSlateApplication::Get().SetAllUserFocusToGameViewport();
		
		HandleEvent=false;
	}else
	{
		FSlateApplication::Get().SetAllUserFocusToGameViewport();
		HandleControllerAnalog(XAxis, 0.f);
		HandleControllerAnalog(YAxis, 0.f);
	}
}else{
		

		FSlateApplication::Get().SetAllUserFocusToGameViewport();
		HandleControllerAnalog(XAxis, 0.f);
		HandleControllerAnalog(YAxis, 0.f);
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


bool STitanPannerMinimal::HandleControllerAnalog(FGamepadKeyNames::Type KeyName, float AnalogValue)
{
#if ENGINE_MAJOR_VERSION>4 && ENGINE_MINOR_VERSION>0
	const FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	return 	FSlateApplication::Get().OnControllerAnalog(KeyName, FSlateApplicationBase::SlateAppPrimaryPlatformUser, PrimaryInputDevice, AnalogValue);
#elif
	return 	FSlateApplication::Get().OnControllerAnalog(KeyName, 0, AnalogValue);
#endif
	
}

bool STitanPannerMinimal::HandleControllerButtonPressed(FGamepadKeyNames::Type KeyName, bool IsRepeat)
{
#if ENGINE_MAJOR_VERSION>4 && ENGINE_MINOR_VERSION>0
	const FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	return 	FSlateApplication::Get().OnControllerButtonPressed(KeyName, FSlateApplicationBase::SlateAppPrimaryPlatformUser, PrimaryInputDevice, IsRepeat);
	
#elif
	return FSlateApplication::Get().OnControllerButtonPressed(KeyName,0,IsRepeat);
#endif
}

bool STitanPannerMinimal::HandleControllerButtonReleased(FGamepadKeyNames::Type KeyName, bool IsRepeat)
{
#if ENGINE_MAJOR_VERSION>4 && ENGINE_MINOR_VERSION>0
	const FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	return 	FSlateApplication::Get().OnControllerButtonReleased(KeyName, FSlateApplicationBase::SlateAppPrimaryPlatformUser, PrimaryInputDevice, IsRepeat);
	
#elif
	return FSlateApplication::Get().OnControllerButtonReleased(KeyName,0,IsRepeat);
#endif
}
