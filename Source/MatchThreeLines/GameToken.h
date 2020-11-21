// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameToken.generated.h"

UCLASS()
class MATCHTHREELINES_API AGameToken : public AActor
{
    GENERATED_BODY()

    bool bIsSelected;

    FVector Location;
    TArray<int32> Index;

public:
    // Sets default values for this actor's properties
    AGameToken();


    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class UMaterial* Material;

    UPROPERTY(VisibleAnywhere)
    class UMaterialInstanceDynamic* MaterialInstance;

    enum ETokenTypes { Red, Blue, Green, Yellow, White };

    ETokenTypes TokenType;

    void AssignMaterialInstanceToMesh();

    FVector GetMaterialInstanceColorVector() const;

    void Init(TArray<int32> InIndex);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnBeginMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnEndMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnMouseClick(AActor* TouchedActor, FKey ButtonPressed);

    UFUNCTION()
    void OnMouseRelease(AActor* TouchedActor, FKey ButtonPressed);

    bool IsNeighbor(const AGameToken* Other);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

FORCEINLINE bool AGameToken::IsNeighbor(const AGameToken* Other)
{
    if (Other->Index[0] == Index[0])
    {
        if (Other->Index[1] - 1 == Index[1] || Other->Index[1] + 1 == Index[1])
        {
            // Same row, left/right
            return true;
        }
    }
    else if (Other->Index[1] == Index[1])
    {
        if (Other->Index[0] - 1 == Index[0] || Other->Index[0] + 1 == Index[0])
        {
            // Same column, above/below
            return true;
        }
    }
    else if (Other->Index[0] + 1 == Index[0])
    {
        if (Other->Index[1] - 1 == Index[1] || Other->Index[1] + 1 == Index[1])
        {
            // Since these are hexgons, these are bottom left/right
            return true;
        }
    }

    return false;
}
