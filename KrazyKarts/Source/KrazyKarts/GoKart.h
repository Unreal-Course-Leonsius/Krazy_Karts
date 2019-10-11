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

protected:

	void Azimuth(float Val);
	void Elevation(float Val);

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

	/// The number of degrees rotated per second at full control throw (degrees/s).
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);
	
};
