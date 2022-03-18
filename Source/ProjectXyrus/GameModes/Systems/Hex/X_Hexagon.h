// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "X_Hexagon.generated.h"


/*
 * Structure with base parameters of Hexagon
 * Used with UX_HexSystemComponent
 */
USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FX_Hexagon
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    float Size = 0.0; // Hex-Size (Radius)

    UPROPERTY(BlueprintReadOnly)
    int32 Q; // Hex-Coordinate ("axial" and "Cube")
    UPROPERTY(BlueprintReadOnly)
    int32 R; // Hex-Coordinate ("axial" and "Cube")
    UPROPERTY(BlueprintReadOnly)
    int32 S; // Hex-Coordinate ("Cube") 
    
public:
    void CreateHexagon(int32 New_Q, int32 New_R, int32 New_S)
    {       
        if ((New_Q + New_R + New_S) == 0)
        {
            Q = New_Q;
            R = New_R;
            S = New_S;
        }
        else
        {
            Q = 0;
            R = 0;
            S = 0;
        }
    }

    void CreateHexagon(int32 New_Q, int32 New_R)
    {
        Q = New_Q;
        R = New_R;
        S = -New_Q -New_R;
    }
};
