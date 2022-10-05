// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "Containers/Array.h"
#include <vector>

/**
 * 
 */
template<typename T> class STIMELESSKNIGHT_API CircleBuffer
{
private:
	int Size;
	int CurrentPosition;
	int ActiveElem;
	std::vector<T> Buffer;

public:


	CircleBuffer<T>(int Size);
	~CircleBuffer();
	T GetElem(int Index);
	bool Push(T Elem);
	T Pop();
	bool Empty();
	//void Print();
};
