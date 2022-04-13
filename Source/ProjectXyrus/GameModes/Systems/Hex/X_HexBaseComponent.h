// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "X_HexSupportLib.h"

#include "X_HexBaseComponent.generated.h"


UCLASS(Blueprintable)
class PROJECTXYRUS_API UX_HexBaseComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Hexagon)
	FX_HexCubeCoordinate CubeCoordinate;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Hexagon)
	FX_HexOffsetCoordinate OffsetCoordinate;

public:
	void SetCoordinate(const FX_HexCubeCoordinate& HexCubeCoordinate);
	void SetCoordinate(const FX_HexOffsetCoordinate& HexOddqCoordinate);
	
	FX_HexCubeCoordinate& GetHexCubeCoordinate();
	FX_HexOffsetCoordinate& GetHexOffsetCoordinate();

private:
	UFUNCTION()
	virtual void OnComponentCreated() override;
};
