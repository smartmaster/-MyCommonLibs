
#include <windows.h>

#include "SmlCVector.h"


void SmlCVector_Ctor(PSmlCVector obj)
{
	ZeroMemory(obj, sizeof(SmlCVector));
	obj->_capacity = SmlCVector_Initial_Size;
	obj->_ptr = obj->_data;
}

void SmlCVector_Dtor(PSmlCVector obj)
{
	if (obj->_ptr != obj->_data)
	{
		free(obj->_ptr);
	}

	ZeroMemory(obj, sizeof(SmlCVector));
}


int SmlCVector_Capacity(PSmlCVector obj)
{
	return obj->_capacity;
}

int SmlCVector_Size(PSmlCVector obj)
{
	return obj->_size;
}


 SML_DATA_PTR* SmlCVector_Ptr(PSmlCVector obj)
{
	return obj->_size? obj->_ptr : NULL;
}

SML_DATA_PTR SmlCVector_Get(PSmlCVector obj, int index)
{
	return obj->_ptr[index];
}

void SmlCVector_Set(PSmlCVector obj, int index, SML_DATA_PTR data)
{
	obj->_ptr[index] = data;
}


void SmlCVector_Resize(PSmlCVector obj, int newsize)
{
	if (newsize >= obj->_capacity)
	{
		SML_DATA_PTR* newptr = calloc(newsize + SmlCVector_Initial_Size, sizeof(SML_DATA_PTR));
		CopyMemory(newptr, obj->_ptr, obj->_capacity * sizeof(SML_DATA_PTR));
		if (obj->_ptr != obj->_data)
		{
			free(obj->_ptr);
		}

		obj->_ptr = newptr;
		obj->_capacity = newsize + SmlCVector_Initial_Size;
	}

	obj->_size = newsize;
}


void SmlCVector_Push(PSmlCVector obj, SML_DATA_PTR data)
{
	SmlCVector_Resize(obj, obj->_size + 1);
	obj->_ptr[obj->_size - 1] = data;
}



SML_DATA_PTR SmlCVector_Pop(PSmlCVector obj)
{
	return obj->_ptr[--obj->_size];
}
