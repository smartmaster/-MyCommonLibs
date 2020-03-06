// JNITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JNICommon.h"
using namespace SmartLib;

static VOID TestCJavaObject(CJavaVM * vm);
void TestCreateVMNeutral(CJavaVM * vm);

int _tmain(int argc, TCHAR ** argv)
{
	bool bLoop = true;
	for (int ii = 0; bLoop && ii < 600; ++ii)
	{
		Sleep(1000);
	}

	CJavaVM javaVM;

	LPCSTR jrePath = nullptr;//"D:\\Unified Data Protection\\Common\\JRE\\bin";

	static LPCWSTR jarsDir[] =
	{
		L"D:\\Unified Data Protection\\Engine\\BIN",
		L"D:\\Unified Data Protection\\Engine\\BIN\\VMwarevCloudDirectorSDK\\5.5.0\\",
	};

	std::vector<CStringA> jars(_countof(jarsDir));
	std::vector<LPCSTR> vecJarsPsz;
	for (int ii = 0; ii < jars.size(); ++ ii)
	{
		CJavaVM::AppendJarsInDirectory(jarsDir[ii], jars[ii]);
		vecJarsPsz.push_back(jars[ii].GetString());
	}
	
	bool debugJVM = true;
	javaVM.CreateJVM(jrePath, &vecJarsPsz[0], vecJarsPsz.size(), debugJVM);

	int testCase = 1;
	switch (testCase)
	{
	case 2:
	{
		CJavaObject jo1(&javaVM);
		jo1.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;");

		CJavaObject job2(&javaVM);
		job2.SetObject(jo1.Detach(), false, true);
	}
	break;
	case 1:
	{
		TestCreateVMNeutral(&javaVM);
	}
	break;
	case 0:
	{
		TestCJavaObject(&javaVM);
	}
	break;
	}
	
    return 0;
}

void TestCreateVMNeutral(CJavaVM * vm)
{
	CJavaObject TestVMWareManager(vm);
	TestVMWareManager.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/TestVMWareManager;");
	TestVMWareManager.MakeGlobal();
	CALL_JAVA_METHOD(
		TestVMWareManager,
		"TestCreateVMNeutral",
		"()V",
		nullptr
	);

}

void GetSMTest1(CJavaObject & cjo)
{
	{
		std::wstring StrValue;
		cjo.GetField("StrValue", StrValue);
		cjo.GetField("StrValue", StrValue);
	}
	{
		jboolean BoolValue;
		cjo.GetField("BoolValue", BoolValue);
		cjo.GetField("BoolValue", BoolValue);
	}
	{
		jbyte ByteValue;
		cjo.GetField("ByteValue", ByteValue);
		cjo.GetField("ByteValue", ByteValue);
	}
	{
		jchar CharValue;
		cjo.GetField("CharValue", CharValue);
		cjo.GetField("CharValue", CharValue);
	}

	{
		jshort ShortValue;
		cjo.GetField("ShortValue", ShortValue);
		cjo.GetField("ShortValue", ShortValue);
	}
	{
		jint IntValue;
		cjo.GetField("IntValue", IntValue);
		cjo.GetField("IntValue", IntValue);
	}
	{
		jlong LongValue;
		cjo.GetField("LongValue", LongValue);
		cjo.GetField("LongValue", LongValue);
	}
	{
		jfloat FloatValue;
		cjo.GetField("FloatValue", FloatValue);
		cjo.GetField("FloatValue", FloatValue);
	}
	{
		jdouble DoubleValue;
		cjo.GetField("DoubleValue", DoubleValue);
		cjo.GetField("DoubleValue", DoubleValue);
	}
}
void GetSMTest2(CJavaVM * vm, CJavaObject & cjo)
{
	{
		jobject jobj = nullptr;
		cjo.GetField("SMTest1Value", "Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;", jobj);
		CJavaObject cjo1(vm);
		cjo1.SetObject(jobj, false, true);
		GetSMTest1(cjo1);
	}


	{
		std::wstring StrValue;
		cjo.GetField("StrValue", StrValue);
		cjo.GetField("StrValue", StrValue);
	}
	{
		jboolean BoolValue;
		cjo.GetField("BoolValue", BoolValue);
		cjo.GetField("BoolValue", BoolValue);
	}
	{
		jbyte ByteValue;
		cjo.GetField("ByteValue", ByteValue);
		cjo.GetField("ByteValue", ByteValue);
	}
	{
		jchar CharValue;
		cjo.GetField("CharValue", CharValue);
		cjo.GetField("CharValue", CharValue);
	}

	{
		jshort ShortValue;
		cjo.GetField("ShortValue", ShortValue);
		cjo.GetField("ShortValue", ShortValue);
	}
	{
		jint IntValue;
		cjo.GetField("IntValue", IntValue);
		cjo.GetField("IntValue", IntValue);
	}
	{
		jlong LongValue;
		cjo.GetField("LongValue", LongValue);
		cjo.GetField("LongValue", LongValue);
	}
	{
		jfloat FloatValue;
		cjo.GetField("FloatValue", FloatValue);
		cjo.GetField("FloatValue", FloatValue);
	}
	{
		jdouble DoubleValue;
		cjo.GetField("DoubleValue", DoubleValue);
		cjo.GetField("DoubleValue", DoubleValue);
	}
}

