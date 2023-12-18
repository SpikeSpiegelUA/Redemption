// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
//#include "Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
//#include "Niagara/Public/NiagaraSystem.h"
//#include "NiagaraSystem.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "ParticlesManager.generated.h"

UCLASS()
class REDEMPTION_API AParticlesManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = true))
		UParticleSystem* DizzyEmitter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = true))
		UNiagaraSystem* FlamesEmitter;
public:	
	// Sets default values for this actor's properties
	AParticlesManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UParticleSystem* GetDizzyEmitter() const;
	UNiagaraSystem* GetFlamesEmitter() const;
};
