// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_PlayerStateGameplayBase.h"

#include "ProjectXyrus/Systems/Hex/PX_HexClusterBase.h"
#include "Net/UnrealNetwork.h"


DECLARE_LOG_CATEGORY_EXTERN(AX_PlayerStateGameplayBase_LOG, Log, All);
DEFINE_LOG_CATEGORY(AX_PlayerStateGameplayBase_LOG);


void APX_PlayerStateGameplayBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APX_PlayerStateGameplayBase, MyHexCluster);
}

void APX_PlayerStateGameplayBase::SetNewHexCluster(APX_HexClusterBase* NewHexCluster)
{
	if (HasAuthority() == false || IsValid(NewHexCluster) == false)
	{
		UE_LOG(AX_PlayerStateGameplayBase_LOG, Log, TEXT("Changing MyHexCluster failed -> PlayerName %s | HasAuthority = %s"),
			*GetPlayerName(), (HasAuthority() ? TEXT("true") : TEXT("false")));
		return;
	}
	
	MyHexCluster = NewHexCluster;
	UE_LOG(AX_PlayerStateGameplayBase_LOG, Log, TEXT("Changed MyHexCluster -> PlayerName %s"), *GetPlayerName());
}

APX_HexClusterBase* APX_PlayerStateGameplayBase::GetMyHexCluster() const
{
	return MyHexCluster;
}
