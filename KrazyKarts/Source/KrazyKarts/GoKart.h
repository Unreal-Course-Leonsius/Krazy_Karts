// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

public:

	class UBoxComponent* Collision;

	class USceneComponent *Scene;

	class USpringArmComponent *SpringArm;

	class UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyRotation(float DeltaTime);

	void UpdateLocationFromVelocity(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(USceneComponent *BP_scene, USpringArmComponent *BP_springarm, UCameraComponent* BP_camera, UBoxComponent* BP_Collision);

	/* Put Components Settings */
	void Setting();


private:
	/// Car Mass (kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	/// Force applied to the car when throttle is fully down (newton)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	/// Minimum radius of the car turning circle at full lock (m).
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	/// Higher means more drag (kg/m)
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;  // Air Resistance Coefficient

	// Higher means more rolling resistance.
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	UPROPERTY(ReplicatedUsing=OnRep_ReplicatedTransform)
	FTransform ReplicatedTransform;
	UFUNCTION()
	void OnRep_ReplicatedTransform();

	UPROPERTY(Replicated)
	FVector Velocity;

	UPROPERTY(Replicated)
	float Throttle;

	UPROPERTY(Replicated)
	float SteeringThrow;

	//UPROPERTY(Replicated)
	//FVector ReplicatedLocation;

	//UPROPERTY(Replicated)
	//FRotator ReplicatedRotation;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Value);

	void Azimuth(float Val);
	void Elevation(float Val);

	FVector GetAirResistance();
	FVector GetRollingResistance();
	
};