void SetSMTest1(CJavaObject & cjo, int base)
{
	{
		CStringW str;
		str.Format(L"%d", base);
		std::wstring StrValue = str.GetString();
		cjo.SetField("StrValue", StrValue.c_str());
		cjo.SetField("StrValue", StrValue.c_str());
	}
	{
		jboolean BoolValue = base + 1;
		cjo.SetField("BoolValue", BoolValue);
		cjo.SetField("BoolValue", BoolValue);
	}
	{
		jbyte ByteValue = base + 2;
		cjo.SetField("ByteValue", ByteValue);
		cjo.SetField("ByteValue", ByteValue);
	}
	{
		jchar CharValue = base + 3;
		cjo.SetField("CharValue", CharValue);
		cjo.SetField("CharValue", CharValue);
	}

	{
		jshort ShortValue = base + 4;
		cjo.SetField("ShortValue", ShortValue);
		cjo.SetField("ShortValue", ShortValue);
	}
	{
		jint IntValue = base + 5;
		cjo.SetField("IntValue", IntValue);
		cjo.SetField("IntValue", IntValue);
	}
	{
		jlong LongValue = base + 6;
		cjo.SetField("LongValue", LongValue);
		cjo.SetField("LongValue", LongValue);
	}
	{
		jfloat FloatValue = (jfloat)(base + 7);
		cjo.SetField("FloatValue", FloatValue);
		cjo.SetField("FloatValue", FloatValue);
	}
	{
		jdouble DoubleValue = base + 8;
		cjo.SetField("DoubleValue", DoubleValue);
		cjo.SetField("DoubleValue", DoubleValue);
	}
}

void SetSMTest2(CJavaVM * vm, CJavaObject & cjo, int base)
{
	{
		CJavaObject cjo1(vm);
		cjo1.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;", "(I)V", base);
		cjo.SetField("SMTest1Value", "Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;", cjo1.JavaObject());
	}

	{
		CStringW str;
		str.Format(L"%d", base);
		std::wstring StrValue = str.GetString();
		cjo.SetField("StrValue", StrValue.c_str());
		cjo.SetField("StrValue", StrValue.c_str());
	}
	{
		jboolean BoolValue = base + 1;
		cjo.SetField("BoolValue", BoolValue);
		cjo.SetField("BoolValue", BoolValue);
	}
	{
		jbyte ByteValue = base + 2;
		cjo.SetField("ByteValue", ByteValue);
		cjo.SetField("ByteValue", ByteValue);
	}
	{
		jchar CharValue = base + 3;
		cjo.SetField("CharValue", CharValue);
		cjo.SetField("CharValue", CharValue);
	}

	{
		jshort ShortValue = base + 4;
		cjo.SetField("ShortValue", ShortValue);
		cjo.SetField("ShortValue", ShortValue);
	}
	{
		jint IntValue = base + 5;
		cjo.SetField("IntValue", IntValue);
		cjo.SetField("IntValue", IntValue);
	}
	{
		jlong LongValue = base + 6;
		cjo.SetField("LongValue", LongValue);
		cjo.SetField("LongValue", LongValue);
	}
	{
		jfloat FloatValue = (jfloat)(base + 7);
		cjo.SetField("FloatValue", FloatValue);
		cjo.SetField("FloatValue", FloatValue);
	}
	{
		jdouble DoubleValue = base + 8;
		cjo.SetField("DoubleValue", DoubleValue);
		cjo.SetField("DoubleValue", DoubleValue);
	}
}






