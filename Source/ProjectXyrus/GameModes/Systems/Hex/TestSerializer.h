// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "X_HexSupportLib.h"

#include "TestSerializer.generated.h"


/** Step 1: Make your struct inherit from FFastArraySerializerItem */
USTRUCT()
struct FExampleItemEntry : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY()
 
	// Your data:
	UPROPERTY()
	int32 ExampleIntProperty;	
 
	UPROPERTY()
	float ExampleFloatProperty;

	UPROPERTY()
	FX_HexCubeCoordinate ExampleHexCubeCoordinateProperty;
 
	/** 
	 * Optional functions you can implement for client side notification of changes to items; 
	 * Parameter type can match the type passed as the 2nd template parameter in associated call to FastArrayDeltaSerialize
	 * 
	 * NOTE: It is not safe to modify the contents of the array serializer within these functions, nor to rely on the contents of the array 
	 * being entirely up-to-date as these functions are called on items individually as they are updated, and so may be called in the middle of a mass update.
	 */
	// void PreReplicatedRemove(const struct FExampleArray& InArraySerializer);
	// void PostReplicatedAdd(const struct FExampleArray& InArraySerializer);
	// void PostReplicatedChange(const struct FExampleArray& InArraySerializer);
};


/** Step 2: You MUST wrap your TArray in another struct that inherits from FFastArraySerializer */
USTRUCT()
struct FTestedArray: public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FExampleItemEntry> Items;	/** Step 3: You MUST have a TArray named Items of the struct you made in step 1. */
 
	/** Step 4: Copy this, replace example with your names */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
	   return FFastArraySerializer::FastArrayDeltaSerialize<FExampleItemEntry, FTestedArray>(Items, DeltaParams, *this);
	}
};


/** Step 5: Copy and paste this struct trait, replacing FExampleArray with your Step 2 struct. */
template<>
struct TStructOpsTypeTraits<FTestedArray> : public TStructOpsTypeTraitsBase2<FTestedArray>
{
	enum 
	{
		WithNetDeltaSerializer = true,
	};
};

 
/** Step 6 and beyond: 
 *		-Declare a UPROPERTY of your FExampleArray (step 2) type.
 *		-You MUST call MarkItemDirty on the FExampleArray when you change an item in the array. You pass in a reference to the item you dirtied. 
 *			See FFastArraySerializer::MarkItemDirty.
 *		-You MUST call MarkArrayDirty on the FExampleArray if you remove something from the array.
 *		-In your classes GetLifetimeReplicatedProps, use DOREPLIFETIME(YourClass, YourArrayStructPropertyName);
 *
 *		You can override the following virtual functions in your structure (step 1) to get notifies before add/deletes/removes:
 *			-void PreReplicatedRemove(const FFastArraySerializer& Serializer)
 *			-void PostReplicatedAdd(const FFastArraySerializer& Serializer)
 *			-void PostReplicatedChange(const FFastArraySerializer& Serializer)
 *
 *		Thats it!
 */
