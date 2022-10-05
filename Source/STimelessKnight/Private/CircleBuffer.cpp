// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleBuffer.h"
#include <iostream>
#include <string>
using namespace std;

template<typename T>
CircleBuffer<T>::CircleBuffer(int Size)
{
	this->Size = Size;
	Buffer.resize(Size);
	this->CurrentPosition = -1;
	this->ActiveElem = 0;
}



template<typename T>
CircleBuffer<T>::~CircleBuffer()
{
	
}

template<typename T>
T CircleBuffer<T>::GetElem(int Index)
{
	return Buffer[Index];
}

template<typename T>
bool CircleBuffer<T>::Push(T Elem)
{
	CurrentPosition = (++CurrentPosition) % Size;
	Buffer[CurrentPosition] = Elem;
	if (ActiveElem < Size) {
		ActiveElem++;
	}
	cout << "\nPush " << Elem << " on: " << CurrentPosition << " Active elem = " << ActiveElem << endl;
	return true;
}

template<typename T>
T CircleBuffer<T>::Pop()
{
	if (this.Empty()) {
		T tmp = Buffer[CurrentPosition];
		CurrentPosition = ((CurrentPosition - 1) + Size) % Size;
		ActiveElem--;
		return tmp;
	}
	else {
		//return NULL;
	}
}

template<typename T>
bool CircleBuffer<T>::Empty() {
	return ActiveElem;
}

//template<typename T>
//void CircleBuffer<T>::Print()
//{
//	for (int i = CurrentPosition; i < Size; i++) {
//		UE_LOG(LogTemp, Log, TEXT("%f"),Buffer[i]);
//	}
//	for (int i = 0; i < CurrentPosition; i++) {
//		UE_LOG(LogTemp, Log, TEXT("%f"), Buffer[i]);
//	}
//}


