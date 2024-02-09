// Copyright Tiago Marino Silva


#include "Character/AuraEnemy.h"

constexpr int HighlightStencilValue = 250;

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void SetHighlightOnMesh(USkeletalMeshComponent* const Mesh, const bool bShouldHighlight)
{
	Mesh->SetRenderCustomDepth(bShouldHighlight);
	Mesh->SetCustomDepthStencilValue(HighlightStencilValue);
}

void AAuraEnemy::HighlightActor()
{
	SetHighlightOnMesh(GetMesh(), true);
	SetHighlightOnMesh(Weapon, true);
}

void AAuraEnemy::UnHighlightActor()
{
	SetHighlightOnMesh(GetMesh(), false);
	SetHighlightOnMesh(Weapon, false);
}