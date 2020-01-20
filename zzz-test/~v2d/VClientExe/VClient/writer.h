/////////////////////////////////////////////////////////////////////////
// Copyright ?2006 Microsoft Corporation. All rights reserved.
// 
//  This file may contain preliminary information or inaccuracies, 
//  and may not correctly represent any associated Microsoft 
//  Product as commercially released. All Materials are provided entirely 
//  “AS IS.?To the extent permitted by law, MICROSOFT MAKES NO 
//  WARRANTY OF ANY KIND, DISCLAIMS ALL EXPRESS, IMPLIED AND STATUTORY 
//  WARRANTIES, AND ASSUMES NO LIABILITY TO YOU FOR ANY DAMAGES OF 
//  ANY TYPE IN CONNECTION WITH THESE MATERIALS OR ANY INTELLECTUAL PROPERTY IN THEM. 
// 


#pragma once


// The type of a file descriptor
typedef enum 
{
    VSS_FDT_UNDEFINED = 0,
    VSS_FDT_EXCLUDE_FILES,
    VSS_FDT_FILELIST,
    VSS_FDT_DATABASE,
    VSS_FDT_DATABASE_LOG,
} VSS_DESCRIPTOR_TYPE;


//////////////////////////////////////////////////////////////////////////////////////
// In-memory representation of a file descriptor
//

struct VssFileDescriptor
{
    VssFileDescriptor(): 
        m_isRecursive(false), 
        m_type(VSS_FDT_UNDEFINED)
        {};

    // Initialize from a IVssWMFiledesc
    void Initialize(
        IVssWMFiledesc * pFileDesc, 
        VSS_DESCRIPTOR_TYPE typeParam
        );

    // Print this file descriptor 
    void Print();

    // Get the string representation of the type
    wstring GetStringFromFileDescriptorType(VSS_DESCRIPTOR_TYPE eType);

    //
    //  Data members
    //

    wstring             m_path;
    wstring             m_filespec;
    wstring             m_alternatePath;
    bool                m_isRecursive;

    VSS_DESCRIPTOR_TYPE m_type;
    wstring             m_expandedPath;
    wstring             m_affectedVolume;
};



//////////////////////////////////////////////////////////////////////////////////////
// In-memory representation of a component dependency
//

#ifdef VSS_SERVER

struct VssDependency
{
    VssDependency() {};

    // Initialize from a IVssWMDependency
    void Initialize(
        IVssWMDependency * pDependency
        );

    // Print this dependency
    void Print();

    //
    //  Data members
    //

    wstring             m_writerId;
    wstring             m_logicalPath;
    wstring             m_componentName;
    wstring             m_fullPath;
};

#endif


//////////////////////////////////////////////////////////////////////////////////////
// In-memory representation of a component
//

struct VssComponent
{
    VssComponent(): 
        m_type(VSS_CT_UNDEFINED),
        m_isSelectable(false),
        m_notifyOnBackupComplete(false),
        m_isTopLevel(false),
        m_isExcluded(false),
        m_isExplicitlyIncluded(false)
        {};

    // Initialize from a IVssWMComponent
    void Initialize(wstring writerNameParam, IVssWMComponent * pComponent);

    // Initialize from a IVssComponent
    void Initialize(wstring writerNameParam, IVssComponent * pComponent);

    // Print summary/detalied information about this component
    void Print(bool bListDetailedInfo);

    // Convert a component type into a string
    wstring GetStringFromComponentType(VSS_COMPONENT_TYPE eComponentType);

    // Return TRUE if the current component is ancestor of the given component
    bool IsAncestorOf(VssComponent & child);

    // return TRUEif it can be explicitly included
    bool CanBeExplicitlyIncluded();

    //
    //  Data members
    //

    wstring             m_name;
    wstring             m_writerName;
    wstring             m_logicalPath;
    wstring             m_caption;
    VSS_COMPONENT_TYPE  m_type;
    bool                m_isSelectable;
    bool                m_notifyOnBackupComplete;

    wstring             m_fullPath;
    bool                m_isTopLevel;
    bool                m_isExcluded;
    bool                m_isExplicitlyIncluded;
    vector<wstring>     m_affectedPaths;
    vector<wstring>     m_affectedVolumes;
    vector<VssFileDescriptor> m_descriptors;

#ifdef VSS_SERVER
    vector<VssDependency> m_dependencies;
#endif
};


//////////////////////////////////////////////////////////////////////////////////////
// In-memory representation of a writer metadata
//

struct VssWriter
{
    VssWriter(): 
        m_isExcluded(false),
        m_supportsRestore(false),
        m_restoreMethod(VSS_RME_UNDEFINED),
        m_writerRestoreConditions(VSS_WRE_UNDEFINED),
        m_rebootRequiredAfterRestore(false)
        {};

    // Initialize from a IVssWMFiledesc
    void Initialize(IVssExamineWriterMetadata * pMetadata);

    // Initialize from a IVssWriterComponentsExt
    void InitializeComponentsForRestore(IVssWriterComponentsExt * pWriterComponents);

    // Print summary/detalied information about this writer
    void Print(bool bListDetailedInfo);

    wstring GetStringFromRestoreMethod(VSS_RESTOREMETHOD_ENUM eRestoreMethod);

    wstring GetStringFromRestoreConditions(VSS_WRITERRESTORE_ENUM eRestoreEnum);

    //
    //  Data members
    //

    wstring                     m_name;
    wstring                     m_id;
    wstring                     m_instanceId;
    vector<VssComponent>        m_components;
    vector<VssFileDescriptor>   m_excludedFiles;
    VSS_WRITERRESTORE_ENUM      m_writerRestoreConditions;
    bool                        m_supportsRestore;
    VSS_RESTOREMETHOD_ENUM      m_restoreMethod;
    bool                        m_rebootRequiredAfterRestore;

    bool                        m_isExcluded;
};

