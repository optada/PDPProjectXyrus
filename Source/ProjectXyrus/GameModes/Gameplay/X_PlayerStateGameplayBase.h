// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectXyrus/GameModes/X_PlayerStateBase.h"

#include "X_PlayerStateGameplayBase.generated.h"


class AX_HexClusterBase;


UCLASS()
class PROJECTXYRUS_API AX_PlayerStateGameplayBase : public AX_PlayerStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	AX_HexClusterBase* MyHexCluster = nullptr;

public:
	void SetNewHexCluster(AX_HexClusterBase* NewHexCluster);
	AX_HexClusterBase* GetMyHexCluster() const;
};
