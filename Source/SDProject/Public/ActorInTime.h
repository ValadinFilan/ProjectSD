// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeCore.h"
#include "ActorInTime.generated.h"

UCLASS()
class SDPROJECT_API AActorInTime : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorInTime();

	//Speed of visualisation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time")
	int32 Speed = 10;

	//Is it visible !!!!!!!!!!!!!NOT USED!!!!!!!!!!!!!!!!
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time")
	//bool Visible = true;

	//Epoch of appearance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Time")
	int32 TargetEpoch = 1;

	//Time threshold of appearance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Time")
	int32 Threshold = 0;

	//Time threshold of appearance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Time")
	int32 DestroyThreshold = 0;

	//Parent time core for binding events
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Core")
	ATimeCore* ParentTimeCore = nullptr;

	//True when animation is active
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	bool Active = false;

	//Offset of parts of object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	float AnimationZOffset = 100;

	//Speed of appearance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	int32 AnimationSpeed = 1;

	//Paused animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	bool Paused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Epoches and time | Animation")
	bool HideActorInTheAndOfEpoch = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//List of objects what will be animated
	TMap<AActor*, FVector> AnimList;
	TMap<AActor*, FVector> HideList;

	//Direction of growing
	bool Up = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/*
	* Some math
	*/

	FVector GetRelativeCoordinatesOfVector(FVector Vector);


	/*
	* Animation operations
	*/

	//Returns true if objects if in visible part of object
	virtual bool IsRelativePointInVisibleVolume(FVector Point, bool Reverse);

	//Add parts of object to animation list
	void AddToAnimList(AActor* Actor);

	//Add parts of object to hide list
	void AddToHideList(AActor* Actor);

	//Remove parts of object from animation list
	void RemoveFromAnimList(AActor* Actor);

	//Remove parts of object from animation list
	void RemoveFromHideList(AActor* Actor);

	//Retrurns true if all attached actors are visible
	bool AreAllAttachedActorsVisible();

	//Visualise objects in visualising volume
	virtual void Visualise();

	//Animation of appearance
	virtual void Animate();

	virtual void HideAll();
	virtual void Hide();
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
