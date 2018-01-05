// Copyright Ben Tristem 2016.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

UCLASS()
class BUILDINGESCAPE_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlate();

	/** Called when something enters the trigger volume */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when something leaves the trigger volume */
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void CheckTriggerPlate();

public:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate")
	TArray<AActor*> TriggerActors;
	
private:
	// Current mass on the pressure plate
	float CurrentMass = 0.f;

	bool PlatePressed = false;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate", meta = (ClampMin = "0.0"))
	float TriggerMass = 20.f;
};
