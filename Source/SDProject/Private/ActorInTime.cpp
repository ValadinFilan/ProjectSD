// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInTime.h"
#include "..\Public\ActorInTime.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AActorInTime::AActorInTime()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorInTime::BeginPlay()
{
	Super::BeginPlay();
	
	//Binding epoch events 
	if (ParentTimeCore) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Binded"));
		ParentTimeCore->UpdateEpochEvent.AddDynamic(this, &AActorInTime::OnUpdateEpochEvent);
		ParentTimeCore->StartEpochEvent.AddDynamic(this, &AActorInTime::OnStartEpochEvent);
		ParentTimeCore->EndEpochEvent.AddDynamic(this, &AActorInTime::OnEndEpochEvent);
		ParentTimeCore->StopEpochEvent.AddDynamic(this, &AActorInTime::OnStopEpochEvent);
		ParentTimeCore->ResumeEpochEvent.AddDynamic(this, &AActorInTime::OnResumeEpochEvent);
	}
	HideAll();
}

// Called every frame
void AActorInTime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Paused) Animate();
}

FVector AActorInTime::GetRelativeCoordinatesOfVector(FVector Vector)
{
	Vector = Vector - GetActorLocation();
	FVector R = GetActorRightVector();
	FVector F = GetActorForwardVector();
	FVector N = UKismetMathLibrary::Cross_VectorVector(F, R);
	FMatrix P(FPlane(F.X, F.Y, F.Z, 0),
		FPlane(R.X, R.Y, R.Z, 0),
		FPlane(N.X, N.Y, N.Z, 0),
		FPlane(0, 0, 0, 1));
	Vector = P.Inverse().TransformVector(Vector);
	Vector.X /= GetActorScale().X;
	Vector.Y /= GetActorScale().Y;
	Vector.Z /= GetActorScale().Z;

	return Vector;
}

bool AActorInTime::IsRelativePointInVisibleVolume(FVector Point, bool Reverse)
{
	return true;
}


void AActorInTime::AddToAnimList(AActor* Actor)
{
	if (!HideList.Contains(Actor) && !AnimList.Contains(Actor)) {
		if (Actor->IsHidden()) {
			AnimList.Add(Actor, GetRelativeCoordinatesOfVector(Actor->GetActorLocation()));
			Actor->AddActorLocalOffset(FVector(0, 0, -AnimationZOffset));
			Actor->SetActorHiddenInGame(false);
		}
	}
}

void AActorInTime::AddToHideList(AActor* Actor)
{
	if (!HideList.Contains(Actor) && !AnimList.Contains(Actor)) {
		if (!Actor->IsHidden()) {
			HideList.Add(Actor, GetRelativeCoordinatesOfVector(Actor->GetActorLocation() + FVector(0, 0, -AnimationZOffset)));
		}
	}
}

void AActorInTime::RemoveFromAnimList(AActor* Actor)
{
	if (AnimList.Contains(Actor)) {
		//Actor->SetActorLocation(*AnimList.Find(Actor) + GetActorLocation());
		Actor->SetActorRelativeLocation(*AnimList.Find(Actor));
		AnimList.Remove(Actor);
	}
	Actor->SetActorHiddenInGame(true);
}

void AActorInTime::RemoveFromHideList(AActor* Actor)
{
	if (HideList.Contains(Actor)) {
		Actor->SetActorRelativeLocation(*HideList.Find(Actor) + FVector(0, 0, AnimationZOffset));
		HideList.Remove(Actor);
		Actor->SetActorHiddenInGame(true);
	}
}

bool AActorInTime::AreAllAttachedActorsVisible()
{
	TArray<AActor*> Actors;
	GetAttachedActors(Actors);
	for (auto i : Actors) {
		if (i->IsHidden()) return false;
	}
	return true;
}

void AActorInTime::Visualise()
{
	TArray<AActor*> Modules;
	GetAttachedActors(Modules);
	for (auto Module : Modules) {
		if (IsRelativePointInVisibleVolume(GetRelativeCoordinatesOfVector(Module->GetActorLocation()), false)) {
			AddToAnimList(Module);
		}
		else {
			RemoveFromAnimList(Module);
		}
	}
}

