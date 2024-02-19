// Copyright Tiago Marino Silva


#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

constexpr int HighlightStencilValue = 250;

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
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