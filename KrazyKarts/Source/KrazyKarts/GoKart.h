// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Component/GoKartMovementComponent.h"
#include "Component/GoKartMovementReplicator.h"
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(USceneComponent *BP_scene, USpringArmComponent *BP_springarm, UCameraComponent* BP_camera, UBoxComponent* BP_Collision);

	/* Put Components Settings */
	void Setting();


private:
	
	void MoveForward(float Value);
	void MoveRight(float Value);


	void Azimuth(float Val);
	void Elevation(float Val);

	UPROPERTY(VisibleAnywhere)
	UGoKartMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere)
	UGoKartMovementReplicator* MovementReplicator;

	
};
