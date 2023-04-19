// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DMSSerializedAttribute.h"

FArchive& operator<<(FArchive& Ar, FDMSSerializedAttribute& Att)
{
	Ar << Att.AttributeTag << Att.Value;
	return Ar;
}
