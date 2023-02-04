// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UECFPWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UECOURSEFINALPROJECT_API UUECFPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUECFPWeaponComponent();

protected:
	
	virtual void BeginPlay() override;
};
