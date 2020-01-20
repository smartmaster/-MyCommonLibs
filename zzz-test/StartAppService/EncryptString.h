#pragma once

#include "stdafx.h"
#include <string>
using namespace std;

LONG EncryptString(CONST wstring & Password, CONST wstring & PlainText, wstring & EncryptedText);
LONG DecryptString(CONST wstring & Password, CONST wstring & EncryptedText, wstring & PlainText);