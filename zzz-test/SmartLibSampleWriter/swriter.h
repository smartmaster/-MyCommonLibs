/*
**++
**
** Copyright (c) Microsoft Corporation
**
**
** Module Name:
**
**	    swriter.h
**
**
** Abstract:
**
**	Demonstrate a  writer
**
*/

#ifndef _SWRITER_H_
#define _SWRITER_H_

///////////////////////////////////////////////////////////////////////////////
// Declarations and Definitions

// This GUID identifies the writer
static const VSS_ID SmlSampleWriterId = 
	//{ 0x079462f1, 0x1079, 0x48dd, { 0xb3, 0xfb, 0xcc, 0xb2, 0xf2, 0x93, 0x4e, 0xcf } };
	{ 0x683a335c, 0xa463, 0x4722, { 0x92, 0x65, 0xd2, 0xb, 0xbc, 0x4f, 0x9b, 0xc8 } }; //<sonmi01>2018-9-21 ###???


static const wchar_t* const  SmlSampleWriterName = 
	//L"MSDNSampleWriter";
	L"SmartLibSampleWriter"; //<sonmi01>2018-9-21 ###???

///////////////////////////////////////////////////////////////////////////////
// DepWriter class

class SmlSampleWriter : public CVssWriter    {
private:
public:
    // initialize all static variables
    static void StaticInitialize()  {  }
    
    SmlSampleWriter() { }
    virtual ~SmlSampleWriter() { Uninitialize(); }

    HRESULT STDMETHODCALLTYPE Initialize();
    HRESULT STDMETHODCALLTYPE Uninitialize()    { return Unsubscribe(); }
    bool STDMETHODCALLTYPE OnIdentify(IN IVssCreateWriterMetadata *pMetadata);
    bool STDMETHODCALLTYPE OnPrepareBackup(_In_ IVssWriterComponents *pComponents);
    bool STDMETHODCALLTYPE OnPrepareSnapshot();
    bool STDMETHODCALLTYPE OnFreeze();
    bool STDMETHODCALLTYPE OnThaw();
    bool STDMETHODCALLTYPE OnPostSnapshot(_In_ IVssWriterComponents *pComponents);
    bool STDMETHODCALLTYPE OnAbort();
    bool STDMETHODCALLTYPE OnBackupComplete(_In_ IVssWriterComponents *pComponents);
    bool STDMETHODCALLTYPE OnBackupShutdown(_In_ VSS_ID SnapshotSetId);
    bool STDMETHODCALLTYPE OnPreRestore(_In_ IVssWriterComponents *pComponents);
    bool STDMETHODCALLTYPE OnPostRestore(_In_ IVssWriterComponents *pComponents);


private:
	HRESULT Dump(_In_ IVssWriterComponents *components);
};


#endif
	
