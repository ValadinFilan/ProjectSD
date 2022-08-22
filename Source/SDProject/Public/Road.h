// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorInTime.h"
#include "Components/StaticMeshComponent.h"
#include "Road.generated.h"

/**
 * 
 */

UCLASS()
class SDPROJECT_API ARoad : public AActorInTime
{
	GENERATED_BODY()
	
public:

	ARoad();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Road;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Border;

	UPROPERTY(EditAnywhere)
	int32 Size = 2;

	UPROPERTY(EditAnywhere)
	float Offset = 100.f;

	ARoad* CreateRoad(FVector Vector);

	AActor* SpawnBlock(UClass* ObjectClass, FVector RelativeVector);

	void Construct();

	ARoad* GetNearRoad(FVector Location, TArray<ARoad*>* Prev = nullptr);

	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	ARoad* XMinusRoad = nullptr;

	UPROPERTY(VisibleAnywhere)
	ARoad* XPlusRoad = nullptr;

	UPROPERTY(VisibleAnywhere)
	ARoad* YMinusRoad = nullptr;

	UPROPERTY(VisibleAnywhere)
	ARoad* YPlusRoad = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Marker = -1;

	virtual void Destroyed() override;

	//Animating

	UPROPERTY(EditAnywhere)
	bool IsAnimatingRoot = false;


	UPROPERTY()
	FVector AnimatingVector = FVector::ZeroVector;

	UPROPERTY()
	FVector AnimatingPoint;

	UPROPERTY()
	FVector BaseAnimatingPoint;

	UPROPERTY(EditAnywhere)
	bool BuildingComplete = false;

	UPROPERTY()
	bool Built = false;

	UPROPERTY()
	float AnimatingLength = 0;

	void BeginRoadAnimating(FVector Vector);

	void MarkRoad(int32 Last);

	virtual void OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue) override;

	virtual bool IsRelativePointInVisibleVolume(FVector Point, bool Reverse);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building")
	void Build();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building")
	void Erase();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | X road")
	void AddXPlus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | X road")
	void DeleteXPlus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | X road")
	void AddXMinus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | X road")
	void DeleteXMinus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | Y road")
	void AddYPlus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | Y road")
	void DeleteYPlus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | Y road")
	void AddYMinus();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Building | Y road")
	void DeleteYMinus();
};