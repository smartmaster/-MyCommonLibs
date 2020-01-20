#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////
template<typename T>
VOID AllocateOneMore(vector<T> & vec)
{
	vec.resize(vec.size() + 1);
}

template<typename T>
T & Last(vector<T> & vec)
{
	return vec[vec.size() - 1];
}
//////////////////////////////////////////////////////////////////////////