// Fill out your copyright notice in the Description page of Project Settings.


#include "Road.h"
#include "..\Public\Road.h"
#include "DrawDebugHelpers.h"


ARoad::ARoad()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	SetRootComponent(BaseMesh);
	
	HideActorInTheAndOfEpoch = false;
	AnimationZOffset = 50;
	AnimationSpeed = 0.0001;
	Speed = 1;

}



ARoad* ARoad::CreateRoad(FVector Vector)
{
	FVector Location = GetActorLocation() + Vector;
	FRotator Rotation = GetActorRotation();

	ARoad* NearRoad = GetNearRoad(Location, nullptr);
	if (NearRoad) {
		return NearRoad;
	}
	else {
		ARoad* NewRoad = Cast<ARoad>(GetWorld()->SpawnActor(GetClass(), &Location, &Rotation));
		NewRoad->ParentTimeCore = ParentTimeCore;
		return NewRoad;
	}
}

AActor* ARoad::SpawnBlock(UClass* ObjectClass, FVector RelativeVector)
{
	if (ObjectClass) {
		AActor* Actor = GetWorld()->SpawnActor(ObjectClass, &RelativeVector);

		Actor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		return Actor;
	}
	else {
		return nullptr;
	}
}

void ARoad::Construct()
{
	Built = true;
	TArray<AActor*> ActorArray;
	GetAttachedActors(ActorArray);

	while (ActorArray.Num() > 0) {
		AActor* Actor = ActorArray[0];
		if (Actor) {
			ActorArray.Remove(Actor);
			Actor->Destroy();
		}
	}
	for (int32 i = -Size; i <= Size; i++)
	{
		for (int32 j = -Size; j <= Size; j++)
		{
			SpawnBlock(Road, FVector(i * Offset, j * Offset, 0));
		}
	}
	SpawnBlock(Border, FVector((Size + 1) * Offset, (Size + 1) * Offset, 0));
	SpawnBlock(Border, FVector((Size + 1) * Offset, -1 * (Size + 1) * Offset, 0));
	SpawnBlock(Border, FVector(-1 * (Size + 1) * Offset, (Size + 1) * Offset, 0));
	SpawnBlock(Border, FVector(-1 * (Size + 1) * Offset, -1 * (Size + 1) * Offset, 0));
	for (int i = -Size; i <= Size; i++)
	{
		SpawnBlock(XPlusRoad ? Road : Border, FVector((Size + 1) * Offset, i * Offset, 0));
		SpawnBlock(XMinusRoad ? Road : Border, FVector(-1 * (Size + 1) * Offset, i * Offset, 0));
		SpawnBlock(YPlusRoad ? Road : Border, FVector(i * Offset, (Size + 1) * Offset, 0));
		SpawnBlock(YMinusRoad ? Road : Border, FVector(i * Offset, -1 * (Size + 1) * Offset, 0));
	}

}

ARoad* ARoad::GetNearRoad(FVector Location, TArray<ARoad*>* Prev)
{
	if (!Prev || !Prev->Contains(this)) {
		if (!Prev) {
			Prev = new TArray<ARoad*>();
		}
		Prev->Add(this);
		if (FVector::Dist(GetActorLocation(), Location) < Offset) {
			return this;
		}
		if (XMinusRoad) {
			ARoad* NewRoad = XMinusRoad->GetNearRoad(Location, Prev);
			if (NewRoad) return NewRoad;
		}
		if (XPlusRoad) {
			ARoad* NewRoad = XPlusRoad->GetNearRoad(Location, Prev);
			if (NewRoad) return NewRoad;
		}
		if (YPlusRoad) {
			ARoad* NewRoad = YPlusRoad->GetNearRoad(Location, Prev);
			if (NewRoad) return NewRoad;
		}
		if (YMinusRoad) {
			ARoad* NewRoad = YMinusRoad->GetNearRoad(Location, Prev);
			if (NewRoad) return NewRoad;
		}
	}
	return nullptr;
}


