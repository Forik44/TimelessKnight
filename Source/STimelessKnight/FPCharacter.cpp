#include "FPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "STimelessKnightGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

AFPCharacter::AFPCharacter()
	:
MaxXP(100.f),
MaxMana(100.f),
ManaRTYour(2.f),
ManaRTObject(3.f),
ManaRTEnemy(4.f),
ManaRTEverything(10.f),
XPRegenerationRate(1.f),
ManaRegenerationRate(1.f),
SpeedStep(200),
SpeedRun(350),
SpeedMana(1.f),
RadiusOfTimeReverse(500.f),
SpeedRegeneration(1.f),
CountHelthTube(0),
CountManaTube(0),
ManaReplenishment(50.f),
HelthReplenishment(50.f),
SpeedCrouch(150.f)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 40));

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 12.f;

	TimeSystemCharacter = CreateDefaultSubobject<UTimeSystemCharacterComponent>(TEXT("TimeSystemCharacter"));
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereCollision->AttachTo(RootComponent);

}

void AFPCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f)
	{
		return;
	}
	float StartBaseEyeHight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHight - BaseEyeHeight + HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0, 0, BaseEyeHeight), false);
}

void AFPCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f)
	{
		return;
	}
	float StartBaseEyeHight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHight - BaseEyeHeight - HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0, 0, BaseEyeHeight), false);
}

void AFPCharacter::CalcCamera(float Deltatime, struct FMinimalViewInfo& OutResult)
{
	if (Camera)
	{
		Camera->GetCameraView(Deltatime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovementComponent()->MaxWalkSpeed = SpeedStep;
	CurrentXP = MaxXP;
	CurrentMana = MaxMana;
	GetWorld()->GetTimerManager().SetTimer(XPRegenerationTimer, this, &AFPCharacter::XPRegeneration, SpeedRegeneration, true);
	GetWorld()->GetTimerManager().SetTimer(ManaRegenerationTimer, this, &AFPCharacter::ManaRegeneration, SpeedRegeneration, true);
	GetWorld()->GetTimerManager().SetTimer(RayToSeeTimer, this, &AFPCharacter::RayToSeeInteractiveItem, 0.1, true);

	SphereCollision->SetSphereRadius(RadiusOfTimeReverse);
}

float AFPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	OnDamageTook.Broadcast();
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ChangeXP(GetXP() - DamageAmount);
	return DamageAmount;
}

void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
}

void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Hori", this, &AFPCharacter::HoriMove);
	PlayerInputComponent->BindAxis("Vert", this, &AFPCharacter::VertMove);

	PlayerInputComponent->BindAxis("HoriRot", this, &AFPCharacter::HoriRot);
	PlayerInputComponent->BindAxis("VertRot", this, &AFPCharacter::VertRot);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFPCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AFPCharacter::StopRun);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::Jump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPCharacter::StopCrouch);

	PlayerInputComponent->BindAction("RewindTimeYour", IE_Pressed, this, &AFPCharacter::RewindTimeYourStart);
	PlayerInputComponent->BindAction("RewindTimeObject", IE_Pressed, this, &AFPCharacter::RewindTimeObjectStart);
	PlayerInputComponent->BindAction("RewindTimeEverything", IE_Pressed, this, &AFPCharacter::RewindTimeEverythingStart);

	PlayerInputComponent->BindAction("RewindTimeYour", IE_Released, this, &AFPCharacter::RewindTimeYourStop);
	PlayerInputComponent->BindAction("RewindTimeObject", IE_Released, this, &AFPCharacter::RewindTimeObjectStop);
	PlayerInputComponent->BindAction("RewindTimeEverything", IE_Released, this, &AFPCharacter::RewindTimeEverythingStop);

	PlayerInputComponent->BindAction("UseHelthTube", IE_Pressed, this, &AFPCharacter::UseHelthTube);
	PlayerInputComponent->BindAction("UseManaTube", IE_Pressed, this, &AFPCharacter::UseManaTube);

	PlayerInputComponent->BindAction("Take", IE_Released, this, &AFPCharacter::TakeItem);
}

void AFPCharacter::ChangeXP(float value)
{
	UE_LOG(LogTemp, Log, TEXT("ChangeXP"));
	if (value <= 0.f) 
	{
		CurrentXP = 0.f;
		OnXPChanged.Broadcast(CurrentXP);
		OnDeath.Broadcast();
	}
	else
	{
		if (value <= MaxXP)
		{
			CurrentXP = value;
		}
		else if (value > MaxXP)
		{
			CurrentXP = MaxXP;
		}
		OnXPChanged.Broadcast(CurrentXP);
	}
}

void AFPCharacter::ChangeMana(float value)
{
	if ((value <= MaxMana) && (value >= 0.f))
	{
		CurrentMana = value;
	}
	else if (value > MaxMana)
	{
		CurrentMana = MaxMana;
	}
	else if (value < 0.f)
	{
	CurrentMana = 0.f;
	}
	OnManaChanged.Broadcast(CurrentMana);
}

