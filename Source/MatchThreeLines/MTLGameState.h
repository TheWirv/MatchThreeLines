// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "GameToken.h"
#include "MTLGameState.generated.h"

/** Structure that describes a GameToken that is falling down from outside the viewport, and how many units it is supposed to fall */
struct FFallingGameToken
{
    AGameToken* Token; // Pointer to the actual token
    int32 Amount; // How many units this token is supposed to fall down

    FFallingGameToken(AGameToken* InToken, const int32 InitialAmount): Token(InToken), Amount(InitialAmount)
    {
    }

    bool operator==(const FFallingGameToken& OtherFallingToken) const
    {
        return Token->GetIndex() == OtherFallingToken.Token->GetIndex();
    }
};

/** Structure that counts how many new GameTokens have to be spawned in one column */
struct FRespawningGameTokenCounter
{
    int32 ColumnIndex; // The column's index
    int32 Amount; // How many GameTokens have to be respawned

    FRespawningGameTokenCounter(const int32 InColumnIndex, const int32 InitialAmount): ColumnIndex(InColumnIndex),
        Amount(InitialAmount)
    {
    }

    bool operator==(const FRespawningGameTokenCounter& Other) const
    {
        return ColumnIndex == Other.ColumnIndex;
    }
};

/**
 * Structure that holds data about an high score entry, i.e.
 * - Player name
 * - Score
 * - Amount of turns needed
 * - Average score per turn
 */
USTRUCT(BlueprintType, Category = "MTL|High Score")
struct FHighScoreEntry
{
    GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MTL|High Score")
    FString PlayerName;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MTL|High Score")
    float Score;

    /** Amount of turns needed for this high score */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MTL|High Score")
    int32 AmountOfTurns;

    /** Average score per turn */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MTL|High Score")
    float AveragedScore;

    FHighScoreEntry()
    {
        PlayerName = FString("Player1");
        Score = 0.f;
        AmountOfTurns = 1;
        AveragedScore = 0.f;
    }
    
    FHighScoreEntry(const FString InPlayerName, const float InScore, const int32 InAmountOfTurns):
        PlayerName(InPlayerName), Score(InScore), AmountOfTurns(InAmountOfTurns)
    {
        AveragedScore = InScore / AmountOfTurns;
    }

    bool operator==(const FHighScoreEntry& Other) const
    {
        return AveragedScore == Other.AveragedScore;
    }

    bool operator<(const FHighScoreEntry& Other) const
    {
        return AveragedScore < Other.AveragedScore;
    }

    bool operator>(const FHighScoreEntry& Other) const
    {
        return AveragedScore > Other.AveragedScore;
    }
};

/**
 * Custom GameState
 * - Manages the playing field and the contained GameTokens
 */
UCLASS()
class MATCHTHREELINES_API AMTLGameState : public AGameStateBase
{
    GENERATED_BODY()

    TArray<TArray<AGameToken*>> PlayingField;

    /** Sets up the playing field's grid and spawns all the GameTokens */
    void InitPlayingField();

    /** Calculates the correct location vector a passed pair of column and row indeces */
    FVector CalculateGameTokenLocation(const int32 ColumnIndex, const int32 RowIndex) const;

    /**
     * Spawns a GameToken at the correct location, defined by its respective column and row indeces
     * @param ColumnIndex The GameToken's column index
     * @param RowIndex The GameToken's row index
     * @param bSpawnOutsideViewport Used during the game for respawning new GameTokens outside of the viewport (let them "fall down")
     */
    bool SpawnGameToken(const int32 ColumnIndex, const int32 RowIndex, const bool bSpawnOutsideViewport = false);

protected:
    virtual void BeginPlay() override;

public:
    AMTLGameState();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MTL|High Score")
    TArray<FHighScoreEntry> HighScores;

    /**
     * Destroys passed SelectedTokens, removes them from the PlayingField and respawns new ones at the top to fill up the grid again
     * @param SelectedTokens Tokens that are to be destroyed
     */
    bool DestroyTokens(TArray<AGameToken*> SelectedTokens);

    void ResetPlayingField();

    // Getters, setters and property-related delegates
    UFUNCTION(BlueprintCallable, Category = "MTL|UI")
    TArray<FHighScoreEntry> GetHighScores() const;

    FORCEINLINE void SetHighScores(const TArray<FHighScoreEntry>& InHighScores) { HighScores = InHighScores; };

    /**
     * Adds a high score to the (5 entries long) list of all-time high scores, and checks beforehand
     * whether it is actually a contender
     * @param InHighScore The high score to be added to the list
     * @returns The player's rank in the new list; -1 if they didn't make it to the Top 5
     */
    int32 AddHighScore(const float InHighScore);
};
