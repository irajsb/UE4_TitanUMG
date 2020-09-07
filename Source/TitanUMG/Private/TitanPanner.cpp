// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanPanner.h"
#define LOCTEXT_NAMESPACE "UMG"

UTitanPanner::UTitanPanner(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
InputScale=FVector2D(1,1);
 ActiveColor=FLinearColor::White;
 VisualCenter=FVector2D(0.5,0.5);
 VisualSize=FVector2D(1000,1000);
 ActiveColor=FLinearColor::White;
 DeActiveColor=FLinearColor(1,1,1,0.5);
}


void UTitanPanner::SetActiveColor(FLinearColor InActiveColor)
{ActiveColor=InActiveColor;
}

TSharedRef<SWidget> UTitanPanner::RebuildWidget()
{

 
 
 

 if (InputScale.SizeSquared() > FMath::Square(DELTA))
 {
  
 }

MyPanner= SNew(STitanPanner);
MyPanner->Owner=this;
 MyPanner->Image1 = BackGround ? StaticCastSharedRef<ISlateBrushSource>(FDeferredCleanupSlateBrush::CreateBrush(BackGround)) : TSharedPtr<ISlateBrushSource>();
 return MyPanner.ToSharedRef();
}
const FText UTitanPanner::GetPaletteCategory()
{
 return LOCTEXT("Titan", "Titan");
}

void UTitanPanner::SetVisualSize(FVector2D InVisualSize)
{VisualSize=InVisualSize;
}

void UTitanPanner::SetDeActiveColor(FLinearColor InDeActiveColor)
{DeActiveColor=InDeActiveColor;
}
#undef LOCTEXT_NAMESPACE
