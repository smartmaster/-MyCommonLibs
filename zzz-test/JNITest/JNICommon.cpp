#include "stdafx.h"
#include "JNICommon.h"


#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);
#endif


namespace SmartLib
{

//<sonmi01>2016-9-5 ###???
C2JString::C2JString(CJavaVM * vm, LPCWSTR pwsz, int size) :
	m_vm(vm),
	m_str16(pwsz ? pwsz : L"")
{
	m_jstring = m_vm->AttachThread()->NewString((const jchar*)pwsz, size > 0 ? size : wcslen(pwsz));
}

C2JString::C2JString(CJavaVM * vm, LPCSTR psz) :
	m_vm(vm),
	m_str8(psz ? psz : "")
{
	m_jstring = m_vm->AttachThread()->NewStringUTF(psz);
}

jstring C2JString::JavaString()
{
	return m_jstring;
}

jstring C2JString::Detach()
{
	jstring ret = m_jstring;
	m_jstring = nullptr;
	return ret;
}

C2JString::~C2JString()
{
	Dispose();
}

VOID C2JString::Dispose()
{
	if (m_jstring)
	{
		m_vm->AttachThread()->DeleteLocalRef(m_jstring);
		m_jstring = nullptr;
	}
}

J2CString::J2CString(CJavaVM * vm, jstring jstr, bool bAttach) :
	m_vm(vm),
	m_jstring(jstr),
	m_bAttached(bAttach)
{
	if (nullptr == m_jstring)
	{
		return;
	}

	{
		int wsize = m_vm->AttachThread()->GetStringLength(m_jstring);
		const jchar * wsz = m_vm->AttachThread()->GetStringChars(m_jstring, nullptr);
		m_str16.assign((const WCHAR *)wsz, wsize);
		m_vm->AttachThread()->ReleaseStringChars(m_jstring, wsz);
		wsz = nullptr;
	}

	{
		int size8 = m_vm->AttachThread()->GetStringUTFLength(m_jstring);
		const char * sz8 = m_vm->AttachThread()->GetStringUTFChars(m_jstring, nullptr);
		m_str8.assign(sz8, size8);
		m_vm->AttachThread()->ReleaseStringUTFChars(m_jstring, sz8);
		sz8 = nullptr;
	}
}

LPCWSTR J2CString::GetString16()
{
	return m_str16.c_str();
}

LPCSTR J2CString::GetString8()
{
	return m_str8.c_str();
}

jstring J2CString::Detach()
{
	jstring ret = m_jstring;
	m_jstring = nullptr;
	return ret;
}

J2CString::~J2CString()
{
	Dispose();
}

VOID J2CString::Dispose()
{
	if (m_bAttached && m_jstring)
	{
		m_vm->AttachThread()->DeleteLocalRef(m_jstring);
		m_jstring = nullptr;
	}
}

CJavaObject::CJavaObject(CJavaVM * vm) :
	m_vm(vm)
{

}

//CJavaObject::CJavaObject()
//{
//
//}

VOID CJavaObject::NewObject(LPCSTR className)
{
	m_className = className;
	//////////////////////////////////////////////////////////////////////////
	m_class = m_vm->AttachThread()->FindClass(className);
	if (nullptr == m_class)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	jmethodID constructor = m_vm->AttachThread()->GetMethodID(m_class, "<init>", "()V");
	if (nullptr == constructor)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	m_object = m_vm->AttachThread()->NewObject(m_class, constructor);
	m_Attached = true;
}

VOID CJavaObject::NewObject(LPCSTR className, LPCSTR ctorSignatures, ...)
{
	m_className = className;
	//////////////////////////////////////////////////////////////////////////
	m_class = m_vm->AttachThread()->FindClass(className);
	if (nullptr == m_class)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	jmethodID constructor = m_vm->AttachThread()->GetMethodID(m_class, "<init>", ctorSignatures);
	if (nullptr == constructor)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	va_list vaList;
	va_start(vaList, ctorSignatures);
	m_object = m_vm->AttachThread()->NewObjectV(m_class, constructor, vaList);
	va_end(vaList);
	m_Attached = true;
}

VOID CJavaObject::SetObject(jobject myjobject, bool bGlobal, bool bAttach)
{
	m_class = m_vm->AttachThread()->GetObjectClass(myjobject);
	if (bGlobal)
	{
		jclass globalClass = (jclass)m_vm->AttachThread()->NewGlobalRef(m_class);
		m_vm->AttachThread()->DeleteLocalRef(m_class);
		m_class = globalClass;
	}
	m_object = myjobject;
	m_isGlobal = bGlobal;
	m_Attached = bAttach;

	CJavaObject::GetClassName(m_vm, m_class, m_className);
}

CJavaObject::~CJavaObject()
{
	Dispose();
}

void CJavaObject::MakeGlobal() //for multi thread
{
	if (!m_isGlobal)
	{
		jobject globalObject = m_vm->AttachThread()->NewGlobalRef(m_object);
		m_vm->AttachThread()->DeleteLocalRef(m_object);
		m_object = globalObject;
		m_isGlobal = true;

		jclass globalClass = (jclass)m_vm->AttachThread()->NewGlobalRef(m_class);
		m_vm->AttachThread()->DeleteLocalRef(m_class);
		m_class = globalClass;
	}
}

jobject CJavaObject::JavaObject()
{
	return m_object;
}

CJavaVM * CJavaObject::JavaVM()
{
	return m_vm;
}

jclass CJavaObject::JavaClass()
{
	return m_class;
}

//VOID CJavaObject::SetJavaVM(CJavaVM * vm)
//{
//	m_vm = vm;
//}

jobject CJavaObject::Detach()
{
	jobject ret = m_object;
	m_object = nullptr;
	return ret;
}

bool CJavaObject::IsGlobal()
{
	return m_isGlobal;
}

VOID CJavaObject::SetField(LPCSTR fieldName, LPCSTR objectName, jobject jobj)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, objectName);
	if (nullptr == fieldID)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	m_vm->AttachThread()->SetObjectField(m_object, fieldID, jobj);
}