void AFPCharacter::XPRegeneration()
{
	if (CurrentXP < MaxXP)
	{
		ChangeXP(CurrentXP + XPRegenerationRate);
	}
}

void AFPCharacter::ManaRegeneration()
{
	if (CurrentMana < MaxMana)
	{
		ChangeMana(CurrentMana + ManaRegenerationRate);
	}
}

void AFPCharacter::ChangeManaRTYour()
{
	if (CurrentMana >= ManaRTYour) {
		ChangeMana(CurrentMana - ManaRTYour);
		TimeSystemCharacter->StartRevers();
	}
	else
	{
		TimeSystemCharacter->StopRevers();
		GetWorld()->GetTimerManager().ClearTimer(ManaRTYourTimer);
		RewindTimeYourStop();
	}
}

void AFPCharacter::ChangeManaRTObject()
{
	if (CurrentMana >= ManaRTObject) {
		if (CatchedObject)
		{
			ChangeMana(CurrentMana - ManaRTObject);
			CatchedObject->TimeSystem->StartRevers();
		}
		if (CatchedEnemy)
		{
			ChangeMana(CurrentMana - ManaRTEnemy);
			CatchedEnemy->TimeSystemCharacter->StartRevers();
		}
	}
	else
	{
		if (CatchedObject)
		{
			CatchedObject->TimeSystem->StopRevers();
		}
		if (CatchedEnemy)
		{
			CatchedEnemy->TimeSystemCharacter->StopRevers();
		}
		RewindTimeObjectStop();
	}
}

void AFPCharacter::ChangeManaRTEverything()
{
	if (CurrentMana >= ManaRTEverything) {
		ChangeMana(CurrentMana - ManaRTEverything);

		for (auto Item : InteractiveItems)
		{
			Cast<AInteractiveItem>(Item)->TimeSystem->StartRevers();
		}
		for (auto Enemy : DefaultEnemies)
		{
			Cast<ADefaultEnemyCharacter>(Enemy)->TimeSystemCharacter->StartRevers();
		}
	}
	else
	{
		for (auto Item : InteractiveItems)
		{
			if (Item)
			{
				Cast<AInteractiveItem>(Item)->TimeSystem->StopRevers();
			}
		}
		for (auto Enemy : DefaultEnemies)
		{
			if (Enemy)
			{
				Cast<ADefaultEnemyCharacter>(Enemy)->TimeSystemCharacter->StopRevers();
			}
		}
		GetWorld()->GetTimerManager().ClearTimer(ManaRTEverythingTimer);
		RewindTimeEverythingStop();
	}
}


float AFPCharacter::GetXP()
{
	return CurrentXP;
}
float AFPCharacter::GetMana()
{
	return CurrentMana;
}

void AFPCharacter::ChangeManaRTOtherObject(FTimerHandle Timer, float ManaRTOtherObject)
{
	if (CurrentMana >= ManaRTOtherObject) {
		ChangeMana(CurrentMana - ManaRTOtherObject);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		RewindTimeObjectStop();
	}
}

void AFPCharacter::HoriMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}
void AFPCharacter::VertMove(float value)
{
	IsVertMove = value > 0.0f;
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFPCharacter::HoriRot(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}
void AFPCharacter::VertRot(float value)
{
	if (value)
	{
		float Rotation = Camera->GetRelativeRotation().Pitch + value;

		if (Rotation < 80 && Rotation > -80)
		{
			Camera->AddLocalRotation(FRotator(value, 0, 0));
		}
	}
}

APawn* AFPCharacter::GetPlayerPawn() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) return nullptr;
	return PlayerPawn;
}

UCharacterMovementComponent* AFPCharacter::GetCharacterMovementComponent() const
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast <UCharacterMovementComponent>(GetPlayerPawn()->GetMovementComponent());
	if (!CharacterMovementComponent) return nullptr;
	return CharacterMovementComponent;
}

void AFPCharacter::RayToSeeInteractiveItem()
{
	FHitResult* Hit = new FHitResult();
	FVector Start = Camera->GetComponentLocation();
	FVector End = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 400 + Start;

	GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Visibility);

	AActiveRegenItem* Item = Cast<AActiveRegenItem>(Hit->Actor);

	if (Item != LastItem && LastItem != nullptr)
	{
		LastItem->SetCustomDeapth(false);
	}
	if (!Item)
	{
		LastItem = nullptr;
		return;
	}
	else
	{
		if (LastItem != Item)
		{
			LastItem = Item;
			Item->SetCustomDeapth(true);
		}
	}
}

void AFPCharacter::StartRun()
{
	if (IsVertMove)
	{
		GetCharacterMovementComponent()->MaxWalkSpeed = SpeedRun;
		OnSprintPressed.Broadcast();
	}
}
void AFPCharacter::StopRun()
{
	GetCharacterMovementComponent()->MaxWalkSpeed = SpeedStep;
	OnSprintReleased.Broadcast();
}

