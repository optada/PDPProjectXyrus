// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_HexClusterCellBase.h"
#include "GameFramework/Actor.h"

#include "PX_HexClusterBase.generated.h"


USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FPX_HexClusterSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APX_HexClusterCellBase> HexagonClusterCellClass = APX_HexClusterCellBase::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPX_HexBaseComponent> HexagonClass = UPX_HexBaseComponent::StaticClass();

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
class PROJECTXYRUS_API APX_HexClusterBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexagonCluster)
	bool GenerateOfFly = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HexagonCluster)
	bool ShowDebugLogOnGeneration = false;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = HexagonCluster)
	FPX_HexClusterSettings ClusterSettings;

	UPROPERTY(Replicated)
	TArray<APX_HexClusterCellBase*> ClusterCells;
	
public:	
	APX_HexClusterBase();

	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
	virtual bool SetupCluster(const FPX_HexClusterSettings& NewSettings);

		
	UPX_HexBaseComponent* GetHexagon_ByCubeCoordinate(const FPX_HexCubeCoordinate& HexCoordinate);
	UPX_HexBaseComponent* GetHexagon_ByGlobalCoordinate(const FVector& GlobalCoordinate);

protected:
	virtual bool GenerateClusterFromSettings();
	virtual bool GenerateCells();
	virtual bool GenerateHexagonsInCell(APX_HexClusterCellBase* ClusterCell, const FVector& ClusterLocation,
		const FPX_HexOffsetCoordinate& FirstHexagonOffset, const FVector2d& HexParameters, const bool& HexagonStartFromTop);

	virtual bool ShouldGenerateHexagon(FPX_HexOffsetCoordinate& HexOffsetCoordinate);

	virtual void ClearCluster();
};