VOID CJavaObject::SetField(LPCSTR fieldName, LPCWSTR value)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "Ljava/lang/String;");
	if (nullptr == fieldID)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	C2JString str(m_vm, value, -1);
	m_vm->AttachThread()->SetObjectField(m_object, fieldID, str.JavaString());
}

VOID CJavaObject::SetField(LPCSTR fieldName, jboolean val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "Z");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetBooleanField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jbyte val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "B");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetByteField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jchar val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "C");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetCharField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jshort val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "S");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetShortField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jint val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "I");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetIntField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jlong val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "J");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetLongField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jfloat val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "F");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetFloatField(m_object, fieldID, val);
}

VOID CJavaObject::SetField(LPCSTR fieldName, jdouble val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "D");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetDoubleField(m_object, fieldID, val);
}

jobject CJavaObject::GetField(LPCSTR fieldName, LPCSTR objectName, jobject & jobj)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, objectName);
	if (nullptr == fieldID)
	{
		return nullptr;
	}

	jobj = m_vm->AttachThread()->GetObjectField(m_object, fieldID);
	return jobj;
}

LPCWSTR CJavaObject::GetField(LPCSTR fieldName, std::wstring & value)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "Ljava/lang/String;");
	if (nullptr == fieldID)
	{
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	jstring jstr = (jstring)m_vm->AttachThread()->GetObjectField(m_object, fieldID);
	J2CString j2cstr(m_vm, jstr, true);
	value = j2cstr.GetString16();
	return value.c_str();
}

jboolean CJavaObject::GetField(LPCSTR fieldName, jboolean & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "Z");
	if (nullptr == fieldID)
	{
		return false;
	}

	val = m_vm->AttachThread()->GetBooleanField(m_object, fieldID);
	return val;
}

jbyte CJavaObject::GetField(LPCSTR fieldName, jbyte & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "B");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetByteField(m_object, fieldID);
	return val;
}

jchar CJavaObject::GetField(LPCSTR fieldName, jchar & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "C");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetCharField(m_object, fieldID);
	return val;
}

jshort CJavaObject::GetField(LPCSTR fieldName, jshort & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "S");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetShortField(m_object, fieldID);
	return val;
}

jint CJavaObject::GetField(LPCSTR fieldName, jint & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "I");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetIntField(m_object, fieldID);
	return val;
}

jlong CJavaObject::GetField(LPCSTR fieldName, jlong & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "J");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetLongField(m_object, fieldID);
	return val;
}

jfloat CJavaObject::GetField(LPCSTR fieldName, jfloat & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "F");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetFloatField(m_object, fieldID);
	return val;
}