void AActorInTime::Animate()
{
	if (AnimList.Num() > 0) {
		TArray<AActor*> ToDel;
		for (auto Item : AnimList) {
			Item.Key->AddActorLocalOffset(FVector(0, 0, AnimationSpeed));
			if (GetRelativeCoordinatesOfVector(Item.Key->GetActorLocation()).Z > Item.Value.Z) {
				Item.Key->SetActorRelativeLocation(Item.Value);
				ToDel.Add(Item.Key);
			}
		}
		for (auto i : ToDel) {
			AnimList.Remove(i);
		}
	}
	if (HideList.Num() > 0) {
		TArray<AActor*> ToDel;
		for (auto Item : HideList) {
			Item.Key->AddActorLocalOffset(FVector(0, 0, -AnimationSpeed));
			if (GetRelativeCoordinatesOfVector(Item.Key->GetActorLocation()).Z < Item.Value.Z) {
				Item.Key->SetActorHiddenInGame(true);
				Item.Key->SetActorRelativeLocation(Item.Value + FVector(0, 0, AnimationZOffset));
				ToDel.Add(Item.Key);
			}
		}
		for (auto i : ToDel) {
			HideList.Remove(i);
		}
	}
}

void AActorInTime::HideAll()
{
	TArray<AActor*> Modules;
	GetAttachedActors(Modules);
	for (auto Module : Modules) {
		Module->SetActorHiddenInGame(true);
	}
}

void AActorInTime::Hide()
{
	TArray<AActor*> Modules;
	GetAttachedActors(Modules);
	for (auto Module : Modules) {
		if (IsRelativePointInVisibleVolume(GetRelativeCoordinatesOfVector(Module->GetActorLocation()), true)) {
			AddToHideList(Module); 
		}
		else {
			RemoveFromHideList(Module);
		}
	}
}

void AActorInTime::OnUpdateEpochEvent(int32 EpochNum, int32 VisualisatingValue)
{
	OnUpdateEpoch(EpochNum, VisualisatingValue);

	UpdateEpoch(EpochNum, VisualisatingValue);
}

void AActorInTime::OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue)
{

	int32 Val = VisualisatingValue - (TargetEpoch - 1) * ParentTimeCore->Duration;

	if ((Threshold < Val || DestroyThreshold > Val - ParentTimeCore->Duration || (!HideActorInTheAndOfEpoch && EpochNum > TargetEpoch)) && !Paused) {
		Up = true;
		Visualise();
	}

	if(EpochNum > TargetEpoch && !Paused && HideActorInTheAndOfEpoch && DestroyThreshold < Val - ParentTimeCore->Duration) {
		Up = false;
		Hide();
	}
}

void AActorInTime::OnStartEpochEvent(int32 EpochNum)
{
	OnStartEpoch(EpochNum);

	StartEpoch(EpochNum);
}

void AActorInTime::OnStartEpoch(int32 EpochNum)
{
	if (EpochNum == TargetEpoch) {
		Active = true;
	}
}

void AActorInTime::OnEndEpochEvent(int32 EpochNum)
{
	OnEndEpoch(EpochNum);

	EndEpoch(EpochNum);
}

void AActorInTime::OnEndEpoch(int32 EpochNum)
{

}

void AActorInTime::OnStopEpochEvent(int32 EpochNum)
{
	OnStopEpoch(EpochNum);

	StopEpoch(EpochNum);
}

void AActorInTime::OnStopEpoch(int32 EpochNum)
{
	if(EpochNum == TargetEpoch) Paused = true;
}

void AActorInTime::OnResumeEpochEvent(int32 EpochNum)
{
	OnResumeEpoch(EpochNum);

	ResumeEpoch(EpochNum);
}

void AActorInTime::OnResumeEpoch(int32 EpochNum)
{
	if (EpochNum == TargetEpoch) Paused = false;
}

