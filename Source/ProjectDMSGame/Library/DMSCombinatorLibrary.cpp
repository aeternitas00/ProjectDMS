// Fill out your copyright notice in the Description page of Project Settings.


#include "DMSCombinatorLibrary.h"

bool UDMSCombinatorLibrary::GetCombinatedResult(const TArray<bool>& iBoolArr, const FString& Combinator)
{

	return false;
}


//if (prevChar=='&') {
//	rv = rv && ParseCombinator(Values, Combinator.RightChop(i+1));
//}
//else if (prevChar == '|') {
//	rv = rv || ParseCombinator(Values, Combinator.RightChop(i + 1));
//}

bool UDMSCombinatorLibrary::ValidateCombinator(const FString& Combinator) /*const*/ {
	for (char c : Combinator) { if (!((c == '(') || (c == ')') || (c == '&') || (c == '|') || ((c <= '9') && (c >= '0')))) return false; }
	// deeper check set of () [] ~~

	return true;
}

