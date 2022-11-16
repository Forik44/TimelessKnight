#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveRegenItem.generated.h"

UCLASS()
class STIMELESSKNIGHT_API AActiveRegenItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AActiveRegenItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		int ItemID;

	UFUNCTION(BlueprintCallable, Category = "Interactive")
		void SetCustomDeapth(bool value);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
