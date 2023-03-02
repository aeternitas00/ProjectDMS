// Fill out your copyright notice in the Description page of Project Settings.


#include "DMSCombinatorLibrary.h"

void UDMSCombinatorLibrary::ExecOperation(bool& iRv, const bool& NewValue, const uint8& Operator)
{
	if (Operator == '&') {
		iRv = iRv && NewValue;
	}
	else if (Operator == '|') {
		iRv = iRv || NewValue;
	}
}
bool UDMSCombinatorLibrary::ParseCombinator(const TArray<bool>& iBoolArr, const FString& Combinator)
{
	// "A&(B|C)&F"

	bool rv = true;
	uint8 prevChar = '&';
	FString prevNumStr = "";
	int prevNum = -1;
	for (int i = 0; i < Combinator.Len(); i++)
	{
		switch (Combinator[i]) {
		case '(':// Start SubOperation
			ExecOperation(rv, ParseCombinator(iBoolArr, Combinator.RightChop(i + 1)), prevChar);
			Combinator.RightChop(i + 1).FindChar(')', i);
			break;
		case ')':// End SubOperation
		case '&':
		case '|':// Operator
			prevNum = FCString::Atoi(*prevNumStr);
			prevNumStr = "";
			ExecOperation(rv, iBoolArr[prevNum], prevChar);

			if (Combinator[i] == ')') return rv;
			prevChar = Combinator[i];
			break;
		default://case '0' ... '9':
			if (Combinator[i] >= '0' && Combinator[i] <= '9') prevNumStr += Combinator[i];
			break;
		}
	}

	if (prevNumStr != "") {
		prevNum = FCString::Atoi(*prevNumStr);
		prevNumStr = "";

		ExecOperation(rv, iBoolArr[prevNum], prevChar);
	}

	return rv;
}

bool UDMSCombinatorLibrary::ValidateCombinator(const FString& Combinator) /*const*/ {
	for (char c : Combinator) { if (!((c == '(') || (c == ')') || (c == '&') || (c == '|') || ((c <= '9') && (c >= '0')))) return false; }
	// deeper check set of () [] ~~

	return true;
}


//if (prevChar=='&') {
//	rv = rv && ParseCombinator(Values, Combinator.RightChop(i+1));
//}
//else if (prevChar == '|') {
//	rv = rv || ParseCombinator(Values, Combinator.RightChop(i + 1));
//}