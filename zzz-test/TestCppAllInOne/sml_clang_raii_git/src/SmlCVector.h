#pragma once

#include <tchar.h>
#include <windows.h>


EXTERN_C_START

#ifndef SmlCVector_Initial_Size
#define SmlCVector_Initial_Size  32
#endif

typedef void* SML_DATA_PTR;


typedef struct  _SmlCVector
{
	int _size;
	int _capacity;
	SML_DATA_PTR* _ptr; /***CLEANUP***/
	SML_DATA_PTR _data[SmlCVector_Initial_Size];
}SmlCVector, * PSmlCVector;


void			SmlCVector_Ctor(PSmlCVector obj);
void			SmlCVector_Dtor(PSmlCVector obj);

int				SmlCVector_Capacity(PSmlCVector obj);
int				SmlCVector_Size(PSmlCVector obj);
SML_DATA_PTR	SmlCVector_Get(PSmlCVector obj, int index);
void			SmlCVector_Set(PSmlCVector obj, int index, SML_DATA_PTR data);
void			SmlCVector_Resize(PSmlCVector obj, int newsize);

SML_DATA_PTR*	SmlCVector_Ptr(PSmlCVector obj);
__declspec(noinline)
void			SmlCVector_Push(PSmlCVector obj, SML_DATA_PTR data);
SML_DATA_PTR	SmlCVector_Pop(PSmlCVector obj);



EXTERN_C_END