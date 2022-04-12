// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexClusterCellBase.h"

#include "Net/UnrealNetwork.h"


void AX_HexClusterCellBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AX_HexClusterCellBase, Hexagons);
}

AX_HexClusterCellBase::AX_HexClusterCellBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
}

void AX_HexClusterCellBase::BeginPlay()
{
	SetReplicates(true);
	
	Super::BeginPlay();
}

void AX_HexClusterCellBase::BeginDestroy()
{
	ClearCell();
	
	Super::BeginDestroy();
}

bool AX_HexClusterCellBase::CreateHexagons(const int32 CountOfHexagons, TSubclassOf<UX_HexagonBaseComponent> HexagonClass)
{
	ClearCell(); // Clear this cell. It can be initialized before
	
	if (CountOfHexagons <= 0 || HexagonClass->IsValidLowLevelFast() == false)
	{
		return false;
	}

	// Create hexagons
	Hexagons.Reserve(CountOfHexagons); 
	for (int32 HexagonIterator = 0; HexagonIterator < CountOfHexagons; HexagonIterator++)
	{
		if (UX_HexagonBaseComponent* Hexagon = NewObject<UX_HexagonBaseComponent>(this, HexagonClass)) // TEXT("Hexagon")
		{
			if (Hexagon->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform) == false)
			{
				return false;	
			}
			
			Hexagon->RegisterComponent();
			Hexagons.Add(Hexagon);
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

int32 AX_HexClusterCellBase::GetCountOfHexagons() const
{
	return Hexagons.Num();
}

UX_HexagonBaseComponent* AX_HexClusterCellBase::GetHexagonByIndex(const int32 HexIndex)
{
	return Hexagons.IsValidIndex(HexIndex) ? Hexagons[HexIndex] : nullptr;
}

void AX_HexClusterCellBase::ClearCell()
{
	// Destroy all created hexagons
	for (auto* Hexagon : Hexagons)
	{
		if (IsValid(Hexagon))
		{
			Hexagon->DestroyComponent(true);
		}
	}
	Hexagons.Empty();
}
