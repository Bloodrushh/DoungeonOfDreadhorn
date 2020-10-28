// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableInterface.h"

// Add default functionality here for any IDamageableInterface functions that are not pure virtual.
void IDamageableInterface::TakeDamage(int32 Amount, EAttack Attack)
{
}

bool IDamageableInterface::CanTakeDamage()
{
	return false;
}
