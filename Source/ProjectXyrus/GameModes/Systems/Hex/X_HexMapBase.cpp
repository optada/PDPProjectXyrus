// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_HexMapBase.h"

#include "Net/UnrealNetwork.h"


void AX_HexMapBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AX_HexMapBase, DeltaTest);
}
