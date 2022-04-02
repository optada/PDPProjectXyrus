// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "X_HexagonBaseComponent.generated.h"


USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FX_HexCoordinate
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
	FX_HexCoordinate Coordinate;
	
public:
	UX_HexagonBaseComponent();

private:
	UFUNCTION()
	virtual void OnComponentCreated() override;
};
