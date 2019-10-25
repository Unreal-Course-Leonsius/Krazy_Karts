// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "KrazyKartsGameMode.h"
#include "KrazyKartsPawn.h"
#include "KrazyKartsHud.h"

#include "Engine/World.h"



AKrazyKartsGameMode::AKrazyKartsGameMode()
{
	DefaultPawnClass = AKrazyKartsPawn::StaticClass();
	/*auto pawn = DefaultPawnClass.Get();
	auto Car = Cast<APawn>(pawn);
	Car.GetSpawnI*/
	HUDClass = AKrazyKartsHud::StaticClass();
}

/// MadDogX code https://community.gamedev.tv/t/solving-the-car-spawning-problem/48054
//APawn* AKrazyKartsGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
//{
//	UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnAtTransform_Implementation"));
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.Instigator = Instigator;
//	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
//	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
//	if (!ResultPawn)
//	{
//		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
//	}
//	return ResultPawn;
//}