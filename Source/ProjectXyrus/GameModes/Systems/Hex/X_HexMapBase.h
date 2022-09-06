// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestSerializer.h"

#include "X_HexMapBase.generated.h"


UCLASS()
class PROJECTXYRUS_API AX_HexMapBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	FTestedArray DeltaTest;

public:
	// Adding an element to the array
	void AddItem()
	{
		FExampleItemEntry a;
		a.ExampleFloatProperty = 3.14;
		a.ExampleIntProperty = 1234;		
		DeltaTest.MarkItemDirty(DeltaTest.Items.Add_GetRef(a));
	}
	
	// Modifying an element
	void ChangeItem(int32 ItemID)
	{
		if (DeltaTest.Items.Num() > ItemID)
		{
			DeltaTest.Items[ItemID].ExampleFloatProperty = 6.28;
			DeltaTest.Items[ItemID].ExampleIntProperty = 5678;
			DeltaTest.MarkItemDirty(DeltaTest.Items[ItemID]);
		}
	}
	
	// Removing an element
	void RemoveLastItem()
	{
		if (DeltaTest.Items.Num() > 0)
		{
			DeltaTest.Items.RemoveAt(DeltaTest.Items.Num()-1, 1);
			DeltaTest.MarkArrayDirty();
		}
	}
};

