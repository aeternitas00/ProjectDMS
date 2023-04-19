// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Attributes to use in this game (numeric values to be used by game actors).
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Attribute/DMSSerializedAttribute.h"
#include "DMSAttribute.generated.h"


/**
 *	Attibute Modifier struct. Using in ModAtt Effect. 
 */
USTRUCT(BlueprintType)
struct FDMSAttributeModifier
{
	GENERATED_BODY()

public:
	/**
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTag AttributeTag;

	/**
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	EDMSModifierType ModifierType;

	/**
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // �±� �� �����غ���
	float Value;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeModifiedSignature, UDMSAttribute*, Attribute);

/*
 *	Class of singele numeric attribute.
 *	( EXPAND TO OTHER TYPES ATTRIBUTE? ) 
 */
UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSAttribute : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	FGameplayTag AttributeTag;
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // �±� �� �����غ���
	float Value;

	/**
	 * �� ��Ʈ����Ʈ�� ���� ���� ���� �ʰ� �ܺ��� �ٸ� ���̳� �Լ��� ���� �������� ���� ����.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	bool bIsDependOnOuter;

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);

	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetDependentValue);

	/**
	 * bIsDependOnOuter�� ���� �� ������ ��� �������� ���� ��������Ʈ.( ���⿡ ���ε��ؼ� ��� )
	 */
	UPROPERTY()
	FGetDependentValue GetDependentValue;

	/**
	 * �ش� ��Ʈ����Ʈ�� ��ȭ�Ǿ��� �� ȣ�� �� ��������Ʈ.
	 */
	UPROPERTY()
	FOnAttributeModified OnAttributeModified;

	/**
	 * Simple getter
	 */
	UFUNCTION(BlueprintCallable)
	float GetValue() { return bIsDependOnOuter ? GetDependentValue.Execute(): Value;  } ;
	
	/**
	 * Simple setter
	 */
	UFUNCTION(BlueprintCallable)
	void SetValue(float i) { if(bIsDependOnOuter) return; Value=i; };

	/** 
	 * Modify attribute value with Modifier
	 */
	UFUNCTION(BlueprintCallable)
	bool ModAttribute(const FDMSAttributeModifier& Modifier);

	/**
	 * Bind to OnAttributeModified
	 */
	UFUNCTION(BlueprintCallable)
	void BindOnModified(const FOnAttributeModifiedSignature& iDelegate);

	FDMSSerializedAttribute ToSerialized() { return FDMSSerializedAttribute(AttributeTag, Value); }

	virtual void Serialize(FArchive& Ar) override;
};

