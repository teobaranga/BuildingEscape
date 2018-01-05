// Copyright Ben Tristem 2016.

#include "Grabber.h"
#include "BuildingEscape.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component, destroying grabber..."), *GetOwner()->GetName());
		DestroyComponent();
	}
}

/// Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// Get player view point this tick
		FVector lineTraceStart, lineTraceEnd;
		GetLineTraceData(lineTraceStart, lineTraceEnd);

		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

void UGrabber::Grab() {
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();

	/// If we hit something then attach a physics handle
	if (HitResult.GetActor())
	{
		auto ComponentToGrab = HitResult.GetComponent();

		// attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator::ZeroRotator
			);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get player view point this tick
	FVector lineTraceStart, lineTraceEnd;
	GetLineTraceData(lineTraceStart, lineTraceEnd);

	/// Setup query parameters
	FCollisionQueryParams traceParameters;
	traceParameters.AddIgnoredActor(GetOwner());

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		lineTraceStart,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParameters
	);

	return hitResult;
}

void UGrabber::GetLineTraceData(FVector &out_LineTraceStart, FVector &out_LineTraceEnd)
{
	FVector location;
	FRotator orientation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(location, orientation);

	out_LineTraceStart = location;
	out_LineTraceEnd = location + orientation.Vector() * Reach;
}