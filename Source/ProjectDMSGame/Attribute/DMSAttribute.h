// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "DMSAttribute.generated.h"



USTRUCT(BlueprintType)
struct FDMSAttributeModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FName AttributeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	EDMSModifierType ModifierType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // �±� �� �����غ���
	float Value;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeModifiedSignature, UDMSAttribute*, Attribute);

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSAttribute : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FName AttributeName;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // �±� �� �����غ���
	float Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	bool bIsDependOnOuter;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);

	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetDependentValue);

	UPROPERTY()
	FGetDependentValue GetDependentValue;

	/*
	 * 
	 */
	//DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);
	//DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAttributeModified, UDMSAttribute, Attribute, float, Delta);

	UPROPERTY()
	FOnAttributeModified OnAttributeModified;

	UFUNCTION(BlueprintCallable)
	float GetValue() { return bIsDependOnOuter ? GetDependentValue.Execute(): Value;  } ;
	
	UFUNCTION(BlueprintCallable)
	void SetValue(float i) { if(bIsDependOnOuter) return; Value=i; };

	UFUNCTION(BlueprintCallable)
	bool ModAttribute(const FDMSAttributeModifier& Modifier);

	UFUNCTION(BlueprintCallable)
	void BindOnModified(const FOnAttributeModifiedSignature& iDelegate);
};

