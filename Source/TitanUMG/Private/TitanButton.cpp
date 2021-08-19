// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanButton.h"

#include "Framework/Styling/ButtonWidgetStyle.h"

UTitanButton::UTitanButton()
{
	const auto Result= ConstructorHelpers::FObjectFinder<UTitanButtonStyle>(TEXT("/TitanUMG/Slate/DefaultButtonStyle"));
	if(Result.Succeeded())
	{
		TitanButtonStyle=Result.Object;
	}

}

TSharedRef<SWidget> UTitanButton::RebuildWidget()
{
	
	if(TitanButtonStyle)
	{
		WidgetStyle =TitanButtonStyle->ButtonStyle;
	}else
	{
		UE_LOG(LogTemp,Warning,TEXT("No Valid Button Widget style assigned on titan button"));
	}
return 	Super::RebuildWidget();
}
