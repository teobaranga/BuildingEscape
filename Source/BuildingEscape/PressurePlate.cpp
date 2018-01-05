// Copyright Ben Tristem 2016.

#include "PressurePlate.h"
#include "BuildingEscape.h"

#include "Components/BoxComponent.h"
#include "DoorComponent.h"

#include <algorithm>

// Sets default values
APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetMobility(EComponentMobility::Static);
	RootComponent = TriggerVolume;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);       // set up a notification for when this component overlaps something
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);       // set up a notification for when this component overlaps something
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
}

void APressurePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered: %s"), *OtherActor->GetName());
	CurrentMass += OtherComp->GetMass();
	CheckTriggerPlate();
}

void APressurePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Exited: %s"), *OtherActor->GetName());
	CurrentMass = std::max(CurrentMass - OtherComp->GetMass(), 0.f);
	CheckTriggerPlate();
}

void APressurePlate::CheckTriggerPlate()
{
	if (!PlatePressed)
	{
		if (CurrentMass >= TriggerMass)
		{
			PlatePressed = true;
			for (auto Door : TriggerActors)
			{
				UDoorComponent* DoorComponent = Door->FindComponentByClass<UDoorComponent>();
				if (DoorComponent)
				{
					DoorComponent->OnOpen.Broadcast();
				}
			}
		}
	}
	else if (CurrentMass < TriggerMass)
	{
		PlatePressed = false;
		for (auto Door : TriggerActors)
		{
			UDoorComponent* DoorComponent = Door->FindComponentByClass<UDoorComponent>();
			if (DoorComponent)
			{
				DoorComponent->OnClose.Broadcast();
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Door closed"));
	}
}
