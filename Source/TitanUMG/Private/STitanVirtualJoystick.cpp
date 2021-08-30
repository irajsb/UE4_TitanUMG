// Copyright Epic Games, Inc. All Rights Reserved.

#include "STitanVirtualJoystick.h"
#include "Rendering/DrawElements.h"
#include "Misc/ConfigCacheIni.h"
#include "TitanJoystick.h"
#include "Framework/Application/SlateApplication.h"


const float OPACITY_LERP_RATE = 3.f;


static FORCEINLINE float GetScaleFactor(const FGeometry& Geometry)
{
	const float DesiredWidth = 1024.0f;

	float UndoDPIScaling = 1.0f / Geometry.Scale;
	return (Geometry.GetDrawSize().GetMax() / DesiredWidth) * UndoDPIScaling;
}

FORCEINLINE FLinearColor STitanVirtualJoystick::GetBaseColor()
{
	return (State == State_Active || State == State_CountingDownToInactive) ? Owner->ActiveColor : Owner->DeActiveColor;
}

void STitanVirtualJoystick::Reset()
{
	// snap the visual center back to normal (for controls that have a center on touch)
	VisualCenter = CorrectedCenter;
}

void STitanVirtualJoystick::Construct( const FArguments& InArgs )
{
	State = State_Inactive;
	bVisible = true;

	
	

	// listen for displaymetrics changes to reposition controls
	FSlateApplication::Get().GetPlatformApplication()->OnDisplayMetricsChanged().AddSP(this, &STitanVirtualJoystick::HandleDisplayMetricsChanged);
}

void STitanVirtualJoystick::HandleDisplayMetricsChanged(const FDisplayMetrics& NewDisplayMetric)
{
	// Mark all controls to be repositioned on next tick
	
		bHasBeenPositioned = false;

}



bool STitanVirtualJoystick::ShouldDisplayTouchInterface()
{
	bool bAlwaysShowTouchInterface = false;
	GConfig->GetBool(TEXT("/Script/Engine.InputSettings"), TEXT("bAlwaysShowTouchInterface"), bAlwaysShowTouchInterface, GInputIni);

	// do we want to show virtual joysticks?
	return FPlatformMisc::GetUseVirtualJoysticks() || bAlwaysShowTouchInterface || ( FSlateApplication::Get().IsFakingTouchEvents() && FPlatformMisc::ShouldDisplayTouchInterfaceOnFakingTouchEvents());
}

static int32 ResolveRelativePosition(float Position, float RelativeTo, float ScaleFactor)
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

static bool PositionIsInside(const FVector2D& Center, const FVector2D& Position, const FVector2D& BoxSize)
{
	return
		Position.X >= Center.X - BoxSize.X * 0.5f &&
		Position.X <= Center.X + BoxSize.X * 0.5f &&
		Position.Y >= Center.Y - BoxSize.Y * 0.5f &&
		Position.Y <= Center.Y + BoxSize.Y * 0.5f;
}

int32 STitanVirtualJoystick::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	int32 RetLayerId = LayerId;

	if (bVisible)
	{
		
		

		
		

			if (Owner-> Image2.IsValid())
			{
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					RetLayerId++,
					AllottedGeometry.ToPaintGeometry(
				VisualCenter - FVector2D(CorrectedVisualSize.X * 0.5f, CorrectedVisualSize.Y * 0.5f),
				CorrectedVisualSize),
					Owner->Image2->GetSlateBrush(),
					static_cast<ESlateDrawEffect>(Owner->BackGroundDrawEffect),
					CurrentColor
					);
			}

			if (Owner->Image1.IsValid())
			{
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					RetLayerId++,
					AllottedGeometry.ToPaintGeometry(
					VisualCenter + Index1Location - FVector2D(CorrectedThumbSize.X * 0.5f, CorrectedThumbSize.Y * 0.5f),
					CorrectedThumbSize),
					Owner->Image1->GetSlateBrush(),
					static_cast<ESlateDrawEffect>(Owner->ThumbDrawEffect),
					CurrentColor
					);
			}
		if(Owner->TextToShow!=FString("NULL"))
		{
			FSlateDrawElement::MakeText(OutDrawElements,
			RetLayerId++,AllottedGeometry.ToPaintGeometry(
                VisualCenter -Owner->FontInfo.Size* FVector2D(CorrectedVisualSize.X , CorrectedVisualSize.Y )*Owner->FontCenterCorrection,
                CorrectedVisualSize),Owner->TextToShow,Owner->FontInfo,static_cast<ESlateDrawEffect>(Owner->TextDrawEffect),Owner->TextColor);
		}
		
	}
	
	return RetLayerId;
}

