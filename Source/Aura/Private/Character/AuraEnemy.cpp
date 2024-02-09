// Copyright Tiago Marino Silva


#include "Character/AuraEnemy.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEnemy::HighlightActor()
{
	bIsHighlighted = true;	
}

void AAuraEnemy::UnHighlightActor()
{
	bIsHighlighted = false;
}

