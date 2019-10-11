// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"

#define OUT

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * Throttle * MaxDrivingForce;

	//UE_LOG(LogTemp, Warning, TEXT("Force = %s"), *Force.ToString());

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * DeltaTime;

	UpdateLocationFromVelocity(DeltaTime);
	

	ApplyRotation(DeltaTime);
	
}

void AGoKart::ApplyRotation(float DeltaTime)
{
	float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AGoKart::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime;  // x = v * t

	FHitResult Hit;
	AddActorWorldOffset(Translation, true, &Hit);   // Change car's location

	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}

void AGoKart::MoveRight(float Value)
{
	SteeringThrow = Value;
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