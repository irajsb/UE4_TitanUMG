// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanJoystick.h"
#define LOCTEXT_NAMESPACE "UMG"
UTitanJoystick::UTitanJoystick(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    ActiveColor = TextColor=FLinearColor(1,1,1,1);
    DeActiveColor = FLinearColor(1,1,1,0.2);
    TimeUntilDeactive = 0.5f;
    TimeUntilReset = 2.0f;
    ActivationDelay = 0.f;
VisualSize=FVector2D(1000,1000);
    ThumbSize=FVector2D(350   ,350);
    StartupDelay = 0.f;
    Center=FVector2D(0.5,0.5);
    InteractionSize=InputScale=FVector2D(1,1);
    Thumb=  Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Engine/MobileResources/HUD/VirtualJoystick_Thumb.VirtualJoystick_Thumb")));
    TextToShow=FString("NULL");
    BackGround=  Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Engine/MobileResources/HUD/VirtualJoystick_Background.VirtualJoystick_Background")));
}


void UTitanJoystick::SetVisualSize(FVector2D InVisualSize)
{VisualSize=InVisualSize;
    MyJoystick->bHasBeenPositioned=false;
    
}

void UTitanJoystick::SetThumbSize(FVector2D InThumbSize)
{
    ThumbSize=InThumbSize;
    MyJoystick->bHasBeenPositioned=false;
}

void UTitanJoystick::SetActiveColor(FLinearColor InActiveColor)
{ActiveColor=InActiveColor;
}

void UTitanJoystick::SetDeActiveColor(FLinearColor InDeActiveColor)
{DeActiveColor=InDeActiveColor;
}

void UTitanJoystick::SetTextColor(FLinearColor InTextColor)
{TextColor=InTextColor;
}

void UTitanJoystick::SetImage1(UTexture2D* New)
{
    Image1 = New  ? StaticCastSharedRef<ISlateBrushSource>(FDeferredCleanupSlateBrush::CreateBrush(New)) : TSharedPtr<ISlateBrushSource>();
}
void UTitanJoystick::SetImage2(UTexture2D* New)
{
    Image2 = New  ? StaticCastSharedRef<ISlateBrushSource>(FDeferredCleanupSlateBrush::CreateBrush(New)) : TSharedPtr<ISlateBrushSource>();
}

TSharedRef<SWidget> UTitanJoystick::RebuildWidget()
{

 
 
 
    SetImage1(Thumb);
    SetImage2(BackGround);

    if (InputScale.SizeSquared() > FMath::Square(DELTA))
    {
  
    }

    MyJoystick= SNew(STitanVirtualJoystick);
    MyJoystick->Owner=this;
    MyJoystick->CapturedPointerIndex=-1;
    if (StartupDelay > 0.f)
    {
        MyJoystick->SetWaitState();
    }
    return MyJoystick.ToSharedRef();
}

const FText UTitanJoystick::GetPaletteCategory()
{
    return LOCTEXT("Titan", "Titan");
}
#undef LOCTEXT_NAMESPACE
