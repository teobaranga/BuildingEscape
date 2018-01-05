// Copyright Ben Tristem 2016.

#include "DoorComponent.h"
#include "BuildingEscape.h"

#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UDoorComponent::UDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UDoorComponent::BeginPlay()
{
	Super::BeginPlay();
}
