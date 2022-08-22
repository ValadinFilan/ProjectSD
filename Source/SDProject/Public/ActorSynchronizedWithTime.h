// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeCore.h"
#include "ActorSynchronizedWithTime.generated.h"


UCLASS()
class SDPROJECT_API AActorSynchronizedWithTime : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorSynchronizedWithTime();

	//Speed of acting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time")
	int32 Speed = 10;

	//Parent time core for binding events
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Core")
	ATimeCore* ParentTimeCore = nullptr;

	//True when animation is active
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	bool Active = false;

	//Paused animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	bool Paused = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* Epoch operations
	*/

	// Binded to UpdateEpochEventFunction
	UFUNCTION()
	void OnUpdateEpochEvent(int32 EpochNum, int32 VisualisatingValue);
	// On epoch update event
	UFUNCTION()
	virtual void OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue);
	// Blueprint function for Epoch update
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEpoch(int32 EpochNum, int32 VisualisatingValue);


	// Binded to StartEpochEventFunction
	UFUNCTION()
	void OnStartEpochEvent(int32 EpochNum);
	// On epoch start event
	UFUNCTION()
	virtual void OnStartEpoch(int32 EpochNum);
	// Blueprint function for epoch update
	UFUNCTION(BlueprintImplementableEvent)
	void StartEpoch(int32 EpochNum);


	// Binded to EndEpochEventFunction
	UFUNCTION()
	void OnEndEpochEvent(int32 EpochNum);
	// On Epoch end event
	UFUNCTION()
	virtual void OnEndEpoch(int32 EpochNum);
	// Blueprint function for epoch end
	UFUNCTION(BlueprintImplementableEvent)
	void EndEpoch(int32 EpochNum);


	// Binded to StopEpochEventFunction
	UFUNCTION()
	void OnStopEpochEvent(int32 EpochNum);
	// On Epoch stop event
	UFUNCTION()
	virtual void OnStopEpoch(int32 EpochNum);
	// Blueprint function for epoch stop
	UFUNCTION(BlueprintImplementableEvent)
	void StopEpoch(int32 EpochNum);

	// Binded to ResumeEpochEventFunction
	UFUNCTION()
	void OnResumeEpochEvent(int32 EpochNum);
	// On Epoch resume event
	UFUNCTION()
	virtual void OnResumeEpoch(int32 EpochNum);
	// Blueprint function for epoch resume
	UFUNCTION(BlueprintImplementableEvent)
	void ResumeEpoch(int32 EpochNum);
};
