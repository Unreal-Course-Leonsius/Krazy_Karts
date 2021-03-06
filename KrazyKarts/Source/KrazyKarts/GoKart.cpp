// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"


#define OUT

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bReplicateMovement = false;

	MovementComponent = CreateDefaultSubobject<UGoKartMovementComponent>(TEXT("MovementComponent"));
	MovementReplicator = CreateDefaultSubobject<UGoKartMovementReplicator>(TEXT("MovementReplicator"));

}

void AGoKart::Initialize(USceneComponent * BP_scene, USpringArmComponent * BP_springarm, UCameraComponent* BP_camera, UBoxComponent* BP_Collision)
{
	/* Get Components */
	Collision = BP_Collision;
	Scene = BP_scene;
	SpringArm = BP_springarm;
	Camera = BP_camera;

	Setting();
}

void AGoKart::Setting()
{
	Scene->bAbsoluteRotation = false; // Rotation of the ball should not affect rotation of boom
	//SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = false; // must not be bAbsolutRotation = true because Scene Component work
	//RootComponent = Collision;
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, TEXT("ActorLocation = %s"), *GetActorLocation().ToString());

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}

	//GameState = GetWorld()->GetGameState();

	/*if (HasAuthority())
	{
		UEngine* engine = GetGameInstance()->GetEngine();
		engine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("This is a Server"));
	}*/
	
}

/// we don't have here class space and this fucntion is not class member it outside of class
FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}


// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (IsLocallyControlled())
	{
		UEngine* engine = GetGameInstance()->GetEngine();
		engine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("This is Only Client"));
	}*/

	/*FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	DrawDebugLine
	(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (GetActorForwardVector() * 1000),
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		5.f
	);
*/


	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);

	/*if (Role == ROLE_SimulatedProxy)
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 200), GetEnumText(Role), this, FColor::White, DeltaTime);
	}*/
	
}




// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AGoKart::Elevation);
	PlayerInputComponent->BindAxis("LookRight", this, &AGoKart::Azimuth);

}



void AGoKart::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetThrottle(Value);

}


void AGoKart::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(Value);


}


void AGoKart::Azimuth(float Val)
{
	
	auto val = FMath::Clamp(Val, -1.f, +1.f);
	auto yaw = val * GetWorld()->DeltaTimeSeconds * 80;

	//UE_LOG(LogTemp, Warning, TEXT("YawAdd = %f"), yaw);
	
	auto yawChange = Scene->RelativeRotation.Yaw + yaw;
	auto Newyaw = FMath::Clamp<float>(yawChange, -50, 50);
	auto NewRotation = FRotator(Scene->GetComponentRotation().Pitch, Newyaw, 0.f);
	Scene->SetRelativeRotation(NewRotation);
	//Scene->AddRelativeRotation(NewRotation);
	//Scene->AddLocalRotation(NewRotation);
	//Scene->SetWorldRotation(NewRotation);

	//UE_LOG(LogTemp, Warning, TEXT("SceneRotaton = %s"), *Scene->GetComponentRotation().ToString());
	//UE_LOG(LogTemp, Error, TEXT("SceneRelativeRotaton = %s"), *Scene->RelativeRotation.ToString());
	
}

void AGoKart::Elevation(float Val)
{
	auto val = FMath::Clamp(Val, -1.f, +1.f);
	auto pitch = val * GetWorld()->GetDeltaSeconds() * 50;

	auto pitchChange = SpringArm->RelativeRotation.Pitch + pitch;
	auto Newpitch = FMath::Clamp<float>(pitchChange, -40.f, 0.f);
	
	auto NewRotation = FRotator(Newpitch, 0.f, 0.f);
	
	//SpringArm->AddRelativeRotation(NewRotation);

	SpringArm->SetRelativeRotation(NewRotation);
}


/// No more needed
//void AGoKart::Server_MoveForward_Implementation(float Value)
//{
//	Throttle = Value;
//	/*UEngine* engine = GetGameInstance()->GetEngine();
//	engine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("This is Only Client"));*/
//}
//
//bool AGoKart::Server_MoveForward_Validate(float Value)
//{
//	return FMath::Abs(Value) <= 1;
//}
//
//
//void AGoKart::Server_MoveRight_Implementation(float Value)
//{
//	SteeringThrow = Value;
//}
//
//bool AGoKart::Server_MoveRight_Validate(float Value)
//{
//	return FMath::Abs(Value) <= 1;
//}