// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "X_HexSystemComponent.generated.h"


USTRUCT(BlueprintType)
struct PROJECTXYRUS_API FX_HexClaster
{
	GENERATED_BODY()

   
};


/*
 *
 */
UCLASS()
class PROJECTXYRUS_API UX_HexSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UX_HexSystemComponent();
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Создание, хранение и управление картой HEX
	// Обновление и передача данных о карте игрокам
	// математика связанная с поиском и навигацией по hex карте (отдельный static - free класс)
	
protected:
	virtual void BeginPlay() override;
};
