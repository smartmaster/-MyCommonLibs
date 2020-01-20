/*
 * NTFS Class common definitions
 */
#pragma once

#ifndef	__NTFS_COMMON_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE
#define	__NTFS_COMMON_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>

//<sonmi01>2014-4-1 ###???
#include <vector>
using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"

#include "NTFS_Lib_DataType.h"

#define	ATTR_NUMS		16				// Attribute Types count
#define	ATTR_INDEX(at)	(((at)>>4)-1)	// Attribute Type to Index, e.g. 0x10->0, 0x30->2
#define	ATTR_MASK(at)	(((DWORD)1)<<ATTR_INDEX(at))	// Attribute Bit Mask

// Bit masks of Attributes
#define	MASK_STANDARD_INFORMATION	ATTR_MASK(ATTR_TYPE_STANDARD_INFORMATION)
#define	MASK_ATTRIBUTE_LIST			ATTR_MASK(ATTR_TYPE_ATTRIBUTE_LIST)
#define	MASK_FILE_NAME				ATTR_MASK(ATTR_TYPE_FILE_NAME)
#define	MASK_OBJECT_ID				ATTR_MASK(ATTR_TYPE_OBJECT_ID)
#define	MASK_SECURITY_DESCRIPTOR	ATTR_MASK(ATTR_TYPE_SECURITY_DESCRIPTOR)
#define	MASK_VOLUME_NAME			ATTR_MASK(ATTR_TYPE_VOLUME_NAME)
#define	MASK_VOLUME_INFORMATION		ATTR_MASK(ATTR_TYPE_VOLUME_INFORMATION)
#define	MASK_DATA					ATTR_MASK(ATTR_TYPE_DATA)
#define	MASK_INDEX_ROOT				ATTR_MASK(ATTR_TYPE_INDEX_ROOT)
#define	MASK_INDEX_ALLOCATION		ATTR_MASK(ATTR_TYPE_INDEX_ALLOCATION)
#define	MASK_BITMAP					ATTR_MASK(ATTR_TYPE_BITMAP)
#define	MASK_REPARSE_POINT			ATTR_MASK(ATTR_TYPE_REPARSE_POINT)
#define	MASK_EA_INFORMATION			ATTR_MASK(ATTR_TYPE_EA_INFORMATION)
#define	MASK_EA						ATTR_MASK(ATTR_TYPE_EA)
#define	MASK_LOGGED_UTILITY_STREAM	ATTR_MASK(ATTR_TYPE_LOGGED_UTILITY_STREAM)

#define	MASK_ALL					((DWORD)-1)

#if 0

#define	DEBUG_TRACE0(t1)					_RPT0(_CRT_WARN, t1)
#define	DEBUG_TRACE1(t1, t2)				_RPT1(_CRT_WARN, t1, t2)
#define	DEBUG_TRACE2(t1, t2, t3)			_RPT2(_CRT_WARN, t1, t2, t3)
#define	DEBUG_TRACE3(t1, t2, t3, t4)		_RPT3(_CRT_WARN, t1, t2, t3, t4)
#define	DEBUG_TRACE4(t1, t2, t3, t4, t5)	_RPT4(_CRT_WARN, t1, t2, t3, t4, t5)

#else

#define	DEBUG_TRACE0(t1)												D_INFO(0, L#t1)
#define	DEBUG_TRACE1(t1, t2)											D_INFO(0, L#t1, t2)
#define	DEBUG_TRACE2(t1, t2, t3)										D_INFO(0, L#t1, t2, t3)
#define	DEBUG_TRACE3(t1, t2, t3, t4)									D_INFO(0, L#t1, t2, t3, t4)
#define	DEBUG_TRACE4(t1, t2, t3, t4, t5)								D_INFO(0, L#t1, t2, t3, t4, t5)
#define	DEBUG_TRACE5(t1, t2, t3, t4, t5, t6)							D_INFO(0, L#t1, t2, t3, t4, t5, t6)
#define	DEBUG_TRACE6(t1, t2, t3, t4, t5, t6, t7)						D_INFO(0, L#t1, t2, t3, t4, t5, t6, t7)
#define	DEBUG_TRACE7(t1, t2, t3, t4, t5, t6, t7, t8)					D_INFO(0, L#t1, t2, t3, t4, t5, t6, t7, t8)
#define	DEBUG_TRACE8(t1, t2, t3, t4, t5, t6, t7, t8, t9)				D_INFO(0, L#t1, t2, t3, t4, t5, t6, t7, t8, t9)
#define	DEBUG_TRACE9(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10)			D_INFO(0, L#t1, t2, t3, t4, t5, t6, t7, t8, t9, t10)

#endif