static void NOOP(int i) { i = i; return; }

void TestMethod1(CJavaObject & cjo1)
{
	{
		jmethodID mid = cjo1.FindMethod("setStrValue", "(Ljava/lang/String;)V");
		C2JString c2jstr(cjo1.JavaVM(), "hello world");
		cjo1.CallMethod(nullptr, mid, c2jstr.JavaString());

		mid = cjo1.FindMethod("getStrValue", "()Ljava/lang/String;");
		jobject jstr = nullptr;
		cjo1.CallMethod(jstr, mid);
		J2CString j2cstr(cjo1.JavaVM(), (jstring)jstr, true);
		NOOP(0);
	}

	{
		jmethodID mid = cjo1.FindMethod("setBoolValue", "(Z)V");
		cjo1.CallMethod(nullptr, mid, true);

		mid = cjo1.FindMethod("isBoolValue", "()Z");
		jboolean ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}


	{
		jmethodID mid = cjo1.FindMethod("setByteValue", "(B)V");
		cjo1.CallMethod(nullptr, mid, 100);

		mid = cjo1.FindMethod("getByteValue", "()B");
		jbyte ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}


	{
		jmethodID mid = cjo1.FindMethod("setCharValue", "(C)V");
		cjo1.CallMethod(nullptr, mid, 101);

		mid = cjo1.FindMethod("getCharValue", "()C");
		jchar ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}

	{
		jmethodID mid = cjo1.FindMethod("setShortValue", "(S)V");
		cjo1.CallMethod(nullptr, mid, 102);

		mid = cjo1.FindMethod("getShortValue", "()S");
		jshort ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo1.FindMethod("setIntValue", "(I)V");
		cjo1.CallMethod(nullptr, mid, 103);

		mid = cjo1.FindMethod("getIntValue", "()I");
		jint ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo1.FindMethod("setLongValue", "(J)V");
		cjo1.CallMethod(nullptr, mid, 104);

		mid = cjo1.FindMethod("getLongValue", "()J");
		jlong ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo1.FindMethod("setFloatValue", "(F)V");
		cjo1.CallMethod(nullptr, mid, 105.00F);

		mid = cjo1.FindMethod("getFloatValue", "()F");
		jfloat ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo1.FindMethod("setDoubleValue", "(D)V");
		cjo1.CallMethod(nullptr, mid, 106.00);

		mid = cjo1.FindMethod("getDoubleValue", "()D");
		jdouble ret;
		cjo1.CallMethod(ret, mid);
		NOOP(0);
	}
	//{
	//	jmethodID mid = cjo1.FindMethod("xxx", "xxx");
	//	cjo1.CallMethod(nullptr, mid, xxx);

	//	mid = cjo1.FindMethod("xxx", "xxx");
	//	xxx ret = cjo1.CallxxxMethod(mid);
	//	NOOP(0);
	//}
}

