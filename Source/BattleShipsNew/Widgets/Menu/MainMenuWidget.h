// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MasterWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API UMainMenuWidget : public UMasterWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedPlayButton();

	UFUNCTION()
	void OnClickedSettingsButton();

	UFUNCTION()
	void OnClickedQuitButton();

	UPROPERTY(EditDefaultsOnly)
	FString GameMapName;

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	
};
