// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectXyrus/GameModes/PX_PlayerStateBase.h"

#include "PX_PlayerStateGameplayBase.generated.h"


class APX_HexClusterBase;


UCLASS()
class PROJECTXYRUS_API APX_PlayerStateGameplayBase : public APX_PlayerStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	APX_HexClusterBase* MyHexCluster = nullptr;

public:
	void SetNewHexCluster(APX_HexClusterBase* NewHexCluster);
	APX_HexClusterBase* GetMyHexCluster() const;
};
