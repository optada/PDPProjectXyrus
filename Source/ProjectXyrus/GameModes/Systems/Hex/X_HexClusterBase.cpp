// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexClusterBase.h"

#include "Net/UnrealNetwork.h"


DECLARE_LOG_CATEGORY_EXTERN(AX_HexClusterBase_LOG, Log, All);
DEFINE_LOG_CATEGORY(AX_HexClusterBase_LOG);


void AX_HexClusterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AX_HexClusterBase, ClusterSettings);
	DOREPLIFETIME(AX_HexClusterBase, ClusterCells);
}

AX_HexClusterBase::AX_HexClusterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
}

void AX_HexClusterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetReplicates(true);
}

void AX_HexClusterBase::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	if (GenerateOfFly)
	{
		if (GenerateClusterFromSettings())
		{
			UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("Hexagon cluster genereted on fly"));
		}
		else
		{
			UE_LOG(AX_HexClusterBase_LOG, Error, TEXT("Hexagon cluster generete failed"));
		}
		
	}
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}

bool AX_HexClusterBase::SetupCluster(const FX_HexClusterSettings& NewSettings)
{
	ClusterSettings = NewSettings;
	return true;
}

bool AX_HexClusterBase::GenerateClusterFromSettings()
{
	ClearCluster();

	return GenerateCells();
}

bool AX_HexClusterBase::GenerateCells()
{
	const FVector StartLocation = GetActorLocation();
	
	const FVector2d HexParameters = {
		2 * ClusterSettings.HexagonSize + ClusterSettings.HexagonShift, 
		FMath::Sqrt(static_cast<double>(3)) * ClusterSettings.HexagonSize + ClusterSettings.HexagonShift
	}; // X = Width | Y = Height

	const bool IsEvenCountHexagonsInCell = static_cast<bool>(ClusterSettings.HexagonClusterCellSize % 2);
	
	const FVector2d CellSize = {
		HexParameters.X * 0.75 * ClusterSettings.HexagonClusterCellSize,
		HexParameters.Y * ClusterSettings.HexagonClusterCellSize
	}; // X = Width | Y = Height

	if (ShowDebugLogOnGeneration)
	{
		UE_LOG(AX_HexClusterBase_LOG, Log, TEXT(" "));
		UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("----- DEBUG LOG FOR GENERATING HEXAGON CLUSTER -----"), HexParameters.X, HexParameters.Y);
		UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("HexParameters: | Width(X) = %f | Height(Y) = %f"), HexParameters.X, HexParameters.Y);
		UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("IsEvenCountHexagonsInCell: | %s"), (IsEvenCountHexagonsInCell ? TEXT("true") : TEXT("false")));
		UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("CellSize: | Width(X) = %f | Height(Y) = %f"), CellSize.X, CellSize.Y);
		UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("StartLocation: | %s"), *StartLocation.ToString());
	}
	
	// Create massive of cells "oddq vertical layout"
	ClusterCells.Reserve(ClusterSettings.ClusterCells_X * ClusterSettings.ClusterCells_Y);
	for (int32 CellIndex_X = 0; CellIndex_X < ClusterSettings.ClusterCells_X; CellIndex_X++)
	{
		for (int32 CellIndex_Y = 0; CellIndex_Y < ClusterSettings.ClusterCells_Y; CellIndex_Y++)
		{
			const int32 CellID = CellIndex_X + CellIndex_Y * ClusterSettings.ClusterCells_X;
			const bool HexagonsStartsFromTop = IsEvenCountHexagonsInCell ? !static_cast<bool>(CellIndex_X % 2) : true; // "oddq vertical layout" --> Shift lower on Y every second X Hexagon

			// Calculating cell size
			const FVector CellShift = {
				CellSize.X * CellIndex_X,
				CellSize.Y * CellIndex_Y,
				0
			};
			
			const FVector CellLocation = StartLocation + CellShift;
			const FRotator CellRotation = { 0, 0, 0 };
			
			FActorSpawnParameters CellSpawnParams;
			CellSpawnParams.bNoFail = true;
			CellSpawnParams.Owner = this;
			CellSpawnParams.OverrideLevel = GetLevel();

			// create cluster
			AX_HexClusterCellBase* ClusterCell = GetWorld()->SpawnActor<AX_HexClusterCellBase>(
				ClusterSettings.HexagonClusterCellClass,
				CellLocation,
				CellRotation,
				CellSpawnParams);
			
			if (ClusterCell == nullptr)
			{
				UE_LOG(AX_HexClusterBase_LOG, Error, TEXT("Failed creating ClusterCell | ID=%d"), CellID);
				return false;
			}
			
			ClusterCell->SetActorLabel("HexagonClusterCell ID = " + FString::FromInt(CellID));
			ClusterCells.Add(ClusterCell);
			
			if (ShowDebugLogOnGeneration)
			{
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT(" "));
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT(" --> "));
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("CREATED HexagonCell | X = %d | Y = %d | ID = %d"), CellIndex_X, CellIndex_Y, CellID);
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("HexagonsStartsFromTop: | %s"), (HexagonsStartsFromTop ? TEXT("true") : TEXT("false")));
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("CellShift: | X = %f | Y = %f"), CellShift.X, CellShift.Y);
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("CellLocation: | %s"), *CellLocation.ToString());
				UE_LOG(AX_HexClusterBase_LOG, Log, TEXT(" --:-- "));
			}
			
			if (GenerateHexagonsInCell(ClusterCell, CellLocation, HexParameters, HexagonsStartsFromTop) == false)
			{
				UE_LOG(AX_HexClusterBase_LOG, Error, TEXT("Failed create or setup hexagons in ClusterCell | ID = %d"), CellID);
				return false;
			}
		}	
	}

	return true;
}