FVector2D STitanVirtualJoystick::ComputeDesiredSize( float ) const
{
	return FVector2D(300, 300);
}

bool STitanVirtualJoystick::SupportsKeyboardFocus() const
{
	return false;
}

FReply STitanVirtualJoystick::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	NumofTouches++;
if(Owner->bIsDisabled)
{
	Owner->OnClickedWhenDisabled.Broadcast();
	return FReply::Unhandled();
}
	
	


	FVector2D LocalCoord = MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );

	
		

		// skip controls already in use
		if (CapturedPointerIndex == -1)
		{
			if (PositionIsInside(CorrectedCenter, LocalCoord, CorrectedInteractionSize))
			{
				// Align Joystick inside of Screen
				AlignBoxIntoScreen(LocalCoord, CorrectedVisualSize, MyGeometry.GetLocalSize());

				CapturedPointerIndex = Event.GetPointerIndex();

				if (Owner->ActivationDelay == 0.f)
				{
					CurrentColor = Owner->ActiveColor;

					if (!Owner->bPreventRecenter&&(PositionIsInside(CorrectedCenter, LocalCoord, CorrectedInteractionSize*Owner->RecenterSize)))
					{
						VisualCenter = LocalCoord;
					}

					if (HandleTouch(0, LocalCoord, MyGeometry.GetLocalSize())) // Never fail!
					{	
						return FReply::Handled().CaptureMouse(SharedThis(this));
						
					}
					
				}
				else
				{
					bNeedUpdatedCenter = true;
					ElapsedTime = 0.f;
					NextCenter = LocalCoord;

					return FReply::Unhandled();
				}
			}
		}
	



	return FReply::Unhandled();
}

FReply STitanVirtualJoystick::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	if(Owner->bIsDisabled&&NumofTouches==0)
	{
		
		return FReply::Unhandled();
	}
	FVector2D LocalCoord = MyGeometry.AbsoluteToLocal( Event.GetScreenSpacePosition() );

	
		

		// is this control the one captured to this pointer?
		if (CapturedPointerIndex == Event.GetPointerIndex())
		{
			if (bNeedUpdatedCenter)
			{
				return FReply::Unhandled();
			}
			else if (HandleTouch(0, LocalCoord, MyGeometry.GetLocalSize()))
			{
				return FReply::Handled();
			}
		}
	

	return FReply::Unhandled();
}

FReply STitanVirtualJoystick::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	NumofTouches--;
	
	
		// is this control the one captured to this pointer?
		if ( CapturedPointerIndex == Event.GetPointerIndex() )
		{
			// release and center the joystick
			Index1Location = FVector2D(0, 0);
			CapturedPointerIndex = -1;

			// send one more joystick update for the centering
			HandleEvent = true;

			// Pass event as unhandled if time is too short
			if ( bNeedUpdatedCenter )
			{
				bNeedUpdatedCenter = false;
				return FReply::Unhandled();
			}

			return FReply::Handled().ReleaseMouseCapture();
		}
	

	return FReply::Unhandled();
}

bool STitanVirtualJoystick::HandleTouch(int32 ControlIndex, const FVector2D& LocalCoord, const FVector2D& ScreenSize)
{
	

	// figure out position around center
	FVector2D Offset = LocalCoord -VisualCenter;
	//UE_LOG(LogTemp,Log,TEXT("LocalCord %f , %f"),LocalCoord.X,LocalCoord.Y);
	// only do work if we aren't at the center
	if (Offset == FVector2D(0, 0))
	{
		Index1Location = Offset;
	}
	else
	{
		// clamp to the ellipse of the stick (snaps to the visual size, so, the art should go all the way to the edge of the texture)
		float DistanceToTouchSqr = Offset.SizeSquared();
		float Angle = FMath::Atan2(Offset.Y, Offset.X);

		// length along line to ellipse: L = 1.0 / sqrt(((sin(T)/Rx)^2 + (cos(T)/Ry)^2))
		float CosAngle = FMath::Cos(Angle);
		float SinAngle = FMath::Sin(Angle);
		float XTerm = CosAngle / (CorrectedVisualSize.X * 0.5f);
		float YTerm = SinAngle / (CorrectedVisualSize.Y * 0.5f);
		float DistanceToEdge = FMath::InvSqrt(XTerm * XTerm + YTerm * YTerm);
		//UE_LOG(LogTemp,Log,TEXT("%f Distance to edge"),DistanceToEdge);
		// only clamp 
		if (DistanceToTouchSqr > FMath::Square(DistanceToEdge))
		{
			
			Index1Location = FVector2D(DistanceToEdge * CosAngle,  DistanceToEdge * SinAngle);
		}
		else
		{
			Index1Location = Offset;
		}
	
	}

	/*FVector2D AbsoluteThumbPos = ThumbPosition + VisualCenter;
	//AlignBoxIntoScreen(AbsoluteThumbPos, CorrectedThumbSize, ScreenSize);
	ThumbPosition = AbsoluteThumbPos - VisualCenter;
	UE_LOG(LogTemp,Log,TEXT("ThumbPosition %f %f "),ThumbPosition.X,ThumbPosition.Y);*/
	return true;
}

