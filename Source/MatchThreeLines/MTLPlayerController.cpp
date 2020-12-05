// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerController.h"
#include "MTLPlayerState.h"
#include "GameToken.h"

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
    }, 0.75f, false);
}

void AMTLPlayerController::StartOrResumeGame()
{
    ChangeMenuWidget(InGameWidget);
    SetPause(false);
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
    ChangeMenuWidget(MainMenuWidget);
}
