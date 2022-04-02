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

bool AX_HexClusterCellBase::CreateHexagons(int ClusterCellSideSize, TSubclassOf<UX_HexagonBaseComponent> HexagonClass)
{
	ClearCell(); // Clear this cell. It can be initialized before
	
	if (ClusterCellSideSize <= 0 || HexagonClass->IsValidLowLevelFast() == false)
	{
		return false;
	}

	// Create hexagons
	const int32 CountOfHexagons = ClusterCellSideSize * ClusterCellSideSize; 
	Hexagons.Reserve(CountOfHexagons); 
	for (int HexagonIterator = 0; HexagonIterator < CountOfHexagons; HexagonIterator++)
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
