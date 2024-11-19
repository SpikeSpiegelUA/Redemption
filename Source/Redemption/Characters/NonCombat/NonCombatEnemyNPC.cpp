// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCombatEnemyNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\GameInstance\Structs\ActorGameInstanceData.h"
#include "Kismet/GameplayStatics.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Dynamics/Gameplay/Managers/GameManager.h"

ANonCombatEnemyNPC::ANonCombatEnemyNPC()
{
	//Create widget for NonCombatEnemyDetectionBarWidget
	NonCombatEnemyDetectionBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	NonCombatEnemyDetectionBarComponentWidget->SetWidgetSpace(EWidgetSpace::World);
	NonCombatEnemyDetectionBarComponentWidget->SetupAttachment(RootComponent);
}

void ANonCombatEnemyNPC::BeginPlay()
{
	Super::BeginPlay();
	ForwardMarker = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	NonCombatEnemyNPCAIController = Cast<ANonCombatEnemyNPCAIController>(this->GetController());
	ensure(NonCombatEnemyNPCAIController);
	//Set up properties for NonCombatEnemyDetectionBarWidget
	NonCombatEnemyDetectionBarComponentWidget->SetWidgetClass(NonCombatEnemyDetectionBarClass);
	NonCombatEnemyDetectionBarWidget = Cast<UNonCombatEnemyDetectionBarWidget>(NonCombatEnemyDetectionBarComponentWidget->GetWidget());
	if (IsValid(NonCombatEnemyDetectionBarWidget)) {
		NonCombatEnemyDetectionBarWidget->Detection = NonCombatEnemyNPCAIController->GetPlayerDetection();
	}
	if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
		Execute_LoadObjectFromGameInstance(this, RedemptionGameInstance);
}

void ANonCombatEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(NonCombatEnemyNPCAIController) && IsValid(Player)) {
		NonCombatEnemyNPCAIController->SetBlackboardDistanceToThePlayer((Player->GetActorLocation() - GetActorLocation()).Length());
		if (NonCombatEnemyNPCAIController->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) && (Player->GetActorLocation()-this->GetActorLocation()).Length() <= 200.f) {
			FVector FacingVector = Player->GetActorLocation() - this->GetActorLocation();
			FRotator FacingRotator = FacingVector.Rotation();
			FacingRotator.Pitch = 0.f;
			this->SetActorRotation(FacingRotator, ETeleportType::None);
		}
	}
	if (IsValid(NonCombatEnemyDetectionBarComponentWidget)) {
		FRotator ComponentRotation = NonCombatEnemyDetectionBarComponentWidget->GetComponentRotation();
		NonCombatEnemyDetectionBarComponentWidget->SetWorldRotation(FRotator(ComponentRotation.Pitch, 90, 0));
	}
}

void ANonCombatEnemyNPC::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (IsValid(GameInstance) && !this->GetFName().IsNone()) {
		TArray<FActorGameInstanceData> ActorGameInstanceDataArray{};
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
			ActorGameInstanceDataArray = GameInstance->TownActors;
		else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
			ActorGameInstanceDataArray = GameInstance->DungeonActors;
		bool ToBeDestroyed = true;
		for (FActorGameInstanceData ActorGameInstanceData : ActorGameInstanceDataArray)
			if (ActorGameInstanceData.ActorName == this->GetFName())
			{
				SetActorTransform(ActorGameInstanceData.ActorTransform);
				FMemoryReader MemReader(ActorGameInstanceData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				Serialize(Ar);
				ToBeDestroyed = false;
				if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
					if (const auto* const GameManager = RedemptionGameModeBase->GetGameManager(); IsValid(GameManager)) {
						FActorSpawnParameters ActorSpawnParameters{};
						SmartObject = GetWorld()->SpawnActor<ASmartObject>(GameManager->NonCombatEnemyNPCSmartObjectClass, ActorSpawnParameters);
						NonCombatEnemyNPCAIController->SetDynamicSubtree();
					}
				}
				break;
			}
		if (const auto* const GameMode = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(GameMode))
			if (ToBeDestroyed && ActorGameInstanceDataArray.Num() > 0 && !GameMode->Midgame)
				Destroy();
	}
}

TArray<TSubclassOf<ACombatEnemyNPC>> ANonCombatEnemyNPC::GetBattleEnemies()
{
	return BattleEnemies;
}

void ANonCombatEnemyNPC::SetBattleEnemies(const TArray<TSubclassOf<ACombatEnemyNPC>>& NewBattleEnemies)
{
	BattleEnemies = NewBattleEnemies;
}

UBoxComponent* ANonCombatEnemyNPC::GetForwardMarker() const
{
	return ForwardMarker;
}

UNonCombatEnemyDetectionBarWidget* ANonCombatEnemyNPC::GetNonCombatEnemyDetectionBarWidget() const
{
	return NonCombatEnemyDetectionBarWidget;
}
