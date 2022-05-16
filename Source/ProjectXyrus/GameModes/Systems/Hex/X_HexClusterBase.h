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
	TSubclassOf<UX_HexBaseComponent> HexagonClass = UX_HexBaseComponent::StaticClass();

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

	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
	virtual bool SetupCluster(const FX_HexClusterSettings& NewSettings);

		
	UX_HexBaseComponent* GetHexagon_ByCubeCoordinate(const FX_HexCubeCoordinate& HexCoordinate);
	UX_HexBaseComponent* GetHexagon_ByGlobalCoordinate(const FVector& GlobalCoordinate);

protected:
	virtual bool GenerateClusterFromSettings();
	virtual bool GenerateCells();
	virtual bool GenerateHexagonsInCell(AX_HexClusterCellBase* ClusterCell, const FVector& ClusterLocation,
		const FX_HexOffsetCoordinate& FirstHexagonOffset, const FVector2d& HexParameters, const bool& HexagonStartFromTop);

	virtual bool ShouldGenerateHexagon(FX_HexOffsetCoordinate& HexOffsetCoordinate);

	virtual void ClearCluster();
};
