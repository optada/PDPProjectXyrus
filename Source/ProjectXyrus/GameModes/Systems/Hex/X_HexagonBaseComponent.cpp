// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexagonBaseComponent.h"

#include "Net/UnrealNetwork.h"


void UX_HexagonBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UX_HexagonBaseComponent, Coordinate);
}

void UX_HexagonBaseComponent::SetHexCubeCoordinate(const FX_HexCubeCoordinate& HexCoordinate)
{
	Coordinate = HexCoordinate;
}

void UX_HexagonBaseComponent::SetHexCubeCoordinate_Oddq(const int32 Col, const int32 Row)
{
	Coordinate.Q = Col;
	Coordinate.R = Row - (Col - (Col & 1)) / 2;
	Coordinate.S = -Coordinate.Q -Coordinate.R;
}

FX_HexCubeCoordinate UX_HexagonBaseComponent::GetHexCubeCoordinate()
{
	return Coordinate;
}

int32 UX_HexagonBaseComponent::GetCol_Oddq() const
{
	return Coordinate.Q;
}

int32 UX_HexagonBaseComponent::GetRow_Oddq() const
{
	return (Coordinate.R + (Coordinate.Q - (Coordinate.Q & 1)) / 2);
}

void UX_HexagonBaseComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetIsReplicated(true);
}