void TestMethod2(CJavaObject & cjo2)
{
	{
		CJavaObject cjo1(cjo2.JavaVM());
		cjo1.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;", "(I)V", 8000);

		jmethodID mid = cjo2.FindMethod("setSMTest1Value", "(Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;)V");
		cjo2.CallMethod(nullptr, mid, cjo1.JavaObject());

		//CALL_JAVA_METHOD(cjo2, "setSMTest1Value", "(Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;)V", nullptr, cjo1.JavaObject());
	}
	{
		jmethodID mid = cjo2.FindMethod("getSMTest1Value", "()Lcom/ca/arcflash/ha/vmwaremanager/SMTest1;");
		jobject ret;
		cjo2.CallMethod(ret, mid);
		CJavaObject cjo1(cjo2.JavaVM());
		cjo1.SetObject(ret, false, true);
		TestMethod1(cjo1);
		NOOP(0);
	}

	{
		jmethodID mid = cjo2.FindMethod("setStrValue", "(Ljava/lang/String;)V");
		C2JString c2jstr(cjo2.JavaVM(), "hello world");
		cjo2.CallMethod(nullptr, mid, c2jstr.JavaString());

		mid = cjo2.FindMethod("getStrValue", "()Ljava/lang/String;");
		jobject jstr;
		cjo2.CallMethod(jstr, mid);
		J2CString j2cstr(cjo2.JavaVM(), (jstring)jstr, true);
		NOOP(0);
	}

	{
		jmethodID mid = cjo2.FindMethod("setBoolValue", "(Z)V");
		cjo2.CallMethod(nullptr, mid, true);

		mid = cjo2.FindMethod("isBoolValue", "()Z");
		jboolean ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}


	{
		jmethodID mid = cjo2.FindMethod("setByteValue", "(B)V");
		cjo2.CallMethod(nullptr, mid, 100);

		mid = cjo2.FindMethod("getByteValue", "()B");
		jbyte ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}


	{
		jmethodID mid = cjo2.FindMethod("setCharValue", "(C)V");
		cjo2.CallMethod(nullptr, mid, 101);

		mid = cjo2.FindMethod("getCharValue", "()C");
		jchar ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}

	{
		jmethodID mid = cjo2.FindMethod("setShortValue", "(S)V");
		cjo2.CallMethod(nullptr, mid, 102);

		mid = cjo2.FindMethod("getShortValue", "()S");
		jshort ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo2.FindMethod("setIntValue", "(I)V");
		cjo2.CallMethod(nullptr, mid, 103);

		mid = cjo2.FindMethod("getIntValue", "()I");
		jint ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo2.FindMethod("setLongValue", "(J)V");
		cjo2.CallMethod(nullptr, mid, 104);

		mid = cjo2.FindMethod("getLongValue", "()J");
		jlong ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo2.FindMethod("setFloatValue", "(F)V");
		cjo2.CallMethod(nullptr, mid, 105.00F);

		mid = cjo2.FindMethod("getFloatValue", "()F");
		jfloat ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}
	{
		jmethodID mid = cjo2.FindMethod("setDoubleValue", "(D)V");
		cjo2.CallMethod(nullptr, mid, 106.00);

		mid = cjo2.FindMethod("getDoubleValue", "()D");
		jdouble ret;
		cjo2.CallMethod(ret, mid);
		NOOP(0);
	}
	//{
	//	jmethodID mid = cjo1.FindMethod("xxx", "xxx");
	//	cjo1.CallMethod(nullptr, mid, xxx);

	//	mid = cjo1.FindMethod("xxx", "xxx");
	//	xxx ret = cjo1.CallxxxMethod(mid);
	//	NOOP(0);
	//}
}


static void TestStatics(CJavaVM * vm)
{
	CJavaObject jobj(vm);
	jobj.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest2;");

	std::wstring desc;
	jobj.GetStaticField("Description", desc);
	jobj.SetStaticField("Description", L"this is JNI test version 2");
	std::wstring desc2;
	jobj.GetStaticField("Description", desc2);


	jmethodID midGet = jobj.FindStaticMethod("GetDescription", "()Ljava/lang/String;");
	jobject jdesc = nullptr;
	jobj.CallStaticMethod(jdesc, midGet);
	J2CString cdesc(vm, (jstring)jdesc, true);

	jmethodID midSet = jobj.FindStaticMethod("SetDescription", "(Ljava/lang/String;)V");
	C2JString c2jdesc(vm, "this is JNI test version 3");
	jobj.CallStaticMethod(nullptr, midSet, c2jdesc.JavaString());

	jobject jdesc2 = nullptr;
	jobj.CallStaticMethod(jdesc2, midGet);
	J2CString cdesc2(vm, (jstring)jdesc2, true);

	NOOP(0);
}

