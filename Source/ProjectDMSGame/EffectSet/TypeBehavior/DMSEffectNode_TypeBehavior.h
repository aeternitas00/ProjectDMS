// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSEffectNode_TypeBehavior.generated.h"

/**
 * 일단 존재의의 자체는 있긴하지만 타입 정보와 실제 타입별 이펙트를 매칭하는 방식
 * 추가로 타입은 맞지만 예외적인 케이스에 대한 구현을 어찌해야 할 지 생각 중.
 * 현재로써는 타입 자체는 이름으로써만 구분하고 타입 비해비어가 그냥 평범한 이펙트 처럼 각 카드마다 직접 넣는 형태.
 */
UCLASS(Abstract, Const, NotEditInlineNew)
class PROJECTDMSGAME_API UDMSEffectNode_TypeBehavior : public UDMSEffectNode
{
	GENERATED_BODY()
	
public:
	FName TypeName;
};
