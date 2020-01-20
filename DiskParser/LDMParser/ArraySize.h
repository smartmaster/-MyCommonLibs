#pragma once

#ifdef ARRAYSIZE
#undef ARRAYSIZE
#endif

template <class T, size_t N>
char (&ArraySize(T (&)[N]))[N];

#define ARRAYSIZE(x) sizeof(ArraySize(x))
