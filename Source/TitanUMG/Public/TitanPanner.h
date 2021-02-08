// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ContentWidget.h"
#include "Templates/SharedPointer.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "STitanPanner.h"
#include "TitanPannerMinimal.h"

#include "TitanPanner.generated.h"

/**
 Used for camera panning and for buttons that need to control Camera (Shoot button )
 */

UCLASS()
class TITANUMG_API UTitanPanner : public UTitanPannerMinimal
{
	GENERATED_UCLASS_BODY()
	public:
	
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey PinchInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey PinchRotateInputKey;
	

	/*Slider does not work as  expected just find effect you like and use it (Total 7 effects)*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Control")
	uint8 BackGroundDrawEffect;
	



	/*Slider does not work as  expected just find effect you like and use it (Total 7 effects)*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Text")
	uint8 TextDrawEffect;
	UPROPERTY(EditAnywhere,Category="Text")
	FSlateFontInfo FontInfo;
	UPROPERTY(EditAnywhere,Category="Text")
	FVector2D FontCenterCorrection;
	//Set to NULL to disable 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Text")
	FString TextToShow;
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category="Text", meta=(ToolTip="Color  of Text"))
	FLinearColor TextColor;
	
	
	virtual TSharedRef<SWidget> RebuildWidget()override;

	TSharedPtr<STitanPanner> MyPanner;
	
	UFUNCTION(BlueprintCallable)
	void SimulateTouch(FVector2D in);
	UFUNCTION(BlueprintCallable)
    void SetTextColor(FLinearColor InTextColor);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
};
