// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanPanner.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#define LOCTEXT_NAMESPACE "UMG"

UTitanPanner::UTitanPanner(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
InputScale=FVector2D(1,1);
 TextColor= ActiveColor=FLinearColor::White;
 VisualCenter=FVector2D(0.5,0.5);
 VisualSize=FVector2D(1000,1000);
 ActiveColor=FLinearColor::White;
 DeActiveColor=FLinearColor(1,1,1,0.5);
 TextToShow=FString("NULL");
 
}




void UTitanPanner::SimulateTouch(FVector2D in)
{
 MyPanner->SimulateTouch(in);
}

void UTitanPanner::SetTextColor(FLinearColor InTextColor)
{TextColor=InTextColor;
}

void UTitanPanner::ReleaseSlateResources(bool bReleaseChildren)
{
 Super::ReleaseSlateResources(bReleaseChildren);
 MyPanner.Reset();
}


TSharedRef<SWidget> UTitanPanner::RebuildWidget()
{

 

 

 if (InputScale.SizeSquared() > FMath::Square(DELTA))
 {
  
 }

MyPanner= SNew(STitanPanner);
MyPanner->Owner=this;
 SetImage1(BackGround);
 return MyPanner.ToSharedRef();
}





#undef LOCTEXT_NAMESPACE