static void TestJavaVectorArray(CJavaVM * vm)
{
	//////////////////////////////////////////////////////////////////////////
	std::vector<jobject> vecJobj;
	for (int ii = 0; ii < 26; ++ ii)
	{
		CString str;
		str.AppendChar(L'A' + ii);
		C2JString c2jstr(vm, str.GetString(), str.GetLength());
		vecJobj.push_back((jobject)(c2jstr.Detach()));
	}

	CJavaArray_jobject objArray(vm, "Ljava/lang/String;");
	objArray.Set(vecJobj);

	CJavaArray_jobject::ReleaseVectorJobject(vm, vecJobj);


	//////////////////////////////////////////////////////////////////////////
	std::vector<jobject> vecGet;
	objArray.Get(vecGet);

	for (int ii = 0; ii < vecGet.size(); ++ ii)
	{
		J2CString j2cstr(vm, (jstring)vecGet[ii], true); //attached, no need to CJavaArray_jobject::Release
		_ftprintf_s(stdout, TEXT("[%02d] %s") TEXT("\r\n"), ii, j2cstr.GetString16());
	}
	vecGet.clear();

	//////////////////////////////////////////////////////////////////////////
	std::vector<jdouble> vecjdoubleSet;
	for (int ii = 0; ii < 10; ++ii)
	{
		vecjdoubleSet.push_back(10000.00 + ii);
	}

	CJavaArray_jdouble doubleArray(vm);
	doubleArray.Set(vecjdoubleSet);

	//////////////////////////////////////////////////////////////////////////
	std::vector<jdouble> vecjdoubleGet;
	doubleArray.Get(vecjdoubleGet);

	for (int ii = 0; ii < vecjdoubleGet.size(); ++ii)
	{
		_ftprintf_s(stdout, TEXT("[%02d] %f") TEXT("\r\n"), ii, vecjdoubleGet[ii]);
	}

}

static void TestJavaArray(CJavaVM * vm)
{
	//////////////////////////////////////////////////////////////////////////
	CJavaArray_jobject objArray(vm, "Ljava/lang/String;");
	objArray.New(10);

	for (int ii = 0; ii < objArray.GetLength(); ++ii)
	{
		CString str;
		str.AppendChar(L'A' + ii);
		C2JString jstr(vm, str.GetString(), str.GetLength());
		objArray.Set(ii, jstr.JavaString());
	}

	for (int ii = 0; ii < objArray.GetLength(); ++ii)
	{
		jobject jobj = objArray.Get(ii);
		J2CString cstr(vm, (jstring)jobj, true);
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), cstr.GetString16());
	}


	//////////////////////////////////////////////////////////////////////////
	CJavaArray_jint intArray(vm);
	intArray.New(10);

	for (int ii = 0; ii < intArray.GetLength(); ++ii)
	{
		intArray.Set(ii, ii);
	}

	int val = -1;
	for (int ii = 0; ii < intArray.GetLength(); ++ii)
	{
		val = intArray.Get(ii);
		_ftprintf_s(stdout, TEXT("%d") TEXT("\r\n"), val);
	}

	//////////////////////////////////////////////////////////////////////////
	CJavaArray_jint intintArray2(vm);
	intintArray2.SetArray(intArray.Detach(), true);
	val = -1;
	for (int ii = 0; ii < intintArray2.GetLength(); ++ii)
	{
		val = intintArray2.Get(ii);
		_ftprintf_s(stdout, TEXT("%d") TEXT("\r\n"), val);
	}

}

static void TestGlobal(CJavaVM * vm)
{
	//////////////////////////////////////////////////////////////////////////
	CJavaObject localObj(vm);
	localObj.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest2;");
	localObj.MakeGlobal();

	//////////////////////////////////////////////////////////////////////////
	CJavaObject glocalObj(vm);
	glocalObj.SetObject(localObj.Detach(), localObj.IsGlobal(), true); 

	TestMethod2(glocalObj);
}

static VOID TestMacro(CJavaVM * vm)
{
	CJavaObject jo(vm);
	jo.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest2;");

	//////////////////////////////////////////////////////////////////////////
	CALL_JAVA_METHOD(jo, "setDoubleValue", "(D)V", nullptr, 999.00);

	jdouble ret = 0;
	CALL_JAVA_METHOD(jo, "getDoubleValue", "()D", ret);

	//////////////////////////////////////////////////////////////////////////
	C2JString c2jstr(vm, "hello this is a JNI call ......");
	CALL_JAVA_STATIC_METHOD(jo, "SetDescription", "(Ljava/lang/String;)V", nullptr, c2jstr.JavaString());

	jobject oret = nullptr;
	CALL_JAVA_STATIC_METHOD(jo, "GetDescription", "()Ljava/lang/String;", oret);

	J2CString j2cstr(vm, (jstring)oret, true);
	NOOP(0);
}


