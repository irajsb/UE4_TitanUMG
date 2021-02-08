// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanPannerMinimal.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#define LOCTEXT_NAMESPACE "UMG"


void UTitanPannerMinimal::SetImage1(UTexture2D* New)
{ Image1 = New  ? StaticCastSharedRef<ISlateBrushSource>(FDeferredCleanupSlateBrush::CreateBrush(New)) : TSharedPtr<ISlateBrushSource>();
}

void UTitanPannerMinimal::ReleaseSlateResources(bool bReleaseChildren)
{
Super::ReleaseSlateResources(bReleaseChildren);

   MyPanner.Reset();
    
}

UTitanPannerMinimal::UTitanPannerMinimal(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    InputScale=FVector2D(1,1);
    ActiveColor=FLinearColor::White;
    VisualCenter=FVector2D(0.5,0.5);
    VisualSize=FVector2D(1000,1000);
    ActiveColor=FLinearColor::White;
    DeActiveColor=FLinearColor(1,1,1,0.5);
}


void UTitanPannerMinimal::SetActiveColor(FLinearColor InActiveColor)
{ActiveColor=InActiveColor;
}



TSharedRef<SWidget> UTitanPannerMinimal::RebuildWidget()
{

 
 
 

    if (InputScale.SizeSquared() > FMath::Square(DELTA))
    {
  
    }

    MyPanner= SNew(STitanPannerMinimal);
    MyPanner->Owner=this;
   SetImage1(BackGround);
    return MyPanner.ToSharedRef();
}
const FText UTitanPannerMinimal::GetPaletteCategory()
{
    return LOCTEXT("Titan", "Titan");
}

void UTitanPannerMinimal::SetVisualSize(FVector2D InVisualSize)
{VisualSize=InVisualSize;
}

void UTitanPannerMinimal::SetDeActiveColor(FLinearColor InDeActiveColor)
{DeActiveColor=InDeActiveColor;
}
#undef LOCTEXT_NAMESPACE
