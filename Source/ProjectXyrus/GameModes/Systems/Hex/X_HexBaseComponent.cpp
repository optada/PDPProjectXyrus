// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexBaseComponent.h"

#include "Net/UnrealNetwork.h"


void UX_HexBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UX_HexBaseComponent, CubeCoordinate);
	DOREPLIFETIME(UX_HexBaseComponent, OffsetCoordinate);
}

void UX_HexBaseComponent::SetCoordinate(const FX_HexCubeCoordinate& HexCubeCoordinate)
{
	CubeCoordinate = HexCubeCoordinate;
	UX_HexSupportLib::ConvertCoordinate_Cube_To_Oddq(CubeCoordinate, OffsetCoordinate);
}

void UX_HexBaseComponent::SetCoordinate(const FX_HexOffsetCoordinate& HexOddqCoordinate)
{
	OffsetCoordinate = HexOddqCoordinate;
	UX_HexSupportLib::ConvertCoordinate_Oddq_To_Cube(OffsetCoordinate, CubeCoordinate);
}

FX_HexCubeCoordinate& UX_HexBaseComponent::GetHexCubeCoordinate()
{
	return CubeCoordinate;
}

FX_HexOffsetCoordinate& UX_HexBaseComponent::GetHexOffsetCoordinate()
{
	return OffsetCoordinate;
}

void UX_HexBaseComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetIsReplicated(true);
}
