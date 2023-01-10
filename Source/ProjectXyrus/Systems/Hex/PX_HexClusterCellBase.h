// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_HexBaseComponent.h"

#include "PX_HexClusterCellBase.generated.h"


UCLASS()
class PROJECTXYRUS_API APX_HexClusterCellBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = HexagonClusterCell)
	TArray<UPX_HexBaseComponent*> Hexagons;
	
public:
	APX_HexClusterCellBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginDestroy() override;

	bool InitializeEmptyCell(const int32 CountOfHexagons);
	bool SetHexagonByIndex(const int32 HexIndex, UPX_HexBaseComponent* Hexagon);

	int32 GetCountOfHexagons() const;
	UPX_HexBaseComponent* GetHexagonByIndex(const int32 HexIndex);
	
protected:
	virtual void ClearCell();
};
