#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <vector>

#include "jni.h"

namespace SmartLib
{

#define MY_JNI_VERSION JNI_VERSION_1_8

class CJavaVM
{
private:
	CJavaVM(CONST CJavaVM &);
	CONST CJavaVM & operator =(CONST CJavaVM &);

private:
	JavaVM * m_jvm{ nullptr };
	DWORD m_jvmMainThreadId{ 0 };

	BOOL m_bJvmOwner{ FALSE };

public:
	CJavaVM()
	{
	}

	int CreateJVM(LPCSTR libPath, LPCSTR * jars, int jarsCount, bool bDebug);
	void AttachJVM(JavaVM * jvm, DWORD jvmMainThreadId, BOOL bJvmOwner);

	JNIEnv * AttachThread();
	void DetachThread();

	~CJavaVM();

public:
	//dir end with
	static void AppendJarsInDirectory(LPCWSTR dir, CStringA & strA);
private:
	static HRESULT Utf16To8String(CONST CStringW & strUtf16, CStringA & strUtf8);
	static HRESULT Utf8To16String(CONST CStringA & strUtf8, CStringW & strUtf16);

private:
	void Dispose();
};

class C2JString
{
private:
	C2JString(CONST C2JString &);
	CONST C2JString & operator =(CONST C2JString &);

private:
	CJavaVM * m_vm{ nullptr };
	jstring m_jstring{ nullptr }; /***INTERNAL***/ /***CLEANUP***/

	CONST std::wstring m_str16; //for display purpose only
	CONST std::string m_str8;
public:
	C2JString(CJavaVM * vm, LPCWSTR pwsz, int size);
	C2JString(CJavaVM * vm, LPCSTR psz);

	jstring JavaString();

	jstring Detach();

	~C2JString();

private:
	VOID Dispose();
};


class J2CString
{
private:
	J2CString(CONST J2CString &);
	CONST J2CString & operator =(CONST J2CString &);

private:
	CJavaVM * m_vm{ nullptr };
	jstring m_jstring{ nullptr }; /***INCOMMING***/ /***CLEANUP***/
	bool m_bAttached{ false };
	std::wstring m_str16; /***INTERNAL***/
	std::string m_str8; /***INTERNAL***/

public:
	J2CString(CJavaVM * vm, jstring jstr, bool bAttach);

	LPCWSTR GetString16();
	LPCSTR GetString8();

	jstring Detach();

	~J2CString();
private:
	VOID Dispose();
};

class CJavaObject
{
private:
	CJavaObject(CONST CJavaObject &);
	CONST CJavaObject & operator =(CONST CJavaObject &);

private:
	CJavaVM * m_vm{ nullptr };
	jclass m_class{ nullptr };
	jobject m_object{ nullptr };
	bool m_Attached{ false };
	bool m_isGlobal{ false };
	std::string m_className; /***INTERNAL***/ //display or debug purpose, do not use
public:
	CJavaObject(CJavaVM * vm);

	//CJavaObject();

	VOID NewObject(LPCSTR className);
	VOID NewObject(LPCSTR className, LPCSTR ctorSignatures, ...);
	VOID SetObject(jobject myjobject, bool bGlobal, bool bAttach);

	~CJavaObject();

	//////////////////////////////////////////////////////////////////////////
	void MakeGlobal(); //for multi thread

	//////////////////////////////////////////////////////////////////////////
	jobject JavaObject();
	CJavaVM * JavaVM();
	jclass JavaClass();

	//VOID SetJavaVM(CJavaVM * vm);

