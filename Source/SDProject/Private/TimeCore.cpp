// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeCore.h"

// Sets default values
ATimeCore::ATimeCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimeCore::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATimeCore::StartEpoch, 1.0, false);
}

// Called every frame
void ATimeCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Paused) {
		TimeValue += TimeSpeed;
		Clock += TimeSpeed;
		if (Clock > 10) Update();

		if (TimeValue > Duration) {
			EndEpoch();
			StartEpoch();
		}
	}
}

void ATimeCore::Update()
{
	Clock -= 10;

	UpdateEpochEvent.Broadcast(CurentEpoch, TimeValue);
}

void ATimeCore::Stop()
{
	Paused = true;
	StopEpochEvent.Broadcast(CurentEpoch);
}

void ATimeCore::Resume()
{
	Paused = false;
	ResumeEpochEvent.Broadcast(CurentEpoch);
}

void ATimeCore::SetupTimeValue(int32 Value)
{
	TimeValue = Value;

	int32 Temp = Duration;
	CurentEpoch = 1;

	while (TimeValue > Temp) {
		CurentEpoch++;
		Temp += Duration;
	}
	
}

void ATimeCore::StartEpoch()
{
	Paused = false;
	StartEpochEvent.Broadcast(CurentEpoch);
}

void ATimeCore::EndEpoch()
{
	if (CurentEpoch < LastEpoch) {
		EndEpochEvent.Broadcast(CurentEpoch);
		CurentEpoch += 1;
	}
	else {
		Paused = true;
	}
}

