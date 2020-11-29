// Fill out your copyright notice in the Description page of Project Settings.

#include "GameToken.h"
#include "MTLPlayerState.h"
#include "MTLGameMode.h"

// private functions
void AGameToken::AssignMaterialInstanceToMesh()
{
    if (Material != nullptr)
    {
        MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
        MaterialInstance->SetVectorParameterValue("BaseColor", GetMaterialInstanceColor());
        Mesh->SetMaterial(0, MaterialInstance);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
                                         TEXT("Couldn't create MaterialInstance for SM_GameToken!"));
    }
}

// public functions
AGameToken::AGameToken()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bIsSelected = false;

    // Set up StaticMesh component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_GameToken"));
    Mesh->SetupAttachment(RootComponent);
    // Get the actual mesh asset
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    UStaticMesh* PlaneMesh = MeshAsset.Object;
    if (PlaneMesh != nullptr)
    {
        static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(
            TEXT("Material'/Game/Materials/M_GameToken.M_GameToken'"));
        Material = MaterialAsset.Object;
        if (Material != nullptr)
        {
            Mesh->SetStaticMesh(PlaneMesh);
            Mesh->SetMaterial(0, Material);
            const FRotator Rotation(0.f, 90.f, 90.f);
            Mesh->SetRelativeRotation(Rotation);
            this->OnBeginCursorOver.AddDynamic(this, &AGameToken::OnBeginMouseOver);
            this->OnEndCursorOver.AddDynamic(this, &AGameToken::OnEndMouseOver);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Couldn't get Material for SM_GameToken!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't get SM_GameToken!"));
    }

    int32 RandomNumber = FMath::RandRange(Red, White);
    TokenType = static_cast<ETokenType>(RandomNumber);
}

void AGameToken::Init(const int32 Column, const int32 Row, const FVector InitialLocation)
{
    Index = FIntPoint(Column, Row);
    Location = InitialLocation;
}

// protected functions
void AGameToken::OnBeginMouseOver(AActor* TouchedActor)
{
    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        PlayerState->SetHoveredOverGameToken(this);
        if (PlayerState->IsSelecting())
        {
            const AGameToken* LastToken = Cast<AGameToken>(PlayerState->GetSelectedTokens().Last());
            if (IsNeighbor(LastToken) && LastToken->TokenType == TokenType)
            {
                AGameToken* SelectedToken = Cast<AGameToken>(TouchedActor);
                if (SelectedToken != nullptr)
                {
                    PlayerState->AddTokenToSelected(SelectedToken);
                }
                else
                {
                    GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
                                                     TEXT("Couldn't cast touched actor to GameToken!"));
                }
            }
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AGameToken::OnEndMouseOver(AActor* TouchedActor)
{
    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        PlayerState->SetHoveredOverGameToken(nullptr);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

bool AGameToken::IsNeighbor(const AGameToken* Other) const
{
    // Column index to left/right is the same
    if (Index.X - 1 == Other->Index.X || Index.X + 1 == Other->Index.X)
    {
        // Same row
        if (Index.Y == Other->Index.Y ||
            // Check the next row for tokens with even column indeces
            ((Index.X % 2 == 0) && (Index.Y - 1 == Other->Index.Y)) ||
            // Check the previous row for tokens with odd column indeces
            ((Index.X % 2 == 1) && (Index.Y + 1 == Other->Index.Y)))
        {
            return true;
        }
    }
    else if ((Index.X == Other->Index.X) && (Index.Y - 1 == Other->Index.Y || Index.Y + 1 == Other->
        Index.Y))
    {
        // Column index is the same and the row index points either to the next or previous one
        return true;
    }

    return false;
}

// Called when the game starts or when spawned
void AGameToken::BeginPlay()
{
    Super::BeginPlay();

    AssignMaterialInstanceToMesh();
}

// Called every frame
void AGameToken::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        bIsSelected = PlayerState->GetSelectedTokens().Contains(this);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
                                         TEXT("Couldn't get PlayerState!"));
    }

    MaterialInstance->SetScalarParameterValue("IsSelected", bIsSelected ? 1 : 0);
}
