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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // 태그 더 생각해보기
	float Value;
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSAttribute : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FName AttributeName;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // 태그 더 생각해보기
	float Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	bool bIsDependOnOuter;

	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetDependentValue);

	UPROPERTY()
	FGetDependentValue GetDependentValue;

	//DECLARE_DYNAMIC_DELEGATE_RetVal(float, FOnAttributeModified);
	//UPROPERTY()
	//FOnAttributeModified OnAttributeModified;

	UFUNCTION(BlueprintCallable)
	float GetValue() { return bIsDependOnOuter ? GetDependentValue.Execute(): Value;  } ;
	
	UFUNCTION(BlueprintCallable)
	void SetValue(float i) { if(bIsDependOnOuter) return; Value=i; };

	UFUNCTION(BlueprintCallable)
	bool ModAttribute(const FDMSAttributeModifier& Modifier);
};
