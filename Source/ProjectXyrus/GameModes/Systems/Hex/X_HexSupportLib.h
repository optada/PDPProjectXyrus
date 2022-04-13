// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "X_HexSupportLib.generated.h"


USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FX_HexOffsetCoordinate
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Q; // Column
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 R; // Row
};


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


UCLASS()
class PROJECTXYRUS_API UX_HexSupportLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintPure, Category = "Hex Support Library")
	static void ConvertCoordinate_Cube_To_Oddq(const FX_HexCubeCoordinate& CubeCoordinate, FX_HexOffsetCoordinate& OddqCoordinate);
	
	UFUNCTION(BlueprintPure, Category = "Hex Support Library")
	static void ConvertCoordinate_Oddq_To_Cube(const FX_HexOffsetCoordinate& OddqCoordinate, FX_HexCubeCoordinate& CubeCoordinate);
};
