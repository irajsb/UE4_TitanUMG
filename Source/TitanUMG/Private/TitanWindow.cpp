// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanWindow.h"
#include "STitanWindow.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Font.h"
#include "Configs/TitanButtonStyle.h"


UTitanWindow::UTitanWindow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	BorderBackground.TintColor=FLinearColor(1,1,1,1);
	BackGroundBorderColor=FLinearColor(0,0,0,0.7);
	BackGroundBorderColorHovered=FLinearColor(0,0,0,1);
	TopBorderColor=FLinearColor(0.04,0.04,0.04,1);
	TopBorderColorHovered=FLinearColor(0.02,0.02,0.02,1);
	BackGroundBorderPadding=FMargin(10);
	WindowText=FText::FromString("Window");
	if (!IsRunningDedicatedServer())
	{
		 ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		WindowFont = FSlateFontInfo(RobotoFontObj.Object, 16, FName("Bold"));
	}
	
	const auto Result= ConstructorHelpers::FObjectFinder<UTitanButtonStyle>(TEXT("/TitanUMG/Slate/DefaultButtonStyle"));
	if(Result.Succeeded())
	{
		QuitButtonStyle=Result.Object->ButtonStyle;
	}

	const auto QuitResult= ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/TitanUMG/Slate/Textures/Cross"));
	if(QuitResult.Succeeded())
	{
	QuitButton=UWidgetBlueprintLibrary::MakeBrushFromTexture(QuitResult.Object,16,16);
	}
	
}

TSharedRef<SWidget> UTitanWindow::RebuildWidget()
{

	Window=SNew(STitanWindow);


	// Add any existing content to the new slate box
	if ( GetChildrenCount() > 0 )
	{
		UPanelSlot* ContentSlot = GetContentSlot();
		if ( ContentSlot->Content )
		{
			Window->SetContent(ContentSlot->Content->TakeWidget());
		}
	}else
	{
		if ( IsDesignTime() )
		{
			Window->SetContent(
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromName(GetFName()))
				]
			);
		}
	}
	
	return Window.ToSharedRef();
}

void UTitanWindow::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	Window.Reset();
}


void UTitanWindow::SetBorderBackGround(FSlateBrush InBorderBackground)
{
	BorderBackground=InBorderBackground;
}

void UTitanWindow::SetBackGroundBorderColor(FSlateColor InBackGroundBorderColor)
{
	BackGroundBorderColor=InBackGroundBorderColor;
}

void UTitanWindow::SetBackGroundBorderColorHovered(FSlateColor InBackGroundBorderColorHovered)
{
	BackGroundBorderColorHovered=InBackGroundBorderColorHovered;
}

void UTitanWindow::SetTopBorder(FSlateBrush InBorderBackground)
{
	TopBorder=InBorderBackground;
}

void UTitanWindow::SetTopBorderColor(FSlateColor InBackGroundBorderColor)
{
	TopBorderColor=InBackGroundBorderColor;
}

void UTitanWindow::SetTopBorderColorHovered(FSlateColor InTopBorderColorHovered)
{
	TopBorderColorHovered=InTopBorderColorHovered;
}

void UTitanWindow::OnSlotAdded(UPanelSlot* InSlot)
{
	if(Window.IsValid()&&InSlot->Content)
	{
		Window->SetContent(InSlot->Content->TakeWidget());
	}
}

void UTitanWindow::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( Window.IsValid() )
	{
		Window->SetContent(SNullWidget::NullWidget);

		if ( IsDesignTime() )
		{
			Window->SetContent(
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromName(GetFName()))
				]
			);
			Window->TitanWindow=this;
		}
	}
}

void UTitanWindow::ProcessResize(FVector2D SizeMultiplier,FVector2D PositionMultiplier) const
{
	
const	FVector2D CorrectedSizeMultiplier=SizeMultiplier/UWidgetLayoutLibrary::GetViewportScale(GetWorld());
const	FVector2D CorrectedPositionMultiplier=PositionMultiplier/UWidgetLayoutLibrary::GetViewportScale(GetWorld());
auto SlotResult= Cast<UCanvasPanelSlot>(Slot);
	if(SlotResult)
	{
		FVector2D ResultSize=	SlotResult->GetSize()+CorrectedSizeMultiplier;
		if(ResultSize.X<MinimumSize.X)
		{
			ResultSize.X=MinimumSize.X;
		}
		if(ResultSize.Y<MinimumSize.Y)
		{
			ResultSize.Y=MinimumSize.Y;
		}
		SlotResult->SetSize(ResultSize);
		SlotResult->SetPosition(SlotResult->GetPosition()+CorrectedPositionMultiplier);
	}
	
}

void UTitanWindow::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	const TAttribute<const FSlateBrush*> BackGroundAttribute=&BorderBackground;
	const TAttribute<const FSlateBrush*> TopBorderBGAttribute=&TopBorder;
	const TAttribute<const FSlateBrush*> QuitButtonAttribute=&QuitButton;
	

	
	Window->SetBorderImage(BackGroundAttribute);
	Window->SetBorderBackgroundColor(BackGroundBorderColor,BackGroundBorderColorHovered);
	Window->SetTopBorderImage(TopBorderBGAttribute);
	Window->SetTopBorderBackgroundColor(TopBorderColor,TopBorderColorHovered);
	Window->SetBorderPadding(BackGroundBorderPadding);
	Window->SetQuitBorderImage(QuitButtonAttribute);
	Window->SetButtonStyle(&QuitButtonStyle);
	Window->CornerDetectionTolerance=CornerDetectionTolerance;
	Window->TitanWindow=this;
	Window->MyTextBlock->SetText(WindowText);
	Window->MyTextBlock->SetFont(WindowFont);
	
}


