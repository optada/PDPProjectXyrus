// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_PlayerStateGameplayBase.h"

#include "ProjectXyrus/GameModes/Systems/Hex/X_HexClusterBase.h"
#include "Net/UnrealNetwork.h"


DECLARE_LOG_CATEGORY_EXTERN(AX_PlayerStateGameplayBase_LOG, Log, All);
DEFINE_LOG_CATEGORY(AX_PlayerStateGameplayBase_LOG);


void AX_PlayerStateGameplayBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AX_PlayerStateGameplayBase, MyHexCluster);
}

void AX_PlayerStateGameplayBase::SetNewHexCluster(AX_HexClusterBase* NewHexCluster)
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

AX_HexClusterBase* AX_PlayerStateGameplayBase::GetMyHexCluster() const
{
	return MyHexCluster;
}
