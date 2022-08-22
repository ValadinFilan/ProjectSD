// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeCore.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateEpoch, int32, EpochNum, int32, VisualisatingValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartEpoch, int32, EpochNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndEpoch, int32, EpochNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopEpoch, int32, EpochNum);

UCLASS()
class SDPROJECT_API ATimeCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeCore();

	//Broadcast at epoch tick
	FUpdateEpoch UpdateEpochEvent;

	//Broadcast at start of new epoch
	FStartEpoch StartEpochEvent;

	//Broadcast at end of current epoch
	FEndEpoch EndEpochEvent;

	//Broadcast at pause of current epoch
	FStopEpoch StopEpochEvent;

	//Broadcast at resume of current epoch
	FStopEpoch ResumeEpochEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Duration = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 TimeSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	bool Paused = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 CurentEpoch = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 LastEpoch = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 TimeValue = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Epoch clocks 
	int32 Clock = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Update epoch function
	void Update();

	//Start epoch function
	void StartEpoch();

	//End epoch function
	void EndEpoch();

	//Stop epoch function
	void Stop();

	//Resume epoch function
	void Resume();

	//Setup new time value
	UFUNCTION(BlueprintCallable)
	void SetupTimeValue(int32 Value);

};
