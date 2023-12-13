// Copyright Tiago Marino Silva

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraEnemy();

protected:
	virtual void BeginPlay() override;
};

