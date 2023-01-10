// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_HexBaseComponent.h"

#include "Net/UnrealNetwork.h"


void UPX_HexBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPX_HexBaseComponent, CubeCoordinate);
	DOREPLIFETIME(UPX_HexBaseComponent, OffsetCoordinate);
}

void UPX_HexBaseComponent::SetCoordinate(const FPX_HexCubeCoordinate& HexCubeCoordinate)
{
	CubeCoordinate = HexCubeCoordinate;
	UPX_HexSupportLib::ConvertCoordinate_Cube_To_Oddq(CubeCoordinate, OffsetCoordinate);
}

void UPX_HexBaseComponent::SetCoordinate(const FPX_HexOffsetCoordinate& HexOddqCoordinate)
{
	OffsetCoordinate = HexOddqCoordinate;
	UPX_HexSupportLib::ConvertCoordinate_Oddq_To_Cube(OffsetCoordinate, CubeCoordinate);
}

FPX_HexCubeCoordinate& UPX_HexBaseComponent::GetHexCubeCoordinate()
{
	return CubeCoordinate;
}

FPX_HexOffsetCoordinate& UPX_HexBaseComponent::GetHexOffsetCoordinate()
{
	return OffsetCoordinate;
}

void UPX_HexBaseComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetIsReplicated(true);
}
