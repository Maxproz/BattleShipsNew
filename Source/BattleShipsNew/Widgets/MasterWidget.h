// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API UMasterWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void AssignAnimations();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anims)
	TArray<class UWidgetAnimation*> WidgetAnimations;

	void ShowWidget()
	{
		SetVisibility(ESlateVisibility::Visible);
	}

	void HideWidget()
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	
	
};