jdouble CJavaObject::GetField(LPCSTR fieldName, jdouble & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetFieldID(m_class, fieldName, "D");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetDoubleField(m_object, fieldID);
	return val;
}

jmethodID CJavaObject::FindMethod(LPCSTR methodName, LPCSTR methodSigature)
{
	return m_vm->AttachThread()->GetMethodID(m_class, methodName, methodSigature);
}

void CJavaObject::CallMethod(LPCVOID /*nullret*/, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	m_vm->AttachThread()->CallVoidMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jobject & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallObjectMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jboolean & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallBooleanMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jbyte & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallByteMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jchar & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallCharMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jshort & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallShortMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jint & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallIntMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jlong & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallLongMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jfloat & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallFloatMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallMethod(jdouble & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallDoubleMethodV(m_object, methodID, args);
	va_end(args);
	//return ret;
}


//////////////////////////////////////////////////////////////////////////
//static
/////////////////////////////////////////////////////////////////////////
VOID CJavaObject::SetStaticField(LPCSTR fieldName, LPCSTR objectName, jobject jobj)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, objectName);
	if (nullptr == fieldID)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	m_vm->AttachThread()->SetStaticObjectField(m_class, fieldID, jobj);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, LPCWSTR value)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "Ljava/lang/String;");
	if (nullptr == fieldID)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	C2JString str(m_vm, value, -1);
	m_vm->AttachThread()->SetStaticObjectField(m_class, fieldID, str.JavaString());
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jboolean val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "Z");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticBooleanField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jbyte val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "B");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticByteField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jchar val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "C");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticCharField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jshort val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "S");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticShortField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jint val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "I");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticIntField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jlong val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "J");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticLongField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jfloat val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "F");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticFloatField(m_class, fieldID, val);
}

VOID CJavaObject::SetStaticField(LPCSTR fieldName, jdouble val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "D");
	if (nullptr == fieldID)
	{
		return;
	}

	m_vm->AttachThread()->SetStaticDoubleField(m_class, fieldID, val);
}

jobject CJavaObject::GetStaticField(LPCSTR fieldName, LPCSTR objectName, jobject & jobj)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, objectName);
	if (nullptr == fieldID)
	{
		return nullptr;
	}

	jobj = m_vm->AttachThread()->GetStaticObjectField(m_class, fieldID);
	return jobj;
}

LPCWSTR CJavaObject::GetStaticField(LPCSTR fieldName, std::wstring & value)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "Ljava/lang/String;");
	if (nullptr == fieldID)
	{
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	jstring jstr = (jstring)m_vm->AttachThread()->GetStaticObjectField(m_class, fieldID);
	J2CString j2cstr(m_vm, jstr, true);
	value = j2cstr.GetString16();
	return value.c_str();
}

jboolean CJavaObject::GetStaticField(LPCSTR fieldName, jboolean & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "Z");
	if (nullptr == fieldID)
	{
		return false;
	}

	val = m_vm->AttachThread()->GetStaticBooleanField(m_class, fieldID);
	return val;
}

jbyte CJavaObject::GetStaticField(LPCSTR fieldName, jbyte & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "B");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticByteField(m_class, fieldID);
	return val;
}

jchar CJavaObject::GetStaticField(LPCSTR fieldName, jchar & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "C");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticCharField(m_class, fieldID);
	return val;
}

jshort CJavaObject::GetStaticField(LPCSTR fieldName, jshort & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "S");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticShortField(m_class, fieldID);
	return val;
}

jint CJavaObject::GetStaticField(LPCSTR fieldName, jint & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "I");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticIntField(m_class, fieldID);
	return val;
}

jlong CJavaObject::GetStaticField(LPCSTR fieldName, jlong & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "J");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticLongField(m_class, fieldID);
	return val;
}

jfloat CJavaObject::GetStaticField(LPCSTR fieldName, jfloat & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "F");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticFloatField(m_class, fieldID);
	return val;
}

jdouble CJavaObject::GetStaticField(LPCSTR fieldName, jdouble & val)
{
	//////////////////////////////////////////////////////////////////////////
	jfieldID fieldID = m_vm->AttachThread()->GetStaticFieldID(m_class, fieldName, "D");
	if (nullptr == fieldID)
	{
		return 0;
	}

	val = m_vm->AttachThread()->GetStaticDoubleField(m_class, fieldID);
	return val;
}

