// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanGoogleFontIcon.h"

#include <string>

#include "Containers/StringConv.h"


#include "Engine/Font.h"

UTitanGoogleFontIcon::UTitanGoogleFontIcon()
{
	const auto Result= ConstructorHelpers::FObjectFinder<UFont>(TEXT("/TitanUMG/Slate/Fonts/MaterialIcons-Regular_Font"));


	
	Text=FText::FromString("ea22");
	
	if(Result.Succeeded())
	{
		
	SetFont(	FSlateFontInfo( Result.Object,24));
	}
}

TAttribute<FText> UTitanGoogleFontIcon::GetDisplayText()
{

	FString String=	Text.ToString().ToUpper();
	char* _HexCode = TCHAR_TO_ANSI(*String);
	wchar_t wc = strtol(_HexCode, NULL, 16);
	FString UtfChar = "";
	UtfChar.AppendChar(wc);
	Output=UtfChar;
	return FText::FromString(UtfChar);
}

void UTitanGoogleFontIcon::OnCreationFromPalette()
{
	Text=FText::FromString("ea22");
}