static void TestV2P(CJavaVM * vm)
{
	CJavaObject v2p(vm);
	C2JString esxVC(vm, "liuke-2012r2-vc");
	C2JString userName(vm, "hbbuadmin");
	C2JString password(vm, "c@123456");
	C2JString protocol(vm, "https");
	CJavaObject ignoreCert(vm);
	ignoreCert.NewObject("Ljava/lang/Boolean;", "(Z)V", true);
	CJavaObject viPort(vm);
	viPort.NewObject("Ljava/lang/Long;", "(J)V", 443);
	v2p.NewObject(
		"Lcom/ca/arcflash/ha/vmwaremanager/V2P_Export;", 
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/Boolean;Ljava/lang/Long;)V",
		esxVC.JavaString(),
		userName.JavaString(),
		password.JavaString(),
		protocol.JavaString(),
		ignoreCert.JavaObject(),
		viPort.JavaObject()
		);
	v2p.MakeGlobal();

	jobject esxNodeArray = nullptr;
	CALL_JAVA_METHOD(
		v2p,
		"getESXNodeList",
		"()[Lcom/ca/arcflash/ha/vmwaremanager/ESXNode;",
		esxNodeArray
	);

	CJavaArray_jobject javaArray(vm, "Lcom/ca/arcflash/ha/vmwaremanager/ESXNode;");
	javaArray.SetArray((jarray)esxNodeArray, true); esxNodeArray = nullptr;

	std::vector<jobject> vecESXNode; /***CLEANUP***/
	javaArray.Get(vecESXNode);

	for (int ii = 0; ii < vecESXNode.size(); ++ ii)
	{
		CJavaObject esxnode(vm);
		esxnode.SetObject(vecESXNode[ii], false, false);
		std::wstring esxName;
		std::wstring  dcDisplayName;
		std::wstring  dcName;
		std::wstring  esxVersion;
		
		esxnode.GetField("esxName", esxName);
		esxnode.GetField("dcDisplayName", dcDisplayName);
		esxnode.GetField("dcName", dcName);
		esxnode.GetField("esxVersion", esxVersion);

		_ftprintf_s(stdout, TEXT("esxName=%s") TEXT("\r\n"), esxName.c_str());
		_ftprintf_s(stdout, TEXT("dcDisplayName=%s") TEXT("\r\n"), dcDisplayName.c_str());
		_ftprintf_s(stdout, TEXT("dcName=%s") TEXT("\r\n"), dcName.c_str());
		_ftprintf_s(stdout, TEXT("esxVersion=%s") TEXT("\r\n"), esxVersion.c_str());
		_ftprintf_s(stdout, TEXT("") TEXT("\r\n"));

		
		NOOP(0);
	}

	CJavaArray_jobject::ReleaseVectorJobject(vm, vecESXNode); /***CLEANUP***/


	CALL_JAVA_METHOD(v2p, "disconnectESX", "()V", nullptr);
}

static VOID TestCJavaObject(CJavaVM * vm)
{
	{
		TestV2P(vm);
	}
	{
		TestMacro(vm);
	}
	{
		TestJavaVectorArray(vm);
	}
	{
		TestGlobal(vm);
	}
	{
		TestJavaArray(vm);
	}

	{
		TestStatics(vm);
	}
	{
		CJavaObject cjo(vm);
		cjo.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest2;");
		GetSMTest2(vm, cjo);
		SetSMTest2(vm, cjo, 200000);
		GetSMTest2(vm, cjo);
		TestMethod2(cjo);
	}

	{
		CJavaObject cjo(vm);
		cjo.NewObject("Lcom/ca/arcflash/ha/vmwaremanager/SMTest2;", "(I)V", 10000);
		GetSMTest2(vm, cjo);
		SetSMTest2(vm, cjo, 900000);
		GetSMTest2(vm, cjo);
		TestMethod2(cjo);
	}
}