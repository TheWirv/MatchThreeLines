// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

#include "MTLPlayerController.generated.h"

/**
 * Custom PlayerController
 */
UCLASS()
class MATCHTHREELINES_API AMTLPlayerController : public APlayerController
{
    GENERATED_BODY()

    /** MouseDown event */
    void OnMouseClicked();

    /** MouseUp event */
    void OnMouseReleased();

    /** Pause the game and display the Pause menu */
    void PauseGame();

    /** Remove the current menu widget and create a new one from the specified class, if provided. */
    void ChangeMenuWidget(const TSubclassOf<UUserWidget> NewWidgetClass);

public:
    AMTLPlayerController();

    /** Wait for a tiny bit, then display the End menu */
    void EndGame();

    /** Hide the main/pause menu, display the in-game menu, then unpause the game */
    UFUNCTION(BlueprintCallable, Category = "MTL – UI")
    void StartOrResumeGame();

    /** Reset the playing field, score and remaining turns */
    UFUNCTION(BlueprintCallable, Category = "MTL – UI")
    void ResetGame() const;

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /** Called to bind functionality to input */
    virtual void SetupInputComponent() override;

    /** The main menu widget */
    UPROPERTY(EditDefaultsOnly, Category = "MTL – UI")
    TSubclassOf<UUserWidget> MainMenuWidget;

    /** The in-game menu widget */
    UPROPERTY(EditDefaultsOnly, Category = "MTL – UI")
    TSubclassOf<UUserWidget> InGameWidget;

    /** The Pause menu widget */
    UPROPERTY(EditDefaultsOnly, Category = "MTL – UI")
    TSubclassOf<UUserWidget> PauseMenuWidget;

    /** The End menu */
    UPROPERTY(EditDefaultsOnly, Category = "MTL – UI")
    TSubclassOf<UUserWidget> EndMenuWidget;

    /** The currently displayed UMG widget */
    UPROPERTY()
    UUserWidget* CurrentWidget;
};
