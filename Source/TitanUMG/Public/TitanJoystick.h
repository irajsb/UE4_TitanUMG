// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "Templates/SharedPointer.h"
#include "CoreMinimal.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "STitanVirtualJoystick.h"
#include "TitanPanner.h"

#include "TitanJoystick.generated.h"

/**
 Wrapper for default UE4 Joysticks !
 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedWhenDisabledPanner);
UCLASS()
class TITANUMG_API UTitanJoystick : public UTitanPanner
{
	GENERATED_UCLASS_BODY()
	public:
	// basically mirroring SVirtualJoystick::FControlInfo but as an editable class
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="For sticks, this is the Thumb"))
	UTexture2D* Thumb;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The center point of the control (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D Center;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Control", meta=(ToolTip="For sticks, the size of the thumb (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D ThumbSize;
	UPROPERTY(EditAnywhere, Category="Control", meta=(ToolTip="The interactive size of the control (if <= 1.0, it's relative to screen, > 1.0 is absolute)"))
	FVector2D InteractionSize;
	
	/* Ragne:0-1 Recenter if touch is inside this box(Interactionsize*RecenterSize)*Prevents Center Going near widget ends* */
	UPROPERTY(EditAnywhere, Category="Control")
	float RecenterSize;
	
	

	

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


	/*Slider does not work as  expected just find effect you like and use it (Total 7 effects)*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Control")
	uint8 ThumbDrawEffect;



	/** The brush to use to draw the thumb for joysticks, or clicked for buttons */
	TSharedPtr< ISlateBrushSource > Image2;
	

	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	UFUNCTION(BlueprintCallable)
    void SetImage2(UTexture2D * New);
	
	virtual TSharedRef<SWidget> RebuildWidget()override;
	
	TSharedPtr<STitanVirtualJoystick> MyJoystick;
};