jmethodID CJavaObject::FindStaticMethod(LPCSTR methodName, LPCSTR methodSigature)
{
	return m_vm->AttachThread()->GetStaticMethodID(m_class, methodName, methodSigature);
}


void CJavaObject::CallStaticMethod(LPCVOID /*nullret*/, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	m_vm->AttachThread()->CallStaticVoidMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jobject & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticObjectMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jboolean & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticBooleanMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jbyte & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticByteMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jchar & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticCharMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jshort & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticShortMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jint & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticIntMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jlong & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticLongMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jfloat & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticFloatMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

void CJavaObject::CallStaticMethod(jdouble & ret, jmethodID methodID, ...)
{
	va_list args;
	va_start(args, methodID);
	ret = m_vm->AttachThread()->CallStaticDoubleMethodV(m_class, methodID, args);
	va_end(args);
	//return ret;
}

VOID CJavaObject::Dispose()
{
	if (m_Attached)
	{
		if (m_object)
		{
			if (m_isGlobal)
			{
				m_vm->AttachThread()->DeleteGlobalRef(m_object);
			}
			else
			{
				m_vm->AttachThread()->DeleteLocalRef(m_object);
			}
			m_object = nullptr;
		}
	}

	if (m_class)
	{
		if (m_isGlobal)
		{
			m_vm->AttachThread()->DeleteGlobalRef(m_class);
		}
		else
		{
			m_vm->AttachThread()->DeleteLocalRef(m_class);
		}
		m_class = nullptr;
	}
}

void CJavaObject::GetClassName(CJavaVM * vm, jclass clsObj, std::string & className)
{
	jclass clsCls = vm->AttachThread()->GetObjectClass(clsObj);

	jmethodID methodId = vm->AttachThread()->GetMethodID(clsCls, "getName", "()Ljava/lang/String;");
	jstring jclassName = (jstring)vm->AttachThread()->CallObjectMethod(clsObj, methodId);

	J2CString j2cstr(vm, jclassName, true);
	className = j2cstr.GetString8();

	vm->AttachThread()->DeleteLocalRef(clsCls);
}

int CJavaVM::CreateJVM(LPCSTR libPath, LPCSTR * jars, int jarsCount, bool bDebug)
{
	CStringA libPathA;
	if (nullptr != libPath && 0 != libPath[0])
	{
		libPathA = "-Djava.library.path=";
		libPathA += libPath;
	}
	

	CStringA jarsListA = "-Djava.class.path=";
	for (int ii = 0; ii < jarsCount; ++ii)
	{
		jarsListA += jars[ii];
	}

	std::vector<JavaVMOption> options;
	JavaVMOption option;

	if (libPathA.GetLength())
	{
		ZeroMemory(&option, sizeof(option));
		option.optionString = libPathA.GetBuffer();
		options.push_back(option);
	}
	
	ZeroMemory(&option, sizeof(option));
	option.optionString = jarsListA.GetBuffer();
	options.push_back(option);

	if (bDebug)
	{
		static char debug[] = "-Xdebug";
		ZeroMemory(&option, sizeof(option));
		option.optionString = debug;
		options.push_back(option);

		static char debugConfig[] = "-Xrunjdwp:transport=dt_socket,address=25600,server=y,suspend=y";
		ZeroMemory(&option, sizeof(option));
		option.optionString = debugConfig;
		options.push_back(option);
	}

	JavaVMInitArgs vmInitArgs = { 0 };
	vmInitArgs.version = MY_JNI_VERSION; //JDK version. This indicates version 1.6
	vmInitArgs.nOptions = options.size();
	vmInitArgs.options = &options[0];
	vmInitArgs.ignoreUnrecognized = 0;

	JNIEnv * env = nullptr;
	int ret = JNI_CreateJavaVM(&m_jvm, (void**)&env, &vmInitArgs);
	m_jvmMainThreadId = GetCurrentThreadId();
	m_bJvmOwner = TRUE;
	return ret;
}

void CJavaVM::AttachJVM(JavaVM * jvm, DWORD jvmMainThreadId, BOOL bJvmOwner)
{
	m_jvm = jvm;
	m_jvmMainThreadId = jvmMainThreadId;
	m_bJvmOwner = bJvmOwner;
}

JNIEnv * CJavaVM::AttachThread()
{
	JNIEnv * env = nullptr;
	jint jret = m_jvm->GetEnv((void**)&env, MY_JNI_VERSION);
	if (JNI_OK != jret || nullptr == env)
	{
		jret = m_jvm->AttachCurrentThread((void **)&env, nullptr);
	}
	return env;
}

void CJavaVM::DetachThread()
{
	if (m_jvmMainThreadId != GetCurrentThreadId())
	{
		JNIEnv * env = nullptr;
		jint jret = m_jvm->GetEnv((void**)&env, MY_JNI_VERSION);
		if (JNI_OK == jret && nullptr != env)
		{
			jret = m_jvm->DetachCurrentThread();
		}
	}
}

CJavaVM::~CJavaVM()
{
	Dispose();
}


void CJavaVM::AppendJarsInDirectory(LPCWSTR dir, CStringA & strA)
{
	CStringW strRet;
	CStringW strDir = dir;
	if (strDir[strDir.GetLength() - 1] != L'\\')
	{
		strDir += L'\\';
	}
	CStringW fileJar = strDir;
	fileJar += L"*.jar";
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFild = ::FindFirstFileW(
		fileJar.GetString(),//_In_  LPCTSTR           lpFileName,
		&FindFileData//_Out_ LPWIN32_FIND_DATA lpFindFileData
	);
	if (INVALID_HANDLE_VALUE == hFild)
	{
		return;
	}

	do
	{
		strRet += strDir;
		strRet += FindFileData.cFileName;
		strRet += L';';
	} while (::FindNextFileW(hFild, &FindFileData));

	::FindClose(hFild);
	hFild = INVALID_HANDLE_VALUE;

	Utf16To8String(strRet, strA);
}

HRESULT CJavaVM::Utf16To8String(CONST CStringW & strUtf16, CStringA & strUtf8)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	int BufferSize = WideCharToMultiByte(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf16.GetString(),//_In_       LPCWSTR lpWideCharStr,
		-1,//_In_       int cchWideChar,
		NULL,//_Out_opt_  LPSTR lpMultiByteStr,
		0,//_In_       int cbMultiByte,
		NULL,//_In_opt_   LPCSTR lpDefaultChar,
		NULL//_Out_opt_  LPBOOL lpUsedDefaultChar
	);
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("WideCharToMultiByte"), hr, TEXT("[%s]"), strUtf16.GetString());
		LEAVE_BLOCK(0);
	}

	CONST INT DELTA = 4;
	LPSTR pBufferUtf8 = strUtf8.GetBuffer(BufferSize + DELTA);
	BufferSize = WideCharToMultiByte(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf16.GetString(),//_In_       LPCWSTR lpWideCharStr,
		-1,//_In_       int cchWideChar,
		pBufferUtf8,//_Out_opt_  LPSTR lpMultiByteStr,
		BufferSize,//_In_       int cbMultiByte,
		NULL,//_In_opt_   LPCSTR lpDefaultChar,
		NULL//_Out_opt_  LPBOOL lpUsedDefaultChar
	);
	strUtf8.ReleaseBuffer();
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("WideCharToMultiByte"), hr, TEXT("[%s]"), strUtf16.GetString());
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CJavaVM::Utf8To16String(CONST CStringA & strUtf8, CStringW & strUtf16)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	int BufferSize = MultiByteToWideChar(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf8.GetString(),//_In_       LPCSTR lpMultiByteStr,
		-1,//_In_       int cbMultiByte,
		NULL,//_Out_opt_  LPWSTR lpWideCharStr,
		0//_In_       int cchWideChar
	);
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("MultiByteToWideChar"), hr, TEXT("[%S]"), strUtf8.GetString());
		LEAVE_BLOCK(0);
	}

	CONST INT DELTA = 4;
	LPWSTR pBufferUtf16 = strUtf16.GetBuffer(BufferSize + DELTA);
	BufferSize = MultiByteToWideChar(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf8.GetString(),//_In_       LPCSTR lpMultiByteStr,
		-1,//_In_       int cbMultiByte,
		pBufferUtf16,//_Out_opt_  LPWSTR lpWideCharStr,
		BufferSize//_In_       int cchWideChar
	);
	strUtf16.ReleaseBuffer();
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("MultiByteToWideChar"), hr, TEXT("[%S]"), strUtf8.GetString());
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

void CJavaVM::Dispose()
{
	if (m_jvm)
	{
		if (m_bJvmOwner)
		{
			m_jvm->DestroyJavaVM();
		}
		m_jvm = nullptr;
	}
}

}//namespace SmartLib