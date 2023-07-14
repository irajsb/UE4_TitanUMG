// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanGoogleFontIcon.h"

#include <string>

#include "Containers/StringConv.h"


#include "Engine/Font.h"

UTitanGoogleFontIcon::UTitanGoogleFontIcon()
{
	if (!IsRunningDedicatedServer())
	{
		const auto Result = ConstructorHelpers::FObjectFinder<UFont>(
			TEXT("/TitanUMG/Slate/Fonts/MaterialIcons-Regular_Font"));


		Text = FText::FromString("ea22");

		if (Result.Succeeded())
		{
			SetFont(FSlateFontInfo(Result.Object, 24));
		}
	}
}

TAttribute<FText> UTitanGoogleFontIcon::GetDisplayText()
{
	const FString String = Text.ToString().ToUpper();
	const char* HexCode = TCHAR_TO_ANSI(*String);
	const wchar_t WChar = strtol(HexCode, nullptr, 16);
	FString UtfChar = "";
	UtfChar.AppendChar(WChar);
	Output = UtfChar;
	return FText::FromString(UtfChar);
}

#if WITH_EDITOR
void UTitanGoogleFontIcon::OnCreationFromPalette()
{
	Text = FText::FromString("ea22");
}
#endif