	jobject Detach();
	bool IsGlobal();

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, LPCSTR objectName, jobject jobj);
	jobject GetField(LPCSTR fieldName, LPCSTR objectName, jobject & jobj);

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, LPCWSTR value);
	LPCWSTR GetField(LPCSTR fieldName, std::wstring & value);

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jboolean val);
	jboolean GetField(LPCSTR fieldName, jboolean & val);


	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jbyte val);
	jbyte GetField(LPCSTR fieldName, jbyte & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jchar val);
	jchar GetField(LPCSTR fieldName, jchar & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jshort val);
	jshort GetField(LPCSTR fieldName, jshort & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jint val);
	jint GetField(LPCSTR fieldName, jint & val);


	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jlong val);
	jlong GetField(LPCSTR fieldName, jlong & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jfloat val);
	jfloat GetField(LPCSTR fieldName, jfloat & val);


	//////////////////////////////////////////////////////////////////////////
	VOID SetField(LPCSTR fieldName, jdouble val);
	jdouble GetField(LPCSTR fieldName, jdouble & val);

	//////////////////////////////////////////////////////////////////////////
	jmethodID	FindMethod(LPCSTR methodName, LPCSTR methodSigature);

	void		CallMethod(LPCVOID /*nullret*/, jmethodID methodID, ...);
	void		CallMethod(jobject & ret, jmethodID methodID, ...);
	void		CallMethod(jboolean & ret, jmethodID methodID, ...);
	void		CallMethod(jbyte & ret, jmethodID methodID, ...);
	void		CallMethod(jchar & ret, jmethodID methodID, ...);
	void		CallMethod(jshort & ret, jmethodID methodID, ...);
	void		CallMethod(jint & ret, jmethodID methodID, ...);
	void		CallMethod(jlong & ret, jmethodID methodID, ...);
	void		CallMethod(jfloat & ret, jmethodID methodID, ...);
	void		CallMethod(jdouble & ret, jmethodID methodID, ...);



	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, LPCSTR objectName, jobject jobj);
	jobject GetStaticField(LPCSTR fieldName, LPCSTR objectName, jobject & jobj);

	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, LPCWSTR value);
	LPCWSTR GetStaticField(LPCSTR fieldName, std::wstring & value);

	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jboolean val);
	jboolean GetStaticField(LPCSTR fieldName, jboolean & val);


	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jbyte val);
	jbyte GetStaticField(LPCSTR fieldName, jbyte & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jchar val);
	jchar GetStaticField(LPCSTR fieldName, jchar & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jshort val);
	jshort GetStaticField(LPCSTR fieldName, jshort & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jint val);
	jint GetStaticField(LPCSTR fieldName, jint & val);


	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jlong val);
	jlong GetStaticField(LPCSTR fieldName, jlong & val);

	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jfloat val);
	jfloat GetStaticField(LPCSTR fieldName, jfloat & val);


	//////////////////////////////////////////////////////////////////////////
	VOID SetStaticField(LPCSTR fieldName, jdouble val);
	jdouble GetStaticField(LPCSTR fieldName, jdouble & val);

	//////////////////////////////////////////////////////////////////////////
	jmethodID	FindStaticMethod(LPCSTR methodName, LPCSTR methodSigature);

	void		CallStaticMethod(LPCVOID /*nullret*/, jmethodID methodID, ...);
	void		CallStaticMethod(jobject & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jboolean & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jbyte & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jchar & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jshort & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jint & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jlong & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jfloat & ret, jmethodID methodID, ...);
	void		CallStaticMethod(jdouble & ret, jmethodID methodID, ...);

private:
	VOID Dispose();

private:
	static void GetClassName(CJavaVM * vm, jclass cls, std::string & className);

};

#define CALL_JAVA_METHOD(javaObject, methodName, methodSigature, ret, ...) \
{ \
	jmethodID mid = javaObject.FindMethod(methodName, methodSigature); \
	javaObject.CallMethod(ret, mid, __VA_ARGS__); \
}


#define CALL_JAVA_STATIC_METHOD(javaObject, methodName, methodSigature, ret, ...) \
{ \
	jmethodID mid = javaObject.FindStaticMethod(methodName, methodSigature); \
	javaObject.CallStaticMethod(ret, mid, __VA_ARGS__); \
}

template<typename T>
class CJavaArrayBase
{
private:
	CJavaArrayBase(CONST CJavaArrayBase &);
	const CJavaArrayBase & operator =(CONST CJavaArrayBase &);

protected:
	CJavaVM * m_vm{ nullptr };
	int m_length{ 0 };
	jarray m_jarray{ nullptr };
	bool m_bAttached{ false };

public:
	CJavaArrayBase(CJavaVM * vm) :
		m_vm(vm)
	{
	}

	void SetArray(jarray myjarray, bool bAttach)
	{
		m_jarray = myjarray;
		m_bAttached = bAttach;
		m_length = m_vm->AttachThread()->GetArrayLength(m_jarray);
	}

	virtual void New(int length) = 0;
	virtual T Get(int ii) = 0;
	virtual void Set(int ii, T element) = 0;

	virtual void Set(const std::vector<T> & vec) = 0;
	virtual void Get(std::vector<T> & vec) = 0;

	int GetLength()
	{
		return m_length;
	}

	jarray JavaArray()
	{
		return m_jarray;
	}

	jarray Detach()
	{
		jarray ret = m_jarray;
		m_jarray = nullptr;
		return ret;
	}