bool AX_HexClusterBase::GenerateHexagonsInCell(AX_HexClusterCellBase* ClusterCell, const FVector& ClusterLocation,
	const FVector2d& HexParameters, const bool& HexagonStartFromTop)
{
	if (IsValid(ClusterCell) == false || ClusterSettings.HexagonClusterCellSize <= 0)
	{
		return false;
	}

	const int32 CountOfHexagonsInCell = ClusterSettings.HexagonClusterCellSize * ClusterSettings.HexagonClusterCellSize;
	if (ClusterCell->CreateHexagons(CountOfHexagonsInCell, ClusterSettings.HexagonClass) == false)
	{
		return false;
	}
	
	for (int32 HexIndex_X = 0; HexIndex_X < ClusterSettings.HexagonClusterCellSize; HexIndex_X++)
	{
		const bool HexagonShiftLower_Y = static_cast<bool>(HexIndex_X % 2) == HexagonStartFromTop; // if we start from top, not even hexagons will be lower
		for (int32 HexIndex_Y = 0; HexIndex_Y < ClusterSettings.HexagonClusterCellSize; HexIndex_Y++)
		{
			if (UX_HexBaseComponent* Hexagon = ClusterCell->GetHexagonByIndex(HexIndex_X + HexIndex_Y * ClusterSettings.HexagonClusterCellSize))
			{
				const FVector HexagonLocation = {
					HexIndex_X * HexParameters.X * 0.75,
					HexIndex_Y * HexParameters.Y + (HexagonShiftLower_Y ? HexParameters.Y / 2 : 0),
					ClusterLocation.Z
				};
				
				Hexagon->SetRelativeLocation(HexagonLocation);

				if (ShowDebugLogOnGeneration)
				{
					UE_LOG(AX_HexClusterBase_LOG, Log, TEXT(" "));
					UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("INITIALIZED Hexagon: | X = %d | Y = %d"), HexIndex_X, HexIndex_Y);
					UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("HexagonShiftLower_Y: | %s"), (HexagonShiftLower_Y ? TEXT("true") : TEXT("false")));
					UE_LOG(AX_HexClusterBase_LOG, Log, TEXT("HexagonLocation: | %s"), *HexagonLocation.ToString());
				}
			}
			else
			{
				return false;	
			}
		}
	}

	return true;
}

void AX_HexClusterBase::ClearCluster()
{
	// Destroy all created cells
	for (auto* ClusterCell : ClusterCells)
	{
		if (IsValid(ClusterCell))
		{
			ClusterCell->Destroy();
		}
	}
	ClusterCells.Empty();
}
