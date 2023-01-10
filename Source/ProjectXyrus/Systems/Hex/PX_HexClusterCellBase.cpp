// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_HexClusterCellBase.h"

#include "Net/UnrealNetwork.h"


void APX_HexClusterCellBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APX_HexClusterCellBase, Hexagons);
}

APX_HexClusterCellBase::APX_HexClusterCellBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
}

void APX_HexClusterCellBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetReplicates(true);
}

void APX_HexClusterCellBase::BeginDestroy()
{
	ClearCell();
	
	Super::BeginDestroy();
}

bool APX_HexClusterCellBase::InitializeEmptyCell(const int32 CountOfHexagons) 
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

bool APX_HexClusterCellBase::SetHexagonByIndex(const int32 HexIndex, UPX_HexBaseComponent* Hexagon)
{
	if (Hexagons.IsValidIndex(HexIndex) || IsValid(Hexagon))
	{
		Hexagons[HexIndex] = Hexagon;
		
		return true;
	}

	return false;
}

int32 APX_HexClusterCellBase::GetCountOfHexagons() const
{
	return Hexagons.Num();
}

UPX_HexBaseComponent* APX_HexClusterCellBase::GetHexagonByIndex(const int32 HexIndex)
{
	return Hexagons.IsValidIndex(HexIndex) ? Hexagons[HexIndex] : nullptr;
}

void APX_HexClusterCellBase::ClearCell()
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
