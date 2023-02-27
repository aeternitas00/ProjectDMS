// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/DMSBooleanCombinatorLibrary.h"

void UDMSBooleanCombinatorLibrary::ExecOperation(bool& iRv, const bool& NewValue, const uint8& Operator)
{
	if (Operator == '&') {
		iRv = iRv && NewValue;
	}
	else if (Operator == '|') {
		iRv = iRv || NewValue;
	}
}

bool UDMSBooleanCombinatorLibrary::ParseCombinator(const TArray<bool>& Values, const FString& Combinator) /*const*/
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
			ExecOperation(rv, ParseCombinator(Values, Combinator.RightChop(i + 1)), prevChar);
			Combinator.RightChop(i + 1).FindChar(')', i);
			break;
		case ')':// End SubOperation
		case '&':
		case '|':// Operator
			prevNum = FCString::Atoi(*prevNumStr);
			prevNumStr = "";
			ExecOperation(rv, Values[prevNum], prevChar);

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

		ExecOperation(rv, Values[prevNum], prevChar);
	}

	return rv;
}
