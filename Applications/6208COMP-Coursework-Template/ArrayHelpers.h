#pragma once
#include <vector>
#include "Vector2f.h"

using namespace Glyph3;

class ArrayHelper
{
public:
	template<typename Value>
	static Value GetElementFromFlattenedArray(std::vector<Value> array, Vector2f position, Vector2f arrayDimensions);

	template<typename Value>
	static void SetElementFromFlattenedArray(std::vector<Value> &array, Value value, Vector2f position, Vector2f arrayDimensions);

	static int WrapArrayIndex(int arraySize, int value);
};

//Source: https://codereview.stackexchange.com/questions/57923/index-into-array-as-if-it-is-circular
inline int ArrayHelper::WrapArrayIndex(int arraySize, int value)
{
	if (value < 0)
	{
		value = arraySize + (value % arraySize);
	}
	else
	{
		value = value % arraySize;
	}

	return value;
}

template <typename Value>
Value ArrayHelper::GetElementFromFlattenedArray(std::vector<Value> array, Vector2f position, Vector2f arrayDimensions)
{
	return array[arrayDimensions.x * position.y + position.x];
}

template <typename Value>
void ArrayHelper::SetElementFromFlattenedArray(std::vector<Value>& array, Value value, Vector2f position, Vector2f arrayDimensions)
{
	array[arrayDimensions.x * position.y + position.x] = value;
}
