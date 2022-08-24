// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingActor.h"


bool ABuildingActor::IsRelativePointInVisibleVolume(FVector Point, bool Reverse)
{
	if (!Reverse) {
		if (Point.Z < (VisibleLevels - 1) * LevelHeight) return true;
		if (Point.Z > (VisibleLevels) *LevelHeight) return false;
	}
	else {
		if (Point.Z > (VisibleLevels + 1) * LevelHeight) return true;
		if (Point.Z < (VisibleLevels) * LevelHeight) return false;
	}

	float acos = FMath::RadiansToDegrees(FMath::Acos(Point.X / LevelRadius));

	if (VisibleAngle < 90) {
		if (Point.X > 0 && Point.Y > 0)
			if (acos < VisibleAngle) return true;
	}
	else if (VisibleAngle < 180) {
		if (Point.X > 0 && Point.Y > 0) return true;
		else if (Point.X < 0 && Point.Y > 0)
			if (acos < VisibleAngle) return true;

	}
	else if (VisibleAngle < 270) {
		if (Point.Y > 0) return true;
		if (360 - acos < VisibleAngle) return true;

	}
	else {
		if (Point.Y > 0 || Point.X < 0 && Point.Y < 0) return true;
		else if (Point.X > 0 && Point.Y < 0)
			if (360 - acos < VisibleAngle) return true;
	}
	return false;
}


void ABuildingActor::OnUpdateEpoch(int32 EpochNum, int32 VisualisatingValue)
{
	Super::OnUpdateEpoch(EpochNum, VisualisatingValue);
	int32 Val = VisualisatingValue - (TargetEpoch - 1) * ParentTimeCore->Duration;
	if (Up && Threshold < Val) {
		VisibleLevels = (Val - Threshold) / (360 / (AnimationSpeed)) + 1;
		VisibleAngle = (Val - Threshold) % (360 / (AnimationSpeed));
	}
	else if(!Up){
		VisibleLevels = MaxLevel - Val / (360 / (AnimationSpeed));
		VisibleAngle = 360 - Val % (360 / (AnimationSpeed));
	}
}
