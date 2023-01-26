// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "TitanGoogleFontIcon.generated.h"

/**
 * https://fonts.google.com/icons
 * select icon
 * copy codepoint to textbox
 */
UCLASS()
class TITANUMG_API UTitanGoogleFontIcon : public UTextBlock
{
	GENERATED_BODY()
	UTitanGoogleFontIcon();
	public:
	virtual TAttribute<FText> GetDisplayText() override;
#if WITH_EDITOR
	virtual void OnCreationFromPalette() override;
#endif
	
	//Character generated from codepoint  used if you want to used elsewhere
	UPROPERTY(EditAnywhere,Category=Content)
	FString Output;
	
};
