// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY();


	UPROPERTY()
		float throttle;
	UPROPERTY()
		float SteeringThrow;
	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;

	bool IsValid() const
	{
		return FMath::Abs(throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FGoKartMove Move);
	

	FVector GetVelocity() { return Velocity;};
	void SetVelocity(FVector Val) { Velocity = Val;};

	void SetThrottle(float Val) { throttle = Val;};
	void SetSteeringThrow(float Val) {SteeringThrow = Val;};

	FGoKartMove GetLastMove() { return LastMove;};

private:
	
	FVector GetAirResistance();
	FVector GetRollingResistence();

	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteeringThrowVar);
	FGoKartMove CreateMove(float DeltaTime);
	//Mass of the car [kg].
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	//[M] 
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;
	//Force when throttle down [N]
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;
	//kg/m
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	//Rolling Resistance
	UPROPERTY(EditAnywhere)
	float RRCoefficient = 0.015;

	float throttle;
	float SteeringThrow;
	FVector Velocity;
	FGoKartMove LastMove;
};
