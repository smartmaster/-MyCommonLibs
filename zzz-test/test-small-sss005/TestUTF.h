#pragma once


#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

class TestUTF
{
public:
	static void prepare_file()
	{
		// UTF-8 data
		char utf8[] = { '\x7a',                       // latin small letter 'z' U+007a
			'\xe6','\xb0','\xb4',         // CJK ideograph "water"  U+6c34
			'\xf0','\x9d','\x84','\x8b' }; // musical sign segno U+1d10b
		std::ofstream fout("text.txt");
		fout.write(utf8, sizeof utf8);
	}
	static void test_file_utf16()
	{
		std::wifstream fin("text.txt");
		fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
		std::cout << "Read from file using UTF-8/UTF-16 codecvt\n";
		for (wchar_t c; fin >> c; )
			std::cout << std::hex << std::showbase << c << '\n';
	}
	static void test_file_ucs2()
	{
		std::wifstream fin("text.txt");
		fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
		std::cout << "Read from file using UTF-8/UCS2 codecvt\n";
		for (wchar_t c; fin >> c; )
			std::cout << std::hex << std::showbase << c << '\n';
	}
	static int Case0()
	{
		prepare_file();
		test_file_utf16();
		test_file_ucs2();
		return 0;
	}
};

