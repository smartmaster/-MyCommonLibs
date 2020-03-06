
#include "stdafx.h"
#include "test-param-stream.h"

#include "test-param-stream.h.Output.hpp"
#include "test-param-stream.h.xml.Output.hpp"

static void NOP() {}


int _tmain_stream(int argc, _TCHAR* argv[])
{
	CONST LONG BUFFER_SIZE = 1024 * 1024;
	CHeapPtr<BYTE> apBuffer;
	apBuffer.Allocate(BUFFER_SIZE);

	BEGIN_BLOCK(7);

	LPCTSTR file1 = TEXT("q:\\111.xml");
	LPCTSTR file2 = TEXT("q:\\222.xml");

	static const TCHAR ROOT_NAME[] = TEXT("my_hard_t_test");

	my_hard_array_t t1;
	t1.Init();

	t1.ToFile(ROOT_NAME, file1);

	my_hard_array_t t2;
	t2.FromFile(ROOT_NAME, file1);
	t2.ToFile(ROOT_NAME, file2);


	NOP();
	END_BLOCK(7);



	BEGIN_BLOCK(6);

	static const TCHAR ROOT_NAME[] = TEXT("my_hard_t_test");

	my_hard_array_t t1;
	t1.Init();

	XmlElement e1;
	t1.ToElement(ROOT_NAME, e1);

	CString strXml1;
	e1.ToString(strXml1);


	my_hard_array_t t2;
	t2.FromElement(ROOT_NAME, e1);

	XmlElement e2;
	t2.ToElement(ROOT_NAME, e2);

	CString strXml2;
	e2.ToString(strXml2);

	ATLASSERT(strXml1 == strXml2);


	NOP();
	END_BLOCK(6);

	BEGIN_BLOCK(5);

	my_hard_array_t t1;
	t1.Init();

	CONST TCHAR FileName[] = TEXT("E:\\123.bin");

	LONGLONG WrittenSize = -1;
	t1.WriteToFile(FileName, WrittenSize);


	my_hard_array_t t2;
	t2.ReadFromFile(FileName);

	NOP();
	END_BLOCK(5);


	BEGIN_BLOCK(4);

	my_hard_array_t t1;
	t1.Init();

	LONGLONG WrittenSize = -1;
	t1.WriteToBuffer(apBuffer.m_pData, BUFFER_SIZE, WrittenSize);


	my_hard_array_t t2;
	t2.ReadFromBuffer(apBuffer.m_pData, WrittenSize);

	NOP();
	END_BLOCK(4);


	return 0;

}