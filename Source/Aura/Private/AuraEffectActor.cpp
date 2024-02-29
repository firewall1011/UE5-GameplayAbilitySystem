// Copyright Tiago Marino Silva


#include "AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetAbilitySystem)) return;
	
	FGameplayEffectContextHandle ContextHandle = TargetAbilitySystem->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystem->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, ContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteGameplayEffectRemovalPolicy != EEffectRemovalPolicy::DoNotRemove)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystem);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	
	if (DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	
	if (InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
    {
    	ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
    }
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
    {
    	ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
    }

	if (DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteGameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetAbilitySystem)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (const auto HandlePair : ActiveEffectHandles)
		{
			if (TargetAbilitySystem == HandlePair.Value)
			{
				TargetAbilitySystem->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}

		if (bDestroyOnEffectRemoval && HandlesToRemove.Num() > 0)
		{
			Destroy();
		}
	}
}

