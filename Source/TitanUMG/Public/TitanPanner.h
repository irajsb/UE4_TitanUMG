// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ContentWidget.h"
#include "Templates/SharedPointer.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "STitanPanner.h"
#include "TitanPanner.generated.h"

/**
 Used for camera panning and for buttons that need to control Camera (Shoot button )
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedWhenDisabled);
UCLASS()
class TITANUMG_API UTitanPanner : public UWidget
{
	GENERATED_UCLASS_BODY()
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Control", meta=(ToolTip="The size of the control (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D VisualSize;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="For sticks, this is the Background"))
	UTexture2D* BackGround;
	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "The scale for control input"))
	FVector2D InputScale;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The main input to send from this control (for sticks, this is the horizontal axis)"))
	FKey MainInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey AltInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey PressInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey PinchInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey PinchRotateInputKey;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Control", meta=(ToolTip="Color  of all controls while any control is active"))
	FLinearColor ActiveColor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category="Control", meta=(ToolTip="Color  of all controls while no controls are active"))
	FLinearColor DeActiveColor;
	/*Slider does not work as  expected just find effect you like and use it (Total 7 effects)*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Control")
	uint8 BackGroundDrawEffect;
	
	FVector2D VisualCenter=FVector2D(0.5,0.5);
	/** Called when the button is clicked When Disabled */
	UPROPERTY(BlueprintAssignable, Category="Joystick|Event")
	FOnClickedWhenDisabled OnClickedWhenDisabled;

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
	//set to disable manually Will fire an event if player tries to activate it Warning:Do not disable when joystick is touched 
	UPROPERTY(BlueprintReadWrite)
	bool bIsDisabled;
	
	virtual TSharedRef<SWidget> RebuildWidget()override;
	const FText GetPaletteCategory();
	TSharedPtr<STitanPanner> MyPanner;
	UFUNCTION(BlueprintCallable)
    void SetVisualSize(FVector2D InVisualSize);
	UFUNCTION(BlueprintCallable)
    void SetDeActiveColor(FLinearColor InDeActiveColor);
	UFUNCTION(BlueprintCallable)
    void SetActiveColor(FLinearColor InActiveColor);
	UFUNCTION(BlueprintCallable)
	void SimulateTouch(FVector2D in);
	UFUNCTION(BlueprintCallable)
    void SetTextColor(FLinearColor InTextColor);
	UFUNCTION(BlueprintCallable)
    void SetImage1(UTexture2D * New);
};
