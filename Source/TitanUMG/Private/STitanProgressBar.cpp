// Copyright Epic Games, Inc. All Rights Reserved.

#include "STitanProgressBar.h"
#include "Rendering/DrawElements.h"



void STitanProgressBar::Construct( const FArguments& InArgs )
{
	check(InArgs._Style);

	MarqueeOffset = 0.0f;

	Style = InArgs._Style;

	SetPercent(InArgs._Percent);
	BarFillType = InArgs._BarFillType;
	
	BackgroundImage = InArgs._BackgroundImage;
	FillImage = InArgs._FillImage;
	MarqueeImage = InArgs._MarqueeImage;
	
	FillColorAndOpacity = InArgs._FillColorAndOpacity;
	BorderPadding = InArgs._BorderPadding;

	CurrentTickRate = 0.0f;
	MinimumTickRate = InArgs._RefreshRate;
	
	SetCanTick(true);

	UpdateMarqueeActiveTimer();
}

void STitanProgressBar::SetPercent(TAttribute< TOptional<float> > InPercent)
{
	if ( !Percent.IdenticalTo(InPercent) )
	{
		
		Percent = InPercent;
		if(Percent.Get().IsSet())
		{
			if(Percent.Get().GetValue()>LerpedPercent)
			{
				LerpedPercent=Percent.Get().GetValue();
			}else
			{
				CurrentFreezeLagAfterUpdate=FreezeLagAfterUpdate;
				
			}
		}
		UpdateMarqueeActiveTimer();
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}

void STitanProgressBar::SetStyle(const FProgressBarStyle* InStyle)
{
	Style = InStyle;

	if (Style == nullptr)
	{
		FArguments Defaults;
		Style = Defaults._Style;
	}

	check(Style);

	Invalidate(EInvalidateWidget::Layout);
}

void STitanProgressBar::SetBarFillType(ETitanProgressBarFillType::Type InBarFillType)
{
	if(BarFillType != InBarFillType)
	{
		BarFillType = InBarFillType;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void STitanProgressBar::SetFillColorAndOpacity(TAttribute< FSlateColor > InFillColorAndOpacity)
{
	if(!FillColorAndOpacity.IdenticalTo(InFillColorAndOpacity))
	{
		FillColorAndOpacity = InFillColorAndOpacity;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void STitanProgressBar::SetBorderPadding(TAttribute< FVector2D > InBorderPadding)
{
	if(!BorderPadding.IdenticalTo(InBorderPadding))
	{
		BorderPadding = InBorderPadding;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void STitanProgressBar::SetBackgroundImage(const FSlateBrush* InBackgroundImage)
{
	if(BackgroundImage != InBackgroundImage)
	{
		BackgroundImage = InBackgroundImage;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void STitanProgressBar::SetIndicatorImage( FSlateBrush* InBackgroundImage)
{
	IndicatorImage=InBackgroundImage;
}

void STitanProgressBar::SetFillImage(const FSlateBrush* InFillImage)
{
	if(FillImage != InFillImage)
	{
		FillImage = InFillImage;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void STitanProgressBar::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if(CurrentFreezeLagAfterUpdate!=0)
	{
		CurrentFreezeLagAfterUpdate--;
	}else
	{
		if(Percent.Get().IsSet())
		LerpedPercent=FMath::FInterpConstantTo(LerpedPercent,Percent.Get().GetValue(),InDeltaTime,LerpSpeed);
	}
}

void STitanProgressBar::SetMarqueeImage(const FSlateBrush* InMarqueeImage)
{
	if(MarqueeImage != InMarqueeImage)
	{
		MarqueeImage = InMarqueeImage;
		Invalidate(EInvalidateWidget::Layout);
	}
}

const FSlateBrush* STitanProgressBar::GetBackgroundImage() const
{
	return BackgroundImage ? BackgroundImage : &Style->BackgroundImage;
}

const FSlateBrush* STitanProgressBar::GetFillImage() const
{
	return FillImage ? FillImage : &Style->FillImage;
}

const FSlateBrush* STitanProgressBar::GetMarqueeImage() const
{
	return MarqueeImage ? MarqueeImage : &Style->MarqueeImage;
}

// Returns false if the clipping zone area is zero in which case we should skip drawing
bool PushTransformedClipTitan(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, FVector2D InsetPadding, FVector2D ProgressOrigin, FSlateRect Progress)
{
	const FSlateRenderTransform& Transform = AllottedGeometry.GetAccumulatedRenderTransform();

	const FVector2D MaxSize = AllottedGeometry.GetLocalSize() - ( InsetPadding * 2.0f );

	const FSlateClippingZone ClippingZone(Transform.TransformPoint(InsetPadding + (ProgressOrigin - FVector2D(Progress.Left, Progress.Top)) * MaxSize),
		Transform.TransformPoint(InsetPadding + FVector2D(ProgressOrigin.X + Progress.Right, ProgressOrigin.Y - Progress.Top) * MaxSize),
		Transform.TransformPoint(InsetPadding + FVector2D(ProgressOrigin.X - Progress.Left, ProgressOrigin.Y + Progress.Bottom) * MaxSize),
		Transform.TransformPoint(InsetPadding + (ProgressOrigin + FVector2D(Progress.Right, Progress.Bottom)) * MaxSize));

	if (ClippingZone.HasZeroArea())
	{
		return false;
	}

	OutDrawElements.PushClip(ClippingZone);
	return true;
}

int32 STitanProgressBar::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	// Used to track the layer ID we will return.
	int32 RetLayerId = LayerId;
	
	bool bEnabled = ShouldBeEnabled( bParentEnabled );
	const ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
	
	const FSlateBrush* CurrentFillImage = GetFillImage();
	
	const FLinearColor FillColorAndOpacitySRGB(InWidgetStyle.GetColorAndOpacityTint() * FillColorAndOpacity.Get().GetColor(InWidgetStyle) * CurrentFillImage->GetTint(InWidgetStyle));
	const FLinearColor ColorAndOpacitySRGB = InWidgetStyle.GetColorAndOpacityTint();

	TOptional<float> ProgressFraction = Percent.Get();
	
	FVector2D BorderPaddingRef = BorderPadding.Get();

	
	const FSlateBrush* CurrentBackgroundImage = GetBackgroundImage();

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		RetLayerId++,
		AllottedGeometry.ToPaintGeometry(),
		CurrentBackgroundImage,
		DrawEffects,
		InWidgetStyle.GetColorAndOpacityTint() * CurrentBackgroundImage->GetTint( InWidgetStyle )
	);	
	
	if( ProgressFraction.IsSet() )
	{
		ETitanProgressBarFillType::Type ComputedBarFillType = BarFillType;
		if (GSlateFlowDirection == EFlowDirection::RightToLeft)
		{
			switch (ComputedBarFillType)
			{
			case ETitanProgressBarFillType::LeftToRight:
				ComputedBarFillType = ETitanProgressBarFillType::RightToLeft;
				break;
			case ETitanProgressBarFillType::RightToLeft:
				ComputedBarFillType = ETitanProgressBarFillType::LeftToRight;
				break;
			}
		}

		const float ClampedFraction = FMath::Clamp(ProgressFraction.GetValue(), 0.0f, 1.0f);
		const float LerpedClampedFraction = FMath::Clamp(LerpedPercent, 0.0f, 1.0f);
		FLinearColor IndicatorColor=IndicatorImage->TintColor.GetSpecifiedColor();
			switch (ComputedBarFillType)
		{
			case ETitanProgressBarFillType::RightToLeft:
			{
					if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(1, 0), FSlateRect(LerpedClampedFraction, 0, 0, 1)))
					{
						// Draw Fill
						FSlateDrawElement::MakeBox(
                            OutDrawElements,
                            RetLayerId++,
                            AllottedGeometry.ToPaintGeometry(
                                FVector2D::ZeroVector,
                                FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
                            IndicatorImage,
                            DrawEffects,
                            IndicatorColor
                            );

						OutDrawElements.PopClip();
					}

					//fill
				if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(1, 0), FSlateRect(ClampedFraction, 0, 0, 1)))
				{
					// Draw Fill
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						RetLayerId++,
						AllottedGeometry.ToPaintGeometry(
							FVector2D::ZeroVector,
							FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
						CurrentFillImage,
						DrawEffects,
						FillColorAndOpacitySRGB
						);

					OutDrawElements.PopClip();
				}
				break;
			}
			case ETitanProgressBarFillType::FillFromCenter:
			{
				float HalfFrac = ClampedFraction / 2.0f;
				const float HalfLerpFrac=LerpedClampedFraction/2.0f;


					if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0.5, 0.5), FSlateRect(HalfLerpFrac, HalfLerpFrac, HalfLerpFrac, HalfLerpFrac)))
					{
						// Draw Fill
						FSlateDrawElement::MakeBox(
                            OutDrawElements,
                            RetLayerId++,
                            AllottedGeometry.ToPaintGeometry(
                                FVector2D( (AllottedGeometry.GetLocalSize().X * 0.5f) - ((AllottedGeometry.GetLocalSize().X * ( LerpedClampedFraction ))*0.5), 0.0f),
                                FVector2D( AllottedGeometry.GetLocalSize().X * ( LerpedClampedFraction ) , AllottedGeometry.GetLocalSize().Y )),
                            IndicatorImage,
                            DrawEffects,
                            IndicatorColor
                            );

						OutDrawElements.PopClip();
					}
				if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0.5, 0.5), FSlateRect(HalfFrac, HalfFrac, HalfFrac, HalfFrac)))
				{
					// Draw Fill
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						RetLayerId++,
						AllottedGeometry.ToPaintGeometry(
							FVector2D( (AllottedGeometry.GetLocalSize().X * 0.5f) - ((AllottedGeometry.GetLocalSize().X * ( ClampedFraction ))*0.5), 0.0f),
							FVector2D( AllottedGeometry.GetLocalSize().X * ( ClampedFraction ) , AllottedGeometry.GetLocalSize().Y )),
						CurrentFillImage,
						DrawEffects,
						FillColorAndOpacitySRGB
						);

					OutDrawElements.PopClip();
				}
				break;
			}
			case ETitanProgressBarFillType::TopToBottom:
			{

					if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 0), FSlateRect(0, 0, 1, LerpedClampedFraction)))
					{
						// Draw Fill
						FSlateDrawElement::MakeBox(
                            OutDrawElements,
                            RetLayerId++,
                            AllottedGeometry.ToPaintGeometry(
                                FVector2D::ZeroVector,
                                FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
                            IndicatorImage,
                            DrawEffects,
                            IndicatorColor
                            );

						OutDrawElements.PopClip();
					}
				if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 0), FSlateRect(0, 0, 1, ClampedFraction)))
				{
					// Draw Fill
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						RetLayerId++,
						AllottedGeometry.ToPaintGeometry(
							FVector2D::ZeroVector,
							FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
						CurrentFillImage,
						DrawEffects,
						FillColorAndOpacitySRGB
						);

					OutDrawElements.PopClip();
				}
				break;
			}
			case ETitanProgressBarFillType::BottomToTop:
			{


					if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 1), FSlateRect(0, LerpedClampedFraction, 1, 0)))
					{
						FSlateRect ClippedAllotedGeometry = FSlateRect(AllottedGeometry.AbsolutePosition, AllottedGeometry.AbsolutePosition + AllottedGeometry.GetLocalSize() * AllottedGeometry.Scale);
						ClippedAllotedGeometry.Top = ClippedAllotedGeometry.Bottom - ClippedAllotedGeometry.GetSize().Y * ClampedFraction;

						// Draw Fill
						FSlateDrawElement::MakeBox(
                            OutDrawElements,
                            RetLayerId++,
                            AllottedGeometry.ToPaintGeometry(
                                FVector2D::ZeroVector,
                                FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
                            IndicatorImage,
                            DrawEffects,
                            IndicatorColor
                            );

						OutDrawElements.PopClip();
					}
				if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 1), FSlateRect(0, ClampedFraction, 1, 0)))
				{
					FSlateRect ClippedAllotedGeometry = FSlateRect(AllottedGeometry.AbsolutePosition, AllottedGeometry.AbsolutePosition + AllottedGeometry.GetLocalSize() * AllottedGeometry.Scale);
					ClippedAllotedGeometry.Top = ClippedAllotedGeometry.Bottom - ClippedAllotedGeometry.GetSize().Y * ClampedFraction;

					// Draw Fill
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						RetLayerId++,
						AllottedGeometry.ToPaintGeometry(
							FVector2D::ZeroVector,
							FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
						CurrentFillImage,
						DrawEffects,
						FillColorAndOpacitySRGB
						);

					OutDrawElements.PopClip();
				}
				break;
			}
			case ETitanProgressBarFillType::LeftToRight:
			default:
			{

					if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 0), FSlateRect(0, 0, LerpedClampedFraction, 1)))
					{
						// Draw Fill
						FSlateDrawElement::MakeBox(
                            OutDrawElements,
                            RetLayerId++,
                            AllottedGeometry.ToPaintGeometry(
                                FVector2D::ZeroVector,
                                FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
                            IndicatorImage,
                            DrawEffects,
                            IndicatorColor
                            );

						OutDrawElements.PopClip();
					}
				if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 0), FSlateRect(0, 0, ClampedFraction, 1)))
				{
					// Draw Fill
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						RetLayerId++,
						AllottedGeometry.ToPaintGeometry(
							FVector2D::ZeroVector,
							FVector2D( AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y )),
						CurrentFillImage,
						DrawEffects,
						FillColorAndOpacitySRGB
						);

					OutDrawElements.PopClip();
				}
				break;
			}
		}
	}
	else
	{
		const FSlateBrush* CurrentMarqueeImage = GetMarqueeImage();
		
		// Draw Marquee
		const float MarqueeAnimOffset = CurrentMarqueeImage->ImageSize.X * MarqueeOffset;
		const float MarqueeImageSize = CurrentMarqueeImage->ImageSize.X;

		if (PushTransformedClipTitan(OutDrawElements, AllottedGeometry, BorderPaddingRef, FVector2D(0, 0), FSlateRect(0, 0, 1, 1)))
		{
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				RetLayerId++,
				AllottedGeometry.ToPaintGeometry(
					FVector2D( MarqueeAnimOffset - MarqueeImageSize, 0.0f ),
					FVector2D( AllottedGeometry.GetLocalSize().X + MarqueeImageSize, AllottedGeometry.GetLocalSize().Y )),
				CurrentMarqueeImage,
				DrawEffects,
				CurrentMarqueeImage->TintColor.GetSpecifiedColor() * ColorAndOpacitySRGB
				);

			OutDrawElements.PopClip();
		}
	}

	return RetLayerId - 1;
}

