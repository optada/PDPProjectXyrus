// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "X_HexBaseComponent.h"

#include "X_HexClusterCellBase.generated.h"


UCLASS()
class PROJECTXYRUS_API AX_HexClusterCellBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = HexagonClusterCell)
	TArray<UX_HexBaseComponent*> Hexagons;
	
public:
	AX_HexClusterCellBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginDestroy() override;

	bool CreateHexagons(const int32 CountOfHexagons, TSubclassOf<UX_HexBaseComponent> HexagonClass);

	int32 GetCountOfHexagons() const;
	UX_HexBaseComponent* GetHexagonByIndex(const int32 HexIndex);
	
protected:
	virtual void ClearCell();
};