void ARoad::Destroyed()
{
	TArray<AActor*> ActorArray;
	GetAttachedActors(ActorArray);
	
	while (ActorArray.Num() > 0) {
		AActor* Actor = ActorArray[0];
		if (Actor) {
			ActorArray.Remove(Actor);
			Actor->Destroy();
		}
	}
	
	if (XMinusRoad) {
		XMinusRoad->XPlusRoad = nullptr;
	}
	if (XPlusRoad) {
		XPlusRoad->XMinusRoad = nullptr;
	}
	if (YMinusRoad) {
		YMinusRoad->YPlusRoad = nullptr;
	}
	if (YPlusRoad) {
		YPlusRoad->XMinusRoad = nullptr;
	}

	Super::Destroyed();
}

void ARoad::BeginRoadAnimating(FVector Vector)
{
	if (AnimatingVector == FVector::ZeroVector) {
		AnimatingVector = Vector;

		FVector TempPoint = GetActorLocation();

		TArray<AActor*> ActorArray;
		GetAttachedActors(ActorArray);

		ActorArray.Sort([TempPoint, Vector](const AActor& A, const AActor& B) {
			return FVector::DotProduct(A.GetActorLocation() - TempPoint, Vector) < FVector::DotProduct(B.GetActorLocation() - TempPoint, Vector);
			});

		if (ActorArray.Num() > 0) { 
			AnimatingPoint = GetRelativeCoordinatesOfVector(ActorArray[0]->GetActorLocation()) - AnimatingVector;
			BaseAnimatingPoint = GetRelativeCoordinatesOfVector(ActorArray[0]->GetActorLocation()) - AnimatingVector;
		}
	}
}

void ARoad::BeginPlay() 
{
	Super::BeginPlay();

	if (IsAnimatingRoot) MarkRoad(0);
}

void ARoad::MarkRoad(int32 Last)
{
	TArray<ARoad*> Queue;
	Marker = Last;
	Queue.Add(this);
	int32 i = 0;
	while (Queue.Num() > 0) {
		i++;
		ARoad* RoadPtr = Queue.Pop(); 
		if (RoadPtr->XMinusRoad) {
			if (RoadPtr->XMinusRoad->Marker == -1) {
				RoadPtr->XMinusRoad->Marker = RoadPtr->Marker + 1;
				RoadPtr->XMinusRoad->BeginRoadAnimating(-FVector::XAxisVector);

				Queue.Insert(RoadPtr->XMinusRoad, 0);
			}
		}
		if (RoadPtr->XPlusRoad) {
			if (RoadPtr->XPlusRoad->Marker == -1) {
				RoadPtr->XPlusRoad->Marker = RoadPtr->Marker + 1;
				RoadPtr->XPlusRoad->BeginRoadAnimating(FVector::XAxisVector);

				Queue.Insert(RoadPtr->XPlusRoad, 0);
			}
		}
		if (RoadPtr->YMinusRoad) {
			if (RoadPtr->YMinusRoad->Marker == -1) {
				RoadPtr->YMinusRoad->Marker = RoadPtr->Marker + 1;
				RoadPtr->YMinusRoad->BeginRoadAnimating(-FVector::YAxisVector);

				Queue.Insert(RoadPtr->YMinusRoad, 0);
			}
		}
		if (RoadPtr->YPlusRoad) {
			if (RoadPtr->YPlusRoad->Marker == -1) {
				RoadPtr->YPlusRoad->Marker = RoadPtr->Marker + 1;
				RoadPtr->YPlusRoad->BeginRoadAnimating(FVector::YAxisVector);

				Queue.Insert(RoadPtr->YPlusRoad, 0);
			}
		}
	}
}

void ARoad::OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue)
{
	if (VisualisatingValue > Threshold && Speed > 0) {
		int32 Distance = (2 * Size + 1) * ((int32) Offset);
		if ((Distance + Offset) * Marker / Speed < VisualisatingValue) {
			AnimatingPoint = BaseAnimatingPoint + AnimatingVector * (VisualisatingValue - (Distance + Offset) * Marker / Speed) * Speed; // - (Distance - ((int32)(Offset / 2 + Offset))) * Marker) * Speed));
			AnimatingLength = VisualisatingValue * Speed;
		}
		else {
			AnimatingPoint = BaseAnimatingPoint;
			AnimatingLength = 0;
		}
		Visualise();
		Hide();
	}
}

