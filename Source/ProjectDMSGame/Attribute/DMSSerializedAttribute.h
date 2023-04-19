// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "DMSSerializedAttribute.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDMSSerializedAttribute
{
	GENERATED_BODY()

	FDMSSerializedAttribute() :AttributeTag(FGameplayTag::EmptyTag), Value(0.0f) {}
	FDMSSerializedAttribute(const FGameplayTag& Tag, const float& iValue) : AttributeTag(Tag), Value(iValue) {}
	
	/**
	 * Tag of attribute. sort of name
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTag AttributeTag;

	/**
	 * Current value of attribute.
	 * TODO :: Non-numeric attribute support?
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	float Value;


	friend FArchive& operator<<(FArchive& Ar, FDMSSerializedAttribute& Att);
};
