// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.


#include "VIModularPlayerState.h"
#include "GAS/VIAbilitySystemComponent.h"

AVIModularPlayerState::AVIModularPlayerState(const FObjectInitializer& OI)
	: Super(OI.SetDefaultSubobjectClass<UVIAbilitySystemComponent>(TEXT("AbilitySystemComponent")))
{

}