bool ARoad::IsRelativePointInVisibleVolume(FVector Point, bool Reverse)
{
	if (IsAnimatingRoot) {
		if(Point.Length() < AnimatingLength) return !Reverse;
	}
	else if (AnimatingVector != FVector::ZeroVector) {
		if (FVector::DotProduct(Point - AnimatingPoint, AnimatingVector) < 0) {
			return !Reverse;
		}
	}
	return Reverse;
}

void ARoad::Build()
{
	if (!Built) {
		Construct();
		if (XMinusRoad) {
			XMinusRoad->Build();
		}
		if (XPlusRoad) {
			XPlusRoad->Build();
		}
		if (YMinusRoad) {
			YMinusRoad->Build();
		}
		if (YPlusRoad) {
			YPlusRoad->Build();
		}
	}
}

void ARoad::Erase()
{
	if (Built) {
		TArray<AActor*> ActorArray;
		GetAttachedActors(ActorArray);
		while (ActorArray.Num() > 0) {
			AActor* Actor = ActorArray[0];
			if (Actor) {
				Actor->Destroy();
			}
			ActorArray.Remove(Actor);
		}
		Built = false;
		if (XMinusRoad) {
			XMinusRoad->Erase();
		}
		if (XPlusRoad) {
			XPlusRoad->Erase();
		}
		if (YMinusRoad) {
			YMinusRoad->Erase();
		}
		if (YPlusRoad) {
			YPlusRoad->Erase();
		}
	}
}

void ARoad::AddXPlus()
{
	if (XPlusRoad) {
		XPlusRoad->AddXPlus();
	}
	else {
		XPlusRoad = CreateRoad(GetActorForwardVector() * (Offset * ((Size + 2) * 2 - 1)));
		XPlusRoad->XMinusRoad = this;
	}
}

void ARoad::DeleteXPlus()
{
	if (XPlusRoad) {
		if (XPlusRoad->XPlusRoad) {
			XPlusRoad->DeleteXPlus();
		}
		else {
			XPlusRoad->Destroy();
			XPlusRoad = nullptr;
		}
	}
}

void ARoad::AddXMinus()
{
	if (XMinusRoad) {
		XMinusRoad->AddXMinus();
	}
	else {
		XMinusRoad = CreateRoad(-1 * GetActorForwardVector() * (Offset * ((Size + 2) * 2 - 1)));
		XMinusRoad->XPlusRoad = this;
	}
}

void ARoad::DeleteXMinus()
{
	if (XMinusRoad) {
		if (XMinusRoad->XMinusRoad) {
			XMinusRoad->DeleteXMinus();
		}
		else {
			XMinusRoad->Destroy();
			XMinusRoad = nullptr;
		}
	}
}

void ARoad::AddYPlus()
{
	if (YPlusRoad) {
		YPlusRoad->AddYPlus();
	}
	else {
		YPlusRoad = CreateRoad(GetActorRightVector() * (Offset * ((Size + 2) * 2 - 1)));
		YPlusRoad->YMinusRoad = this;
	}
}

void ARoad::DeleteYPlus()
{
	if (YPlusRoad) {
		if (YPlusRoad->YPlusRoad) {
			YPlusRoad->DeleteYPlus();
		}
		else {
			YPlusRoad->Destroy();
			YPlusRoad = nullptr;
		}
	}
}

void ARoad::AddYMinus()
{
	if (YMinusRoad) {
		YMinusRoad->AddYMinus();
	}
	else {
		YMinusRoad = CreateRoad(-1 * GetActorRightVector() * (Offset * ((Size + 2) * 2 - 1)));
		YMinusRoad->YPlusRoad = this;
	}
}

void ARoad::DeleteYMinus()
{
	if (YMinusRoad) {
		if (YMinusRoad->YMinusRoad) {
			YMinusRoad->DeleteYMinus();
		}
		else {
			YMinusRoad->Destroy();
			YMinusRoad = nullptr;
		}
	}
}