// User defined Callback routines to process raw attribute data
// Set bDiscard to TRUE if this Attribute is to be discarded
// Set bDiscard to FALSE to let CFileRecord process it
typedef void(*ATTR_RAW_CALLBACK)(CONST ATTR_HEADER_COMMON *attrHead, BOOL *bDiscard);

// User defined Callback routine to handle CFileRecord parsed attributes
// Will be called by CFileRecord::TraverseAttrs() for each attribute
// attrClass is the according attributes wrapping class, CAttr_xxx
// Set bStop to TRUE if don't want to continue
// Set bStop to FALSE to continue processing
class CAttrBase;
typedef void(*ATTRS_CALLBACK)(CONST CAttrBase *attr, void *context, BOOL *bStop);

// User defined Callback routine to handle Directory traversing
// Will be called by CFileRecord::TraverseSubEntries for each sub entry
class CIndexEntry;
typedef void(*SUBENTRY_CALLBACK)(CONST CIndexEntry *ie);


// List Entry
template <class ENTRY_TYPE>
struct NTSLIST_ENTRY
{
	NTSLIST_ENTRY	* m_Next;
	ENTRY_TYPE		* m_Entry;
};

// List Entry Smart Pointer
template <class ENTRY_TYPE>
class CEntrySmartPtr
{
public:
	CEntrySmartPtr(ENTRY_TYPE *ptr = NULL)
	{
		m_EntryPtr = ptr;
	}

	virtual ~CEntrySmartPtr()
	{
		if (m_EntryPtr)
			delete m_EntryPtr;
	}

private:
	CONST ENTRY_TYPE *m_EntryPtr;

public:
	CEntrySmartPtr<ENTRY_TYPE> operator = (CONST ENTRY_TYPE* ptr)
	{
		// Delete previous pointer if allocated
		if (m_EntryPtr)
			delete m_EntryPtr;

		m_EntryPtr = ptr;

		return *this;
	}

	CONST ENTRY_TYPE* operator->() CONST
	{
		_ASSERT(m_EntryPtr);
		return m_EntryPtr;
	}

		BOOL IsValid() CONST
	{
		return m_EntryPtr != NULL;
	}
};

//******************************************
// Single list implementation
//******************************************
#if 0
template <class ENTRY_TYPE>
class CNtfsSList
{
public:
	CNtfsSList()
	{
		m_ListHead = m_ListTail = NULL;
		m_ListCurrent = NULL;
		m_EntryCount = 0;
	}

	virtual ~CNtfsSList()
	{
		RemoveAll();
	}

private:
	INT m_EntryCount;
	NTSLIST_ENTRY<ENTRY_TYPE> *m_ListHead;
	NTSLIST_ENTRY<ENTRY_TYPE> *m_ListTail;
	NTSLIST_ENTRY<ENTRY_TYPE> *m_ListCurrent;

public:
	// Get entry count
	INT GetCount() CONST
	{
		return m_EntryCount;
	}

		// Insert to tail
		BOOL InsertEntry(ENTRY_TYPE *entry)
	{
			NTSLIST_ENTRY<ENTRY_TYPE> *le = new NTSLIST_ENTRY<ENTRY_TYPE>;
			if (!le)
				return FALSE;

			le->m_Entry = entry;
			le->m_Next = NULL;

			if (m_ListTail == NULL)
				m_ListHead = le;		// Empty list
			else
				m_ListTail->m_Next = le;

			m_ListTail = le;

			m_EntryCount++;
			return TRUE;
	}

	// Remove all entries
	void RemoveAll()
	{
		while (m_ListHead)
		{
			m_ListCurrent = m_ListHead->m_Next;
			delete m_ListHead->m_Entry;
			delete m_ListHead;

			m_ListHead = m_ListCurrent;
		}

		m_ListHead = m_ListTail = NULL;
		m_ListCurrent = NULL;
		m_EntryCount = 0;
	}

	// Find first entry
	ENTRY_TYPE *FindFirstEntry() CONST
	{
		((CNtfsSList<ENTRY_TYPE>*)this)->m_ListCurrent = m_ListHead;

		if (m_ListCurrent)
			return m_ListCurrent->m_Entry;
		else
			return NULL;
	}

		// Find next entry
		ENTRY_TYPE *FindNextEntry() CONST
	{
		if (m_ListCurrent)
		((CNtfsSList<ENTRY_TYPE>*)this)->m_ListCurrent = m_ListCurrent->m_Next;

		if (m_ListCurrent)
			return m_ListCurrent->m_Entry;
		else
			return NULL;
	}

		// Throw all entries
		// Caution! All entries are just thrown without free
		void ThrowAll()
	{
			m_ListHead = m_ListTail = NULL;
			m_ListCurrent = NULL;
			m_EntryCount = 0;
	}
};	//CSList
#else
template <typename ENTRY_TYPE>
class CNtfsSList
{
public:
	CNtfsSList()
	{
		m_current = 0;
	}