	virtual ~CJavaArrayBase()
	{
		Dispose();
	}

protected:
	virtual void Dispose()
	{
		if (m_bAttached && m_jarray)
		{
			m_vm->AttachThread()->DeleteLocalRef(m_jarray);
			m_jarray = nullptr;
		}
	}
};

class CJavaArray_jobject :
	public CJavaArrayBase<jobject>
{
private:
	jclass m_class{ nullptr };
	CONST std::string m_str8; //display purpose
public:
	CJavaArray_jobject(CJavaVM * vm, LPCSTR className) :
		CJavaArrayBase<jobject>(vm),
		m_str8(className)
	{
		m_class = m_vm->AttachThread()->FindClass(className);
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewObjectArray(length, m_class, nullptr);
		m_bAttached = true;
		m_length = length;
	}

	virtual jobject Get(int ii) override
	{
		return m_vm->AttachThread()->GetObjectArrayElement((jobjectArray)m_jarray, ii);
	}

	virtual void Set(int ii, jobject element) override
	{
		m_vm->AttachThread()->SetObjectArrayElement((jobjectArray)m_jarray, ii, element);
	}

	virtual void Set(const std::vector<jobject> & vec)
	{
		New(vec.size());
		for (int ii = 0; ii < vec.size(); ++ii)
		{
			Set(ii, vec[ii]);
		}
	}
	virtual void Get(std::vector<jobject> & vec)
	{
		vec.resize(m_length);
		for (int ii = 0; ii < vec.size(); ++ ii)
		{
			vec[ii] = Get(ii);
		}
	}

	static VOID ReleaseVectorJobject(CJavaVM * vm, std::vector<jobject> & vecJobj)
	{
		for (int ii = 0; ii < vecJobj.size(); ++ ii)
		{
			CJavaObject cjo(vm);
			cjo.SetObject(vecJobj[ii], false, true); //destructor delete jobject ref
		}
		vecJobj.clear();
	}

	//static VOID VectorCJavaObject2JObject(CONST std::vector<CJavaObject> & cjos, std::vector<jobject> & jos)
	//{
	//	jos.resize(cjos.size());
	//	for (int ii = 0; ii < jos.size(); ++ii)
	//	{
	//		jos[ii] = const_cast<CJavaObject&>(cjos[ii]).JavaObject();
	//	}
	//}

	//static void VectorJObject2CJavaObject(CJavaVM * vm, CONST std::vector<jobject> & jos, std::vector<CJavaObject> & cjos)
	//{
	//	cjos.resize(jos.size());
	//	for (int ii = 0; ii < cjos.size(); ii++)
	//	{
	//		cjos[ii].SetJavaVM(vm);
	//		cjos[ii].SetObject(nullptr, jos[ii], false, true);
	//	}
	//}
};


