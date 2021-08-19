// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ContentWidget.h"
#include "Framework/Styling/ButtonWidgetStyle.h"

#include "TitanWindow.generated.h"

/**
 * 
 */
class STitanWindow;
UCLASS()
class TITANUMG_API UTitanWindow : public UContentWidget
{
	GENERATED_UCLASS_BODY()


	

	virtual TSharedRef<SWidget> RebuildWidget()override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

	

	public:
	
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border)
	FText WindowText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border)
	FSlateFontInfo WindowFont;
	
	/** Brush to drag as the background */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border, meta=( DisplayName="Border Brush" ))
	FSlateBrush BorderBackground;
	
	UFUNCTION(BlueprintCallable)
	void SetBorderBackGround(FSlateBrush InBorderBackground);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border)
	FMargin BackGroundBorderPadding;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border, meta=( DisplayName="Border Animatable Color" ))
	FSlateColor BackGroundBorderColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border, meta=( DisplayName="Border Animatable Color" ))
	FSlateColor BackGroundBorderColorHovered;
	
	UFUNCTION(BlueprintCallable)
	void SetBackGroundBorderColor(FSlateColor InBackGroundBorderColor);
	UFUNCTION(BlueprintCallable)
	void SetBackGroundBorderColorHovered(FSlateColor InBackGroundBorderColorHovered);


	/** Brush to drag as the background */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border, meta=( DisplayName="Top Border Brush" ))
	FSlateBrush TopBorder;
	
	UFUNCTION(BlueprintCallable)
	void SetTopBorder(FSlateBrush InBorderBackground);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border, meta=( DisplayName="Top Border Animatable Color" ))
	FSlateColor TopBorderColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Border, meta=( DisplayName="Top Border Animatable Color" ))
	FSlateColor TopBorderColorHovered;
	
	UFUNCTION(BlueprintCallable)
	void SetTopBorderColor(FSlateColor InBackGroundBorderColor);
	UFUNCTION(BlueprintCallable)
	void SetTopBorderColorHovered(FSlateColor InTopBorderColorHovered);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Button, meta=( DisplayName="Quit Button Image " ))
	FSlateBrush QuitButton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Button)
	FButtonStyle QuitButtonStyle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Resize")
	float CornerDetectionTolerance=10;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Resize")
	FVector2D MinimumSize=FVector2D(200);


	void ProcessResize(FVector2D SizeMultiplier,FVector2D PositionMultiplier) const;
	
	


	//


	virtual void SynchronizeProperties() override;
	
	private:
	TSharedPtr<STitanWindow> Window;
};
