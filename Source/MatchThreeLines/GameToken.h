// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameToken.generated.h"

#define VERTICAL_OFFSET 86.5f
#define HORIZONTAL_OFFSET 75.f

enum ETokenType
{
    Red,
    Blue,
    Green,
    Yellow,
    White
};

UCLASS()
class MATCHTHREELINES_API AGameToken : public AActor
{
    GENERATED_BODY()

    bool bIsSelected;

    FVector Location;
    FIntPoint Index;

    ETokenType TokenType;

    void AssignMaterialInstanceToMesh();

    /**
    * Gets the color for this token's material instance.
    *
    * @returns This token's material instance color.
    */
    FColor GetMaterialInstanceColor() const
    {
        switch (TokenType)
        {
        case Red:
            return FColor::Red;
        case Blue:
            return FColor::Blue;
        case Green:
            return FColor::Green;
        case Yellow:
            return FColor::Yellow;
        default:
            return FColor::White;
        }
    }

public:
    // Sets default values for this actor's properties
    AGameToken();

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class UMaterial* Material;

    UPROPERTY(VisibleAnywhere)
    class UMaterialInstanceDynamic* MaterialInstance;

    void Init(const int32 Column, const int32 Row, const FVector InitialLocation);

    FIntPoint GetIndex() const;

    /**
    * Equality operator.
    *
    * @param OtherToken GameToken to compare.
    * @returns True if this GameToken has the same index as OtherToken. False otherwise.
    */
    bool operator==(const AGameToken* OtherToken) const
    {
        return Index.X == OtherToken->Index.X && Index.Y == OtherToken->Index.Y;
    }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnBeginMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnEndMouseOver(AActor* TouchedActor);

    bool IsNeighbor(const AGameToken* Other) const;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

FORCEINLINE FIntPoint AGameToken::GetIndex() const { return Index; };