void STitanVirtualJoystick::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	if (State == State_WaitForStart || State == State_CountingDownToStart)
	{
		CurrentColor = FLinearColor(0,0,0,0);
	}
	else
	{
		// lerp to the desired opacity based on whether the user is interacting with the joystick
	//	CurrentColor = FMath::Lerp(CurrentOpacity, GetBaseOpacity(), OPACITY_LERP_RATE * InDeltaTime);
		
	CurrentColor=	FLinearColor::LerpUsingHSV(CurrentColor,GetBaseColor(),OPACITY_LERP_RATE * InDeltaTime);
		
	}

	// count how many controls are active
	//TODO remove this
	int32 NumActiveControls = 0;

	// figure out how much to scale the control sizes
	float ScaleFactor = GetScaleFactor(AllottedGeometry);

		

		if (bNeedUpdatedCenter)
		{
			ElapsedTime += InDeltaTime;
			if (ElapsedTime > Owner->ActivationDelay)
			{
				bNeedUpdatedCenter = false;
				CurrentColor = Owner->ActiveColor;

				if (!Owner->bPreventRecenter)
				{
				VisualCenter = NextCenter;
				}

				HandleTouch(0, NextCenter, AllottedGeometry.GetLocalSize());
			}
		}

		// calculate absolute positions based on geometry
		// @todo: Need to manage geometry changing!
		if (!bHasBeenPositioned || ScaleFactor != PreviousScalingFactor)
		{
			// update all the sizes
			CorrectedCenter = FVector2D(ResolveRelativePosition(Owner->Center.X, AllottedGeometry.GetLocalSize().X, ScaleFactor), ResolveRelativePosition(Owner->Center.Y, AllottedGeometry.GetLocalSize().Y, ScaleFactor));
			VisualCenter = CorrectedCenter;
			CorrectedVisualSize = FVector2D(ResolveRelativePosition(Owner->VisualSize.X, AllottedGeometry.GetLocalSize().X, ScaleFactor), ResolveRelativePosition(Owner->VisualSize.Y, AllottedGeometry.GetLocalSize().Y, ScaleFactor));
			CorrectedInteractionSize = FVector2D(ResolveRelativePosition(Owner->InteractionSize.X, AllottedGeometry.GetLocalSize().X, ScaleFactor), ResolveRelativePosition(Owner->InteractionSize.Y, AllottedGeometry.GetLocalSize().Y, ScaleFactor));
			CorrectedThumbSize = FVector2D(ResolveRelativePosition(Owner->ThumbSize.X, AllottedGeometry.GetLocalSize().X, ScaleFactor), ResolveRelativePosition(Owner->ThumbSize.Y, AllottedGeometry.GetLocalSize().Y, ScaleFactor));
			CorrectedInputScale = Owner->InputScale; // *ScaleFactor;
			bHasBeenPositioned = true;
		}

		if (CapturedPointerIndex >= 0 || HandleEvent)
		{
			//Handle event is for sending one more input
			HandleEvent = false;

			// Get the corrected thumb offset scale (now allows ellipse instead of assuming square)
			FVector2D ThumbScaledOffset = FVector2D(Index1Location.X * 2.0f / CorrectedVisualSize.X, Index1Location.Y * 2.0f / CorrectedVisualSize.Y);
			float ThumbSquareSum = ThumbScaledOffset.X * ThumbScaledOffset.X + ThumbScaledOffset.Y * ThumbScaledOffset.Y;
			float ThumbMagnitude = FMath::Sqrt(ThumbSquareSum);
			FVector2D ThumbNormalized = FVector2D(0.f, 0.f);
			if (ThumbSquareSum > SMALL_NUMBER)
			{
				const float Scale = 1.0f / ThumbMagnitude;
				ThumbNormalized = FVector2D(ThumbScaledOffset.X * Scale, ThumbScaledOffset.Y * Scale);
			}

			// Find the scale to apply to ThumbNormalized vector to project onto unit square
			float ToSquareScale = fabs(ThumbNormalized.Y) > fabs(ThumbNormalized.X) ? FMath::Sqrt((ThumbNormalized.X * ThumbNormalized.X) / (ThumbNormalized.Y * ThumbNormalized.Y) + 1.0f)
				: ThumbNormalized.X == 0.0f ? 1.0f : FMath::Sqrt((ThumbNormalized.Y * ThumbNormalized.Y) / (ThumbNormalized.X * ThumbNormalized.X) + 1.0f);

			// Apply proportional offset corrected for projection to unit square
			FVector2D NormalizedOffset = ThumbNormalized * CorrectedInputScale * ThumbMagnitude * ToSquareScale;

			// now pass the fake joystick events to the game
			//press and release handling
			
			const FGamepadKeyNames::Type XAxis = (Owner->MainInputKey.IsValid() ? Owner->MainInputKey.GetFName() : ( FGamepadKeyNames::LeftAnalogX  ));
			const FGamepadKeyNames::Type YAxis = (Owner->AltInputKey.IsValid() ? Owner->AltInputKey.GetFName() : (FGamepadKeyNames::LeftAnalogY ));

			FSlateApplication::Get().SetAllUserFocusToGameViewport();
			FSlateApplication::Get().OnControllerAnalog(XAxis, 0, NormalizedOffset.X);
			FSlateApplication::Get().OnControllerAnalog(YAxis, 0, -NormalizedOffset.Y);
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
		}

		// is this active?
		if (CapturedPointerIndex != -1)
		{
			NumActiveControls++;
		}
	

	// we need to store the computed scale factor so we can compare it with the value computed in the following frame and, if necessary, recompute widget position
	PreviousScalingFactor = ScaleFactor;

	// STATE MACHINE!
	if (NumActiveControls > 0 || Owner->bPreventRecenter)
	{
		// any active control snaps the state to active immediately
		State = State_Active;
	}
	else
	{
		switch (State)
		{
			case State_WaitForStart:
				{
					State = State_CountingDownToStart;
					Countdown = Owner->StartupDelay;
				}
				break;
			case State_CountingDownToStart:
				// update the countdown
				Countdown -= InDeltaTime;
				if (Countdown <= 0.0f)
				{
					State = State_Inactive;
				}
				break;
			case State_Active:
				if (NumActiveControls == 0)
				{
					// start going to inactive
					State = State_CountingDownToInactive;
					Countdown = Owner->TimeUntilDeactive;
				}
				break;

			case State_CountingDownToInactive:
				// update the countdown
				Countdown -= InDeltaTime;
				if (Countdown <= 0.0f)
				{
					// should we start counting down to a control reset?
					if (Owner->TimeUntilReset > 0.0f)
					{
						State = State_CountingDownToReset;
						Countdown = Owner->TimeUntilReset;
					}
					else
					{
						// if not, then just go inactive
						State = State_Inactive;
					}
				}
				break;

			case State_CountingDownToReset:
				Countdown -= InDeltaTime;
				if (Countdown <= 0.0f)
				{
					// reset all the controls
					
						Reset();
					

					// finally, go inactive
					State = State_Inactive;
				}
				break;
		}
	}
}

