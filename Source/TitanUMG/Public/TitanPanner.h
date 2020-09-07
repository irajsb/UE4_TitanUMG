// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ContentWidget.h"
#include "Templates/SharedPointer.h"
#include "STitanVirtualJoystick.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "STitanPanner.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "TitanPanner.generated.h"

/**
 * 
 */
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
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Control", meta=(ToolTip="Color  of all controls while any control is active"))
	FLinearColor ActiveColor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category="Control", meta=(ToolTip="Color  of all controls while no controls are active"))
	FLinearColor DeActiveColor;
	
	FVector2D VisualCenter=FVector2D(0.5,0.5);
	
	
	virtual TSharedRef<SWidget> RebuildWidget()override;
	const FText GetPaletteCategory();
	TSharedPtr<STitanPanner> MyPanner;
	UFUNCTION(BlueprintCallable)
    void SetVisualSize(FVector2D InVisualSize);
	UFUNCTION(BlueprintCallable)
    void SetDeActiveColor(FLinearColor InDeActiveColor);
	UFUNCTION(BlueprintCallable)
    void SetActiveColor(FLinearColor InActiveColor);
};
