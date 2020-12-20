// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

#include "MTLPlayerController.generated.h"

UENUM(BlueprintType, Category = "MTL – UI")
enum EPauseWidgetContentToShow
{
    PauseMenu,
    EndMenu
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayFadeAnimationDelegate, bool, bFadeIn, EPauseWidgetContentToShow,
                                             PauseWidgetContentToShow);

/**
 * Custom PlayerController
 */
UCLASS()
class MATCHTHREELINES_API AMTLPlayerController : public APlayerController
{
    GENERATED_BODY()

    /** The Pause HUD widget */
    UPROPERTY(EditDefaultsOnly, Category = "MTL – UI")
    TSubclassOf<UUserWidget> PauseHUDWidget;

    /** The main in-game HUD widget */
    UPROPERTY(EditDefaultsOnly, Category = "MTL – UI")
    TSubclassOf<UUserWidget> MainHUDWidget;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    /** MouseDown event */
    void OnMouseClicked();

    /** MouseUp event */
    void OnMouseReleased();

    /** Action to be bound to Esc key */
    void PauseAction();

    /**
     * Either hides the in-game menu and shows the Pause HUD, or the other way round
     * @param bShow Whether to show the Pause HUD, and thus hiding the in-game menu;
     *              or hide the Pause HUD, which displays the in-game menu
     */
    void ShowPauseHUD(const bool bShow = true);

    /** Saves the player's progress, like their name and high score */
    void SaveProfile();

    /** Loads the player's prior progress, like their name and high score */
    void LoadProfile();

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /** Called to bind functionality to input. */
    virtual void SetupInputComponent() override;

public:
    AMTLPlayerController();

    /** Wait for a tiny bit, then display the End menu */
    void EndGame();

    /**
     * Pauses the game and displays the Pause HUD, or unpauses and shows main in-game HUD
     * @param bPause Whether to pause or unpause
     */
    UFUNCTION(BlueprintCallable, Category = "MTL – PlayerController")
    void PauseGame(const bool bPause);

    /** Hide the main/pause menu, display the in-game menu, then unpause the game */
    UFUNCTION(BlueprintCallable, Category = "MTL – UI")
    void StartOrResumeGame();

    /** Reset the playing field, score and remaining turns */
    UFUNCTION(BlueprintCallable, Category = "MTL – UI")
    void ResetGame() const;

    /** Quit the game, after having saved the profile */
    UFUNCTION(BlueprintCallable, Category = "MTL – UI")
    void Quit();

    /** Event dispatcher to let the UI know that it should play a fade in/out animation on the Pause HUD */
    UPROPERTY(BlueprintAssignable, Category = "MTL – UI")
    FPlayFadeAnimationDelegate OnPlayFadeAnimationDelegate;
};
