// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanProgressBar.h"
#include "Slate/SlateBrushAsset.h"
#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UTitanProgressBar

static FProgressBarStyle* DefaultProgressBarStyle = nullptr;

UTitanProgressBar::UTitanProgressBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultProgressBarStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultProgressBarStyle = new FProgressBarStyle(FCoreStyle::Get().GetWidgetStyle<FProgressBarStyle>("ProgressBar"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultProgressBarStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultProgressBarStyle;
	WidgetStyle.FillImage.TintColor = FLinearColor::White;

	BarFillType = ETitanProgressBarFillType::LeftToRight;
	bIsMarquee = false;
	Percent = 0;
	FillColorAndOpacity = FLinearColor::White;
	BorderPadding = FVector2D(0, 0);
}

void UTitanProgressBar::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyProgressBar.Reset();
}

TSharedRef<SWidget> UTitanProgressBar::RebuildWidget()
{
	MyProgressBar = SNew(STitanProgressBar);

	return MyProgressBar.ToSharedRef();
}

void UTitanProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute< TOptional<float> > PercentBinding = OPTIONAL_BINDING_CONVERT(float, Percent, TOptional<float>, ConvertFloatToOptionalFloat);
	TAttribute<FSlateColor> FillColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, FillColorAndOpacity);

	MyProgressBar->SetStyle(&WidgetStyle);

	MyProgressBar->SetBarFillType(BarFillType);
	MyProgressBar->SetPercent(bIsMarquee ? TOptional<float>() : PercentBinding);
	MyProgressBar->SetFillColorAndOpacity(FillColorAndOpacityBinding);
	MyProgressBar->SetBorderPadding(BorderPadding);
	MyProgressBar->SetIndicatorImage(&DamageIndicator);
	MyProgressBar->FreezeLagAfterUpdate=LagFreezeAfterUpdate;
	MyProgressBar->LerpSpeed=DamageLagInterpolationSpeed;
}

void UTitanProgressBar::SetIsMarquee(bool InbIsMarquee)
{
	bIsMarquee = InbIsMarquee;
	if (MyProgressBar.IsValid())
	{
		MyProgressBar->SetPercent(bIsMarquee ? TOptional<float>() : Percent);
	}
}

void UTitanProgressBar::SetFillColorAndOpacity(FLinearColor Color)
{
	FillColorAndOpacity = Color;
	if (MyProgressBar.IsValid())
	{
		MyProgressBar->SetFillColorAndOpacity(FillColorAndOpacity);
	}
}

void UTitanProgressBar::SetPercent(float InPercent)
{
	Percent = InPercent;
	if (MyProgressBar.IsValid())
	{
		MyProgressBar->SetPercent(InPercent);
	}
}

void UTitanProgressBar::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS)
	{
		if (Style_DEPRECATED != nullptr)
		{
			const FProgressBarStyle* StylePtr = Style_DEPRECATED->GetStyle<FProgressBarStyle>();
			if (StylePtr != nullptr)
			{
				WidgetStyle = *StylePtr;
			}

			Style_DEPRECATED = nullptr;
		}

		if (BackgroundImage_DEPRECATED != nullptr)
		{
			WidgetStyle.BackgroundImage = BackgroundImage_DEPRECATED->Brush;
			BackgroundImage_DEPRECATED = nullptr;
		}

		if (FillImage_DEPRECATED != nullptr)
		{
			WidgetStyle.FillImage = FillImage_DEPRECATED->Brush;
			FillImage_DEPRECATED = nullptr;
		}

		if (MarqueeImage_DEPRECATED != nullptr)
		{
			WidgetStyle.MarqueeImage = MarqueeImage_DEPRECATED->Brush;
			MarqueeImage_DEPRECATED = nullptr;
		}
	}
}

#if WITH_EDITOR

const FText UTitanProgressBar::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

void UTitanProgressBar::OnCreationFromPalette()
{
	FillColorAndOpacity = FLinearColor(0, 0.5f, 1.0f);
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