void AFPCharacter::StartCrouch()
{
	if (IsVertMove) GetCharacterMovementComponent()->MaxWalkSpeed = SpeedCrouch;
	Crouch();
}
void AFPCharacter::StopCrouch()
{
	GetCharacterMovementComponent()->MaxWalkSpeed = SpeedStep;
	UnCrouch();
}

void AFPCharacter::UseHelthTube()
{
	if (CountHelthTube) 
	{
		ChangeXP(GetXP() + HelthReplenishment);
		CountHelthTube--;
	}
}
void AFPCharacter::UseManaTube()
{
	if (CountManaTube) 
	{
		ChangeMana(GetMana() + HelthReplenishment);
		CountManaTube--;
	}
}

void AFPCharacter::TakeItem()
{
	OnTakeItemPressed.Broadcast();
	FHitResult* Hit = new FHitResult();
	FVector Start = Camera->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 40;
	FVector End = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 1000 + Start;
	GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Visibility);

	AActiveRegenItem* TakingItem = Cast<AActiveRegenItem>(Hit->Actor);
	if (TakingItem) {
		switch (TakingItem->ItemID)
		{
		case 1:
			CountHelthTube++;
			TakingItem->Destroy();
			OnItemTook.Broadcast();
			break;
		case 2:
			CountManaTube++;
			TakingItem->Destroy();
			OnItemTook.Broadcast();
			break;
		default:
			break;
		}
	}
	
}

void AFPCharacter::RewindTimeYourStart()
{
	OnReversSelfPressed.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(ManaRTYourTimer, this, &AFPCharacter::ChangeManaRTYour, SpeedMana, true, 0);
	Camera->PostProcessSettings.VignetteIntensity = 1;
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
	Camera->PostProcessSettings.GrainIntensity = 0.8;
	Camera->PostProcessSettings.bOverride_GrainIntensity = true;
}

void AFPCharacter::RewindTimeYourStop()
{
	OnReversSelfReleased.Broadcast();
	TimeSystemCharacter->StopRevers();
	Camera->PostProcessSettings.VignetteIntensity = 0;
	Camera->PostProcessSettings.bOverride_VignetteIntensity = false;
	Camera->PostProcessSettings.GrainIntensity = 0;
	Camera->PostProcessSettings.bOverride_GrainIntensity = false;
	GetWorld()->GetTimerManager().ClearTimer(ManaRTYourTimer);
}

void AFPCharacter::RewindTimeObjectStart()
{
	OnReversObjectPressed.Broadcast();

	FHitResult* Hit = new FHitResult();
	FVector Start = Camera->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 50;
	FVector End = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 1000 + Start;
	GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Visibility);

	CatchedObject = Cast<AInteractiveItem>(Hit->Actor);
	if (CatchedObject)
	{
		GetWorld()->GetTimerManager().SetTimer(ManaRTObjectTimer, this, &AFPCharacter::ChangeManaRTObject, SpeedMana, true, 0);
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Pawn);
		CatchedEnemy = Cast<ADefaultEnemyCharacter>(Hit->Actor);
		if (CatchedEnemy)
		{
			GetWorld()->GetTimerManager().SetTimer(ManaRTObjectTimer, this, &AFPCharacter::ChangeManaRTObject, SpeedMana, true, 0);
		}
	}
}
void AFPCharacter::RewindTimeObjectStop()
{
	OnReversObjectReleased.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(ManaRTObjectTimer);
	if (CatchedObject)
	{
		CatchedObject->TimeSystem->StopRevers();
		CatchedObject = nullptr;
	}
	if (CatchedEnemy)
	{
		CatchedEnemy->TimeSystemCharacter->StopRevers();
		CatchedEnemy = nullptr;
	}
}

void AFPCharacter::RewindTimeEverythingStart()
{
	SphereCollision->GetOverlappingActors(InteractiveItems, AInteractiveItem::StaticClass());
	SphereCollision->GetOverlappingActors(DefaultEnemies, ADefaultEnemyCharacter::StaticClass());
	OnReversEvPressed.Broadcast();

	if (InteractiveItems.Num() > 0 || DefaultEnemies.Num() > 0) 
	{
		GetWorld()->GetTimerManager().SetTimer(ManaRTEverythingTimer, this, &AFPCharacter::ChangeManaRTEverything, SpeedMana, true, 0);
	}

}
void AFPCharacter::RewindTimeEverythingStop()
{
	OnReversEvReleased.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(ManaRTEverythingTimer);
	for (auto Item : InteractiveItems)
	{
		if (Item)
		{
			Cast<AInteractiveItem>(Item)->TimeSystem->StopRevers();
		}
	}
	for (auto Enemy : DefaultEnemies)
	{
		if (Enemy)
		{
			Cast<ADefaultEnemyCharacter>(Enemy)->TimeSystemCharacter->StopRevers();
		}
	}
	InteractiveItems.Empty();
	DefaultEnemies.Empty();
}


