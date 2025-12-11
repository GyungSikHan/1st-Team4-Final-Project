#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "GameFramework/Character.h"
#include "AI/BehaviorTree/CBehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Engine/TargetPoint.h"
#include "BaseAI.generated.h"

class UBehaviorTree;
class USphereComponent;

UCLASS()
class EMBER_API ABaseAI : public ACharacter
{
	GENERATED_BODY()
public:
	FDamagesData GetDamagesData()const { return DamageData; }
	
public:
	ABaseAI();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
	
	// AI State
	UFUNCTION(BlueprintCallable, Category = "AI|State")
	virtual void OnDeath();
	virtual void EndDeath();

	// AI Patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	TArray<ATargetPoint*> PatrolPoint;

	// Getter
	// virtual float GetAttackPower() const { return AttackPower; }
	UBehaviorTree* GetBehaviorTree() const;

	virtual void Tick(float DeltaSeconds) override;


protected:
	// AI 기본 정보	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	TObjectPtr<class UMontageSystemComponent> MontageComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	TObjectPtr<class UEquipmentManagerComponent> EquipComponent;
	//TODOS HP 관련 변경 요청
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	TObjectPtr<class UStatusComponent> StatusComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	TObjectPtr<class UC_StateComponent> AIState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category = "AI/Weapon")
	TObjectPtr<class UCAIWeaponComponent> WeaponComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move")
	TObjectPtr<class UC_CharacterMovementComponent> AIMoveComponent;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AI|Behavior")
	TObjectPtr<UCBehaviorTreeComponent> BehaviorTreeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "AI/BehaviorTree")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* WidgetTrigger;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void HandleBeginOverlap(AActor* OtherActor) {}
	virtual void HandleEndOverlap(AActor* OtherActor) {}
public:
	UC_CharacterMovementComponent* GetAIMovement()const;
	UStatusComponent* GetStatusComponent()const { return StatusComponent; }
	
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	// float MaxHP;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	// float CurrentHP;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stat")
	// float AttackPower;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Info")
	FName MonsterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
	bool bIsDie;

	UPROPERTY()
	TObjectPtr<AActor> LastDamageCauser;
	UPROPERTY(EditAnywhere, Category = "Speed")
	int32 WalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Speed")
	int32 RunSpeed;
	UPROPERTY(EditAnywhere, Category = "Speed")
	int32 SprintSpeed;
private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHitted(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION()
	void OnRep_Hitted();
public:
	UFUNCTION(BlueprintCallable, Category="Sound")
	void PlaySound(AISoundCategory InSoundType);

private:
	UPROPERTY(ReplicatedUsing = "OnRep_Hitted")
	FDamagesData DamageData;
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* AISounds[(int32)AISoundCategory::Max];
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundAttenuation* SoundAttenuation;
	UPROPERTY(EditAnywhere, Category="Debug")
	bool bDebug;

};
