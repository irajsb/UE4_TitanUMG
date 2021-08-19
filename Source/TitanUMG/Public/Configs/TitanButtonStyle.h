// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TitanButtonStyle.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class TITANUMG_API UTitanButtonStyle : public UDataAsset
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FButtonStyle ButtonStyle;
};
