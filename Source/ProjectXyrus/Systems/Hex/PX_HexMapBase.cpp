// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_HexMapBase.h"

#include "Net/UnrealNetwork.h"


void APX_HexMapBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APX_HexMapBase, DeltaTest);
}
