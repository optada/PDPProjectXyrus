// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexagonBaseComponent.h"

#include "Net/UnrealNetwork.h"


void UX_HexagonBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UX_HexagonBaseComponent, Coordinate);
}

UX_HexagonBaseComponent::UX_HexagonBaseComponent()
{

}

void UX_HexagonBaseComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetIsReplicated(true);
}
