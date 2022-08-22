// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSynchronizedWithTime.h"

// Sets default values
AActorSynchronizedWithTime::AActorSynchronizedWithTime()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorSynchronizedWithTime::BeginPlay()
{
	Super::BeginPlay();

	//Binding epoch events 
	if (ParentTimeCore) {
		ParentTimeCore->UpdateEpochEvent.AddDynamic(this, &AActorSynchronizedWithTime::OnUpdateEpochEvent);
		ParentTimeCore->StartEpochEvent.AddDynamic(this, &AActorSynchronizedWithTime::OnStartEpochEvent);
		ParentTimeCore->EndEpochEvent.AddDynamic(this, &AActorSynchronizedWithTime::OnEndEpochEvent);
		ParentTimeCore->StopEpochEvent.AddDynamic(this, &AActorSynchronizedWithTime::OnStopEpochEvent);
		ParentTimeCore->ResumeEpochEvent.AddDynamic(this, &AActorSynchronizedWithTime::OnResumeEpochEvent);
	}
}

// Called every frame
void AActorSynchronizedWithTime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AActorSynchronizedWithTime::OnUpdateEpochEvent(int32 EpochNum, int32 VisualisatingValue)
{
	OnUpdateEpoch(EpochNum, VisualisatingValue);

	UpdateEpoch(EpochNum, VisualisatingValue);
}

void AActorSynchronizedWithTime::OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue)
{

}

void AActorSynchronizedWithTime::OnStartEpochEvent(int32 EpochNum)
{
	OnStartEpoch(EpochNum);

	StartEpoch(EpochNum);
}

void AActorSynchronizedWithTime::OnStartEpoch(int32 EpochNum)
{

}

void AActorSynchronizedWithTime::OnEndEpochEvent(int32 EpochNum)
{
	OnEndEpoch(EpochNum);

	EndEpoch(EpochNum);
}

void AActorSynchronizedWithTime::OnEndEpoch(int32 EpochNum)
{

}

void AActorSynchronizedWithTime::OnStopEpochEvent(int32 EpochNum)
{
	OnStopEpoch(EpochNum);

	StopEpoch(EpochNum);
}

void AActorSynchronizedWithTime::OnStopEpoch(int32 EpochNum)
{

}

void AActorSynchronizedWithTime::OnResumeEpochEvent(int32 EpochNum)
{
	OnResumeEpoch(EpochNum);

	ResumeEpoch(EpochNum);
}

void AActorSynchronizedWithTime::OnResumeEpoch(int32 EpochNum)
{

}