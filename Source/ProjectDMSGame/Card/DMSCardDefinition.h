// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/DataAsset.h"
#include "Common/DMSCommons.h"
#include "Common/DMSTimingCondition.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSCardDefinition.generated.h"


/**
 *  Non mutatable Card's original data / definition.
 * 
 * 	========================================
 *
	ī�� ������Ƽ�� ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UCardFragment>> Fragments;
	���·� ������Ƽ�� ����ȭ ���Ѽ� �ϴ� ���� �߰�...

	������ �󿡼� ��̿� �������� �ִ� ���� UCardFragment ���� Ŭ������ �� �ν��Ͻ��� ����鼭 ���� ������ �� �ְԵ�..
	-> �ʿ��� ������ ������Ƽ Ŭ������ �̾���̱� ����. 

 *
 * =========================================
 * 
 * �ֿ켱������ �ε��� ������ ��¿��� �����ϴ� �Ǵٸ� �����ʹ� �������ϸ� ���۷��̵Ǿ� ���� �ε��Ǳ⿡ ������ ����� ���� �����ʹ� ������ ���ȭ ���� �ʰ� UObj ���·� ����� �����ع����µ�.
 * �����ϰ� SoftPtr ������ �Ἥ �ε� ����ȭ�� �ʿ��� �κе� �ƴϰ� ( ��¥�� ī�� �ϳ��� ����ϸ� ���� �� �����ͼ� �ʿ��� �����̱⿡ ) 
 * ���� �߰��Ұ� ���ٸ� �� �ٸ� DataAsset ���������� ������ ���°� ������ ��.
 * (LyraPickup -> LyraInventory ����)
 * 
 */
class UDMSEffect_Cost;
class UDMSEffectNode_TypeBehavior;


UCLASS(Blueprintable, BlueprintType, Const, ClassGroup = (Card))
class PROJECTDMSGAME_API UDMSCardDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// ----------------------------------------------------------------------------------------- //
	// �⺻���� ī�� ���� ��ҵ��� �ΰ��ӳ����� ���� ���ɼ��� ���� �����غ�����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText CardName;

	// Using Data registry Connect with type behavior?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FName DefaultCardType; // ENUM?

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	//TSubclassOf<UDMSEffectNode_TypeBehavior> DefaultCardType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText EffectText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText FlavorText;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	//FBrush Image;
		
	// ----------------------------------------------------------------------------------------- //
	// DEPRECATED
	
	// UI �����ϴ� �κа� ���� �ʿ伺 ����. ( Ÿ�ٵ� �÷��� ���� ����Ʈ ������ Status condition check ���� )

	// Card's Effect.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	UDMSEffectSet* CardEffects;

	// Card's Play Cost -> visualizing info
	// �÷��� '��'�� �����ؾ� ��ҵ鿡 ���� �����͵鵵 ���⿡
	// ��� ������ �ڽ�Ʈ�� ������ �������� ��츦 ����� Node ��� Set
	// �ڽ�Ʈ ���� ��Ŀ� ���� ���? (���� ���� ������ ���� ������ �����ؾ� �� => ������ Ŭ����ȭ�� �´°�? Effect�� �Ļ�Ŭ���� )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	UDMSEffectSet* CostAndPlayCondition;

	// ----------------------------------------------------------------------------------------- //


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	TMap<FName,UDMSEffectSet*> CardEffectSets;
};
