// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerState.h"
#include "VIModularPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class VIGASCOMPANIONBRIDGE_API AVIModularPlayerState : public AGSCModularPlayerState
{
	GENERATED_BODY()
	
public:
	AVIModularPlayerState(const FObjectInitializer& OI);
};