	virtual ~CNtfsSList()
	{
		RemoveAll();
	}

private:
	vector<ENTRY_TYPE*> m_vec;
	mutable size_t m_current;

public:
	// Get entry count
	INT GetCount() CONST
	{
		return m_vec.size();
	}

		// Insert to tail
		BOOL InsertEntry(ENTRY_TYPE *entry)
	{
			m_vec.push_back(entry);
			return TRUE;
	}

	// Remove all entries
	void RemoveAll()
	{
		for (size_t ii = 0; ii < m_vec.size(); ++ii)
		{
			delete m_vec[ii];
			m_vec[ii] = NULL;
		}
		m_vec.clear();
	}

	// Find first entry
	ENTRY_TYPE *FindFirstEntry() CONST
	{
		m_current = 0;

		if (m_current >= m_vec.size())
		{
			return NULL;
		}
		else
		{
			return m_vec[m_current];
		}
	}

		// Find next entry
		ENTRY_TYPE *FindNextEntry() CONST
	{
		if (m_current >= m_vec.size())
		{
			return NULL;
		}

		++m_current;

		if (m_current >= m_vec.size())
		{
			return NULL;
		}
		else
		{
			return m_vec[m_current];
		}
	}

		// Throw all entries
		// Caution! All entries are just thrown without free
		void ThrowAll()
	{
		m_current = 0; //<sonmi01>2014-4-18 ###???
		m_vec.clear();

	}
};	//CSList
#endif


//******************************************
// Stack implementation
//******************************************
#if 0
template <class ENTRY_TYPE>
class CNtfsStack
{
public:
	CNtfsStack()
	{
		m_ListHead = m_ListTail = NULL;
		m_EntryCount = 0;
	}

	virtual ~CNtfsStack()
	{
		RemoveAll();
	}

private:
	INT m_EntryCount;
	NTSLIST_ENTRY<ENTRY_TYPE> *m_ListHead;
	NTSLIST_ENTRY<ENTRY_TYPE> *m_ListTail;

public:
	// Get entry count
	INT GetCount() CONST
	{
		return m_EntryCount;
	}

		// Insert to head
		BOOL Push(ENTRY_TYPE *entry)
	{
			NTSLIST_ENTRY<ENTRY_TYPE> *le = new NTSLIST_ENTRY<ENTRY_TYPE>;
			if (!le)
				return FALSE;

			le->m_Entry = entry;
			le->m_Next = m_ListHead;

			m_ListHead = le;

			if (m_ListTail == NULL)
				m_ListTail = le;		// Empty list

			m_EntryCount++;
			return TRUE;
	}

	// Remove from head
	ENTRY_TYPE* Pop()
	{
		if (m_ListHead == NULL)
			return NULL;

		NTSLIST_ENTRY<ENTRY_TYPE> *le = m_ListHead;
		ENTRY_TYPE *e = le->m_Entry;

		if (m_ListTail == m_ListHead)
			m_ListTail = m_ListHead->m_Next;
		m_ListHead = m_ListHead->m_Next;

		delete le;
		m_EntryCount--;

		return e;
	}

	// Remove all entries
	void RemoveAll()
	{
		NTSLIST_ENTRY<ENTRY_TYPE> *le;

		while (m_ListHead)
		{
			le = m_ListHead->m_Next;
			delete m_ListHead->m_Entry;
			delete m_ListHead;

			m_ListHead = le;
		}

		m_ListHead = m_ListTail = NULL;
		m_EntryCount = 0;
	}
};	//CStack
#else
template <class ENTRY_TYPE>
class CNtfsStack
{
public:
	CNtfsStack()
	{
	}

	virtual ~CNtfsStack()
	{
		RemoveAll();
	}

private:
	vector<ENTRY_TYPE*> m_vec;

public:
	// Get entry count
	INT GetCount() CONST
	{
		return m_vec.size();
	}

		// Insert to head
	BOOL Push(ENTRY_TYPE *entry)
	{
			m_vec.push_back(entry);
			return TRUE;
	}

	// Remove from head
	ENTRY_TYPE* Pop()
	{
		ENTRY_TYPE* e = NULL;
		if (m_vec.size())
		{
			e = m_vec.back();
			m_vec.pop_back();
		}

		return e;
	}

	// Remove all entries
	void RemoveAll()
	{
		for (size_t ii = 0; ii < m_vec.size(); ++ii)
		{
			delete m_vec[ii];
			m_vec[ii] = NULL;
		}
		m_vec.clear();
	}
};	//CStack
#endif

#endif
