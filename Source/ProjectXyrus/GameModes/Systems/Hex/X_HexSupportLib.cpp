// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexSupportLib.h"


void UX_HexSupportLib::ConvertCoordinate_Cube_To_Oddq(const FX_HexCubeCoordinate& CubeCoordinate, FX_HexOffsetCoordinate& OddqCoordinate)
{
	OddqCoordinate.Q = CubeCoordinate.Q;
	OddqCoordinate.R = CubeCoordinate.R + (CubeCoordinate.Q - (CubeCoordinate.Q & 1)) / 2;
}

void UX_HexSupportLib::ConvertCoordinate_Oddq_To_Cube(const FX_HexOffsetCoordinate& OddqCoordinate, FX_HexCubeCoordinate& CubeCoordinate)
{
	CubeCoordinate.Q = OddqCoordinate.Q;
	CubeCoordinate.R = OddqCoordinate.R - (OddqCoordinate.Q - (OddqCoordinate.Q & 1)) / 2;
	CubeCoordinate.S = -CubeCoordinate.Q - CubeCoordinate.R;
}
