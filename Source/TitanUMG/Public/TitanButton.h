// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Configs/TitanButtonStyle.h"

#include "TitanButton.generated.h"

/**
 * 
 */
UCLASS()
class TITANUMG_API UTitanButton : public UButton
{
	GENERATED_BODY()


	public:
	UTitanButton();
	//Optional will replace current style if valid  : To Create one right click in content browser->Misc->DataAsset->TitanButton
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTitanButtonStyle* TitanButtonStyle;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
};
