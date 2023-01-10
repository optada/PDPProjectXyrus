// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_HexSupportLib.h"

#include "PX_HexBaseComponent.generated.h"


UCLASS(Blueprintable)
class PROJECTXYRUS_API UPX_HexBaseComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Hexagon)
	FPX_HexCubeCoordinate CubeCoordinate;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Hexagon)
	FPX_HexOffsetCoordinate OffsetCoordinate;

public:
	void SetCoordinate(const FPX_HexCubeCoordinate& HexCubeCoordinate);
	void SetCoordinate(const FPX_HexOffsetCoordinate& HexOddqCoordinate);
	
	FPX_HexCubeCoordinate& GetHexCubeCoordinate();
	FPX_HexOffsetCoordinate& GetHexOffsetCoordinate();

private:
	UFUNCTION()
	virtual void OnComponentCreated() override;
};
