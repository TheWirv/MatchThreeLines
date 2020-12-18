// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerController.h"
#include "MTLPlayerState.h"
#include "GameToken.h"
#include "MTLGameState.h"
#include "MTLSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// private functions
void AMTLPlayerController::OnMouseClicked()
{
    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();

    if (MTLPlayerState != nullptr)
    {
        AGameToken* CurrentlyHoveredGameToken = MTLPlayerState->GetHoveredOverGameToken();
        if (CurrentlyHoveredGameToken != nullptr)
        {
            MTLPlayerState->SetIsSelecting(true);
            MTLPlayerState->AddTokenToSelected(CurrentlyHoveredGameToken);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AMTLPlayerController::OnMouseReleased()
{
    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();

    if (MTLPlayerState != nullptr)
    {
        MTLPlayerState->EndTurn();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AMTLPlayerController::PauseGame()
{
    Pause();
    ChangeMenuWidget(PauseMenuWidget);
}

void AMTLPlayerController::ChangeMenuWidget(const TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);

        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AMTLPlayerController::SaveProfile()
{
    if (UMTLSaveGame* SaveGameInstance = Cast<UMTLSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UMTLSaveGame::StaticClass())))
    {
        const AMTLGameState* GameState = GetWorld()->GetGameState<AMTLGameState>();
        if (GameState != nullptr)
        {
            SaveGameInstance->PlayerName = PlayerState->GetPlayerName();
            SaveGameInstance->HighScore = GameState->GetHighScore();

            if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, DEFAULT_SAVE_SLOT_NAME, SaveGameInstance->UserIndex))
            {
                UE_LOG(LogTemp, Log, TEXT("Successfully save the game for player `%s`"), *PlayerState->GetPlayerName());
            }
            else 
            {
                GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Failed to save the game!"));
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Couldn't get GameState!"));
        }
    }
}

void AMTLPlayerController::LoadProfile()
{
    // Weirdly, the PlayerName is being populated with the computer's name, so reset it first
    PlayerState->SetPlayerName("");
    if (UMTLSaveGame* LoadedProfile = Cast<UMTLSaveGame>(UGameplayStatics::LoadGameFromSlot(DEFAULT_SAVE_SLOT_NAME, 0)))
    {
        AMTLGameState* GameState = GetWorld()->GetGameState<AMTLGameState>();
        if (GameState != nullptr)
        {
            PlayerState->SetPlayerName(LoadedProfile->PlayerName);
            GameState->SetHighScore(LoadedProfile->HighScore);
            UE_LOG(LogTemp, Log, TEXT("Loaded profile for Player `%s`"), *LoadedProfile->PlayerName);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Couldn't get GameState!"));
        }
    }
}

// public functions
AMTLPlayerController::AMTLPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AMTLPlayerController::EndGame()
{
    FTimerHandle DelayTimerHandle;
    GetWorldTimerManager().SetTimer(DelayTimerHandle, [this]()
    {
        Pause();
        ChangeMenuWidget(EndMenuWidget);
    }, 0.5f, false);
}

void AMTLPlayerController::StartOrResumeGame()
{
    ChangeMenuWidget(InGameWidget);
    SetPause(false);
}

void AMTLPlayerController::ResetGame() const
{
    AMTLGameState* GameState = GetWorld()->GetGameState<AMTLGameState>();
    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();

    if (GameState != nullptr && MTLPlayerState != nullptr)
    {
        GameState->ResetPlayingField();
        MTLPlayerState->Init();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Couldn't get GameState or PlayerState!"));
    }
}

void AMTLPlayerController::Quit()
{
    SaveProfile();
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

// protected functions
void AMTLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("MouseClick", IE_Pressed, this, &AMTLPlayerController::OnMouseClicked);
    InputComponent->BindAction("MouseClick", IE_Released, this, &AMTLPlayerController::OnMouseReleased);
    InputComponent->BindAction("Pause", IE_Pressed, this, &AMTLPlayerController::PauseGame);
}

void AMTLPlayerController::BeginPlay()
{
    Super::BeginPlay();

    Pause();

    LoadProfile();

    ChangeMenuWidget(MainMenuWidget);
}