FVector2D STitanProgressBar::ComputeDesiredSize( float ) const
{
	return GetMarqueeImage()->ImageSize;
}

bool STitanProgressBar::ComputeVolatility() const
{
	return SLeafWidget::ComputeVolatility() || Percent.IsBound() || FillColorAndOpacity.IsBound() || BorderPadding.IsBound();
}

void STitanProgressBar::SetActiveTimerTickRate(float TickRate)
{
	if (CurrentTickRate != TickRate || !ActiveTimerHandle.IsValid())
	{
		CurrentTickRate = TickRate;

		TSharedPtr<FActiveTimerHandle> SharedActiveTimerHandle = ActiveTimerHandle.Pin();
		if (SharedActiveTimerHandle.IsValid())
		{
			UnRegisterActiveTimer(SharedActiveTimerHandle.ToSharedRef());
		}

		UpdateMarqueeActiveTimer();
	}
}

void STitanProgressBar::UpdateMarqueeActiveTimer()
{
	if (ActiveTimerHandle.IsValid())
	{
		UnRegisterActiveTimer(ActiveTimerHandle.Pin().ToSharedRef());
	}

	if (!Percent.IsBound() && !Percent.Get().IsSet())
	{
		// If percent is not bound or set then its marquee. Set the timer
		ActiveTimerHandle = RegisterActiveTimer(CurrentTickRate, FWidgetActiveTimerDelegate::CreateSP(this, &STitanProgressBar::ActiveTick));
	}
}

EActiveTimerReturnType STitanProgressBar::ActiveTick(double InCurrentTime, float InDeltaTime)
{
	MarqueeOffset = InCurrentTime - FMath::FloorToDouble(InCurrentTime);
	
	TOptional<float> PercentFraction = Percent.Get();
	if (PercentFraction.IsSet())
	{
		SetActiveTimerTickRate(MinimumTickRate);
	}
	else
	{
		SetActiveTimerTickRate(0.0f);
	}

	Invalidate(EInvalidateWidget::Paint);

	return EActiveTimerReturnType::Continue;
}


