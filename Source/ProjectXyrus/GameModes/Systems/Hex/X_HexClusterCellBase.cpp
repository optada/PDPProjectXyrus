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

void AX_HexClusterCellBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetReplicates(true);
}

void AX_HexClusterCellBase::BeginDestroy()
{
	ClearCell();
	
	Super::BeginDestroy();
}

bool AX_HexClusterCellBase::InitializeEmptyCell(const int32 CountOfHexagons) 
{
	ClearCell(); // Clear this cell. It can be initialized before
	
	if (CountOfHexagons <= 0)
	{
		return false;
	}

	// Create empty hexagons
	Hexagons.Reserve(CountOfHexagons); 
	for (int32 HexagonIterator = 0; HexagonIterator < CountOfHexagons; HexagonIterator++)
	{
		Hexagons.Add(nullptr);
	}
	
	return true;
}

bool AX_HexClusterCellBase::SetHexagonByIndex(const int32 HexIndex, UX_HexBaseComponent* Hexagon)
{
	if (Hexagons.IsValidIndex(HexIndex) || IsValid(Hexagon))
	{
		Hexagons[HexIndex] = Hexagon;
		
		return true;
	}

	return false;
}

int32 AX_HexClusterCellBase::GetCountOfHexagons() const
{
	return Hexagons.Num();
}

UX_HexBaseComponent* AX_HexClusterCellBase::GetHexagonByIndex(const int32 HexIndex)
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
