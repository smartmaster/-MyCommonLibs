#include "stdafx.h"
#include "stdafx.include.h"
#include "FileGuard.h"

//FileGuard::FileGuard(FILE* file)
//    : m_file(file)
//{
//}

FileGuard::FileGuard(IFileDevice* fd) :
	m_file(fd)
{
}

FileGuard::~FileGuard()
{
    //fclose(m_file);
	m_file.Release();
}



//FileGuard::operator FILE*()
//{
//    return m_file;
//}

FileGuard::operator IFileDevice*()
{
    return m_file;
}

