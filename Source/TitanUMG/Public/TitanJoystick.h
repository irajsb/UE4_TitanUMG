// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ContentWidget.h"
#include "Templates/SharedPointer.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "STitanVirtualJoystick.h"
#include "TitanJoystick.generated.h"

/**
 Wrapper for default UE4 Joysticks !
 
 */
UCLASS()
class TITANUMG_API UTitanJoystick : public UWidget
{
	GENERATED_UCLASS_BODY()
	public:
	// basically mirroring SVirtualJoystick::FControlInfo but as an editable class
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="For sticks, this is the Thumb"))
	UTexture2D* Thumb;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="For sticks, this is the Background"))
	UTexture2D* BackGround;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The center point of the control (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D Center;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Control", meta=(ToolTip="The size of the control (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D VisualSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Control", meta=(ToolTip="For sticks, the size of the thumb (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D ThumbSize;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The interactive size of the control (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D InteractionSize;
	
	/* Ragne:0-1 Recenter if touch is inside this box(Interactionsize*RecenterSize)*Prevents Center Going near widget ends* */
	UPROPERTY(EditAnywhere, Category="Control")
	float RecenterSize;
	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "The scale for control input"))
	FVector2D InputScale;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The main input to send from this control (for sticks, this is the horizontal axis)"))
	FKey MainInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey AltInputKey;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The alternate input to send from this control (for sticks, this is the vertical axis)"))
	FKey PressInputKey;

	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Control", meta=(ToolTip="Color  of all controls while any control is active"))
	FLinearColor ActiveColor;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category="Control", meta=(ToolTip="Color  of all controls while no controls are active"))
	FLinearColor DeActiveColor;

	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="How long after user interaction will all controls fade out to Inactive Opacity"))
	float TimeUntilDeactive;

	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="How long after going inactive will controls reset/recenter themselves (0.0 will disable this feature)"))
	float TimeUntilReset;

	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="How long after joystick enabled for touch (0.0 will disable this feature)"))
	float ActivationDelay;

	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="Whether to prevent joystick re-center"))
	bool bPreventRecenter;

	UPROPERTY(EditAnywhere, Category = "Control", meta = (ToolTip = "Delay at startup before virtual joystick is drawn"))
	float StartupDelay;
	/** The brush to use to draw the background for joysticks, or unclicked for buttons */
	TSharedPtr< ISlateBrushSource > Image1;

	/** The brush to use to draw the thumb for joysticks, or clicked for buttons */
	TSharedPtr< ISlateBrushSource > Image2;
	
	
	//Needed for anim
	UFUNCTION(BlueprintCallable)
	void SetVisualSize(FVector2D InVisualSize);
	UFUNCTION(BlueprintCallable)
	void SetThumbSize(FVector2D InThumbSize);
	UFUNCTION(BlueprintCallable)
    void SetActiveColor(FLinearColor InActiveColor);
	UFUNCTION(BlueprintCallable)
    void SetDeActiveColor(FLinearColor InDeActiveColor);

	virtual TSharedRef<SWidget> RebuildWidget()override;
	const FText GetPaletteCategory();
	TSharedPtr<STitanVirtualJoystick> MyJoystick;
};