void STitanVirtualJoystick::SetJoystickVisibility(const bool bInVisible, const bool bInFade)
{
	// if we aren't fading, then just set the current opacity to desired
	if (!bInFade)
	{
		if (bInVisible)
		{
			CurrentColor = GetBaseColor();
		}
		else
		{
			CurrentColor = FLinearColor(0,0,0,0);
		}
	}

	bVisible = bInVisible;
}

void STitanVirtualJoystick::AlignBoxIntoScreen(FVector2D& Position, const FVector2D& Size, const FVector2D& ScreenSize)
{
	if ( Size.X > ScreenSize.X || Size.Y > ScreenSize.Y )
	{
		return;
	}

	// Align box to fit into screen
	if ( Position.X - Size.X * 0.5f < 0.f )
	{
		Position.X = Size.X * 0.5f;
	}

	if ( Position.X + Size.X * 0.5f > ScreenSize.X )
	{
		Position.X = ScreenSize.X - Size.X * 0.5f;
	}

	if ( Position.Y - Size.Y * 0.5f < 0.f )
	{
		Position.Y = Size.Y * 0.5f;
	}

	if ( Position.Y + Size.Y * 0.5f > ScreenSize.Y )
	{
		Position.Y = ScreenSize.Y - Size.Y * 0.5f;
	}
}
