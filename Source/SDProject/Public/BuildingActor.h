// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorInTime.h"
#include "BuildingActor.generated.h"

/**
 * 
 */
UCLASS()
class SDPROJECT_API ABuildingActor : public AActorInTime
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building animation")
	int32 VisibleLevels = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building animation")
	int32 MaxLevel = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building animation")
	int32 VisibleAngle = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building animation")
	float LevelHeight = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building animation")
	float LevelRadius = 500;

	virtual bool IsRelativePointInVisibleVolume(FVector Point, bool Reverse) override;
		
	virtual void OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue) override;
};
