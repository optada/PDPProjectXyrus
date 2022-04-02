// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "X_HexClusterCellBase.h"
#include "GameFramework/Actor.h"

#include "X_HexClusterBase.generated.h"


USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FX_HexClusterSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AX_HexClusterCellBase> HexagonClusterCellClass = AX_HexClusterCellBase::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UX_HexagonBaseComponent> HexagonClass = UX_HexagonBaseComponent::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0, Delta = 1))
	int32 ClusterCells_X = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0, Delta = 1))
	int32 ClusterCells_Y = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0, Delta = 1))
	int32 HexagonClusterCellSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0, Delta = 1))
	float HexagonSize = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = 0, Delta = 0.5))
	float HexagonShift = 0.0;
};


/*
 *	This class should be created and initialized only on server
 *	On client this class using as map (gameplay map of hexagons) 
 */
UCLASS()
class PROJECTXYRUS_API AX_HexClusterBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexagonCluster)
	bool GenerateOfFly = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexagonCluster)
	bool ShowDebugLogOnGeneration = false;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = HexagonCluster)
	FX_HexClusterSettings ClusterSettings;

	UPROPERTY(Replicated)
	TArray<AX_HexClusterCellBase*> ClusterCells;
	
public:	
	AX_HexClusterBase();

	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
	virtual bool SetupCluster(const FX_HexClusterSettings& NewSettings);

protected:
	virtual bool GenerateClusterFromSettings();
	virtual bool GenerateCells();
	virtual bool GenerateHexagonsInCell(AX_HexClusterCellBase* ClusterCell, const FVector& ClusterLocation,
		const FVector2d& HexParameters, const bool& HexagonStartFromTop);

	virtual void ClearCluster();
	
	//AX_HexagonBase* GetHexagon_ByHexCoordinate(FX_HexCoordinate HexCoordinate);
	//AX_HexagonBase* GetHexagon_ByGlobalCoordinate(FVector GlobalCoordinate);

	// поиск соседних точек
	// Ориентировка по векторам направления
};
