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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // 태그 더 생각해보기
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
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute) // 태그 더 생각해보기
	float Value;

	/**
	 * 이 어트리뷰트가 고정 값을 갖지 않고 외부의 다른 값이나 함수를 통해 정해지는 지의 여부.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attribute)
	bool bIsDependOnOuter;

	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, UDMSAttribute*, Attribute);

	DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetDependentValue);

	/**
	 * bIsDependOnOuter가 참일 때 실제로 어떻게 구할지에 대한 델리게이트.( 여기에 바인딩해서 사용 )
	 */
	UPROPERTY()
	FGetDependentValue GetDependentValue;

	/**
	 * 해당 어트리뷰트가 변화되었을 때 호출 될 델리게이트.
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

