// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementComponent.h"
#include "GameFramework/GameStateBase.h"
// Sets default values for this component's properties
UGoKartMovementComponent::UGoKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy){
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}

	// ...
}

void UGoKartMovementComponent::SimulateMove(const FGoKartMove Move)
{
	//Moving Forward
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.throttle;
	Force += GetAirResistance();
	Force += GetRollingResistence();

	FVector Accleration = Force / Mass;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	Velocity = Velocity + Accleration * Move.DeltaTime;

	UpdateLocationFromVelocity(Move.DeltaTime);
}

FVector UGoKartMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UGoKartMovementComponent::GetRollingResistence()
{
	float AccererationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccererationDueToGravity;
	return -Velocity.GetSafeNormal() * RRCoefficient * NormalForce;
}

void UGoKartMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FHitResult HitResult;

	FVector Translation = Velocity * DeltaTime * 100;

	GetOwner()->AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}

}
void UGoKartMovementComponent::ApplyRotation(float DeltaTime, float SteeringThrowVar)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrowVar;
	FQuat RoationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
	Velocity = RoationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RoationDelta);
}

FGoKartMove UGoKartMovementComponent::CreateMove(float DeltaTime)
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;//*10 to cheat test
	Move.SteeringThrow = SteeringThrow;
	Move.throttle = throttle;
	//In the previous lecture we use GetWorld()->GetTimeSeconds()  to compare moves. 
	//This could be problematic if the local times diverged significantly.
	//A more robust approach would be to use
	//AGameStateBase::GetServerWorldTimeSeconds() which automatically tries to guess the lag and give you the current server time.
	//#include "GameFramework/GameStateBase.h
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	//Move.Time = GetWorld()->TimeSeconds;

	return Move;
}