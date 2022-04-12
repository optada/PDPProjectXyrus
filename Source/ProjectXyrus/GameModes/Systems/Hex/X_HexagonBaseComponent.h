// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "X_HexagonBaseComponent.generated.h"


USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FX_HexCubeCoordinate
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Q; // Hex-Coordinate ("axial" and "Cube")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 R; // Hex-Coordinate ("axial" and "Cube")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 S; // Hex-Coordinate ("Cube")
};


UCLASS(Blueprintable)
class PROJECTXYRUS_API UX_HexagonBaseComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Hexagon)
	FX_HexCubeCoordinate Coordinate;

public:
	void SetHexCubeCoordinate(const FX_HexCubeCoordinate& HexCoordinate);
	void SetHexCubeCoordinate_Oddq(const int32 Col, const int32 Row);

	UFUNCTION(BlueprintCallable, Category = HexMath)
	FX_HexCubeCoordinate GetHexCubeCoordinate();
	UFUNCTION(BlueprintCallable, Category = HexMath)
	int32 GetCol_Oddq() const;
	UFUNCTION(BlueprintCallable, Category = HexMath)
	int32 GetRow_Oddq() const;

private:
	UFUNCTION()
	virtual void OnComponentCreated() override;
};
