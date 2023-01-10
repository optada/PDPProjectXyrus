// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_HexSupportLib.h"


void UPX_HexSupportLib::ConvertCoordinate_Cube_To_Oddq(const FPX_HexCubeCoordinate& CubeCoordinate, FPX_HexOffsetCoordinate& OddqCoordinate)
{
	OddqCoordinate.Q = CubeCoordinate.Q;
	OddqCoordinate.R = CubeCoordinate.R + (CubeCoordinate.Q - (CubeCoordinate.Q & 1)) / 2;
}

void UPX_HexSupportLib::ConvertCoordinate_Oddq_To_Cube(const FPX_HexOffsetCoordinate& OddqCoordinate, FPX_HexCubeCoordinate& CubeCoordinate)
{
	CubeCoordinate.Q = OddqCoordinate.Q;
	CubeCoordinate.R = OddqCoordinate.R - (OddqCoordinate.Q - (OddqCoordinate.Q & 1)) / 2;
	CubeCoordinate.S = -CubeCoordinate.Q - CubeCoordinate.R;
}