class CJavaArray_jboolean :
	public CJavaArrayBase<jboolean>
{
public:
	CJavaArray_jboolean(CJavaVM * vm) :
		CJavaArrayBase<jboolean>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewBooleanArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jboolean Get(int ii) override
	{
		jboolean ret = false;
		m_vm->AttachThread()->GetBooleanArrayRegion((jbooleanArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jboolean element) override
	{
		m_vm->AttachThread()->SetBooleanArrayRegion((jbooleanArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jboolean> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetBooleanArrayRegion((jbooleanArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jboolean> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetBooleanArrayRegion((jbooleanArray)m_jarray, 0, vec.size(), &vec[0]);
	}


#if 0
	virtual void Set(const std::vector<xxx> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetxxxArrayRegion((xxxArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<xxx> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetxxxArrayRegion((xxxArray)m_jarray, 0, vec.size(), &vec[0]);
	}
#endif
};

class CJavaArray_jbyte :
	public CJavaArrayBase<jbyte>
{
public:
	CJavaArray_jbyte(CJavaVM * vm) :
		CJavaArrayBase<jbyte>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewByteArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jbyte Get(int ii) override
	{
		jbyte ret = false;
		m_vm->AttachThread()->GetByteArrayRegion((jbyteArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jbyte element) override
	{
		m_vm->AttachThread()->SetByteArrayRegion((jbyteArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jbyte> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetByteArrayRegion((jbyteArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jbyte> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetByteArrayRegion((jbyteArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};


class CJavaArray_jchar :
	public CJavaArrayBase<jchar>
{
public:
	CJavaArray_jchar(CJavaVM * vm) :
		CJavaArrayBase<jchar>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewCharArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jchar Get(int ii) override
	{
		jchar ret = false;
		m_vm->AttachThread()->GetCharArrayRegion((jcharArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jchar element) override
	{
		m_vm->AttachThread()->SetCharArrayRegion((jcharArray)m_jarray, ii, 1, &element);
	}


	virtual void Set(const std::vector<jchar> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetCharArrayRegion((jcharArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jchar> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetCharArrayRegion((jcharArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};


class CJavaArray_jshort :
	public CJavaArrayBase<jshort>
{
public:
	CJavaArray_jshort(CJavaVM * vm) :
		CJavaArrayBase<jshort>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewShortArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jshort Get(int ii) override
	{
		jshort ret = false;
		m_vm->AttachThread()->GetShortArrayRegion((jshortArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jshort element) override
	{
		m_vm->AttachThread()->SetShortArrayRegion((jshortArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jshort> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetShortArrayRegion((jshortArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jshort> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetShortArrayRegion((jshortArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};

class CJavaArray_jint :
	public CJavaArrayBase<jint>
{
public:
	CJavaArray_jint(CJavaVM * vm) :
		CJavaArrayBase<jint>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewIntArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jint Get(int ii) override
	{
		jint ret = false;
		m_vm->AttachThread()->GetIntArrayRegion((jintArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jint element) override
	{
		m_vm->AttachThread()->SetIntArrayRegion((jintArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jint> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetIntArrayRegion((jintArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jint> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetIntArrayRegion((jintArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};


class CJavaArray_jlong :
	public CJavaArrayBase<jlong>
{
public:
	CJavaArray_jlong(CJavaVM * vm) :
		CJavaArrayBase<jlong>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewLongArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jlong Get(int ii) override
	{
		jlong ret = false;
		m_vm->AttachThread()->GetLongArrayRegion((jlongArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jlong element) override
	{
		m_vm->AttachThread()->SetLongArrayRegion((jlongArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jlong> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetLongArrayRegion((jlongArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jlong> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetLongArrayRegion((jlongArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};


class CJavaArray_jfloat :
	public CJavaArrayBase<jfloat>
{
public:
	CJavaArray_jfloat(CJavaVM * vm) :
		CJavaArrayBase<jfloat>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewFloatArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jfloat Get(int ii) override
	{
		jfloat ret = false;
		m_vm->AttachThread()->GetFloatArrayRegion((jfloatArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jfloat element) override
	{
		m_vm->AttachThread()->SetFloatArrayRegion((jfloatArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jfloat> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetFloatArrayRegion((jfloatArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jfloat> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetFloatArrayRegion((jfloatArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};


class CJavaArray_jdouble :
	public CJavaArrayBase<jdouble>
{
public:
	CJavaArray_jdouble(CJavaVM * vm) :
		CJavaArrayBase<jdouble>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewDoubleArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual jdouble Get(int ii) override
	{
		jdouble ret = false;
		m_vm->AttachThread()->GetDoubleArrayRegion((jdoubleArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, jdouble element) override
	{
		m_vm->AttachThread()->SetDoubleArrayRegion((jdoubleArray)m_jarray, ii, 1, &element);
	}

	virtual void Set(const std::vector<jdouble> & vec) override
	{
		New(vec.size());
		m_vm->AttachThread()->SetDoubleArrayRegion((jdoubleArray)m_jarray, 0, vec.size(), &vec[0]);
	}

	virtual void Get(std::vector<jdouble> & vec) override
	{
		vec.resize(m_length);
		m_vm->AttachThread()->GetDoubleArrayRegion((jdoubleArray)m_jarray, 0, vec.size(), &vec[0]);
	}
};

#if 0
class CJavaArray_xxx :
	public CJavaArrayBase<xxx>
{
public:
	CJavaArray_xxx(CJavaVM * vm) :
		CJavaArrayBase<xxx>(vm)
	{
	}

	virtual void New(int length) override
	{
		m_jarray = m_vm->AttachThread()->NewxxxArray(length);
		m_bAttached = true;
		m_length = length;
	}

	virtual xxx Get(int ii) override
	{
		xxx ret = false;
		m_vm->AttachThread()->GetXXXArrayRegion((jxxxArray)m_jarray, ii, 1, &ret);
		return ret;
	}

	virtual void Set(int ii, xxx element) override
	{
		m_vm->AttachThread()->SetXXXArrayRegion((jxxxArray)m_jarray, ii, 1, &element);
	}
};
#endif

}//namespace SmartLib