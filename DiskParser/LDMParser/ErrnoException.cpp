#include "stdafx.h"
#include "stdafx.include.h"
#include "ErrnoException.h"

ErrnoException::ErrnoException(const char* message, int error /*= errno*/)
    : std::runtime_error(message)
    , m_error(error)
{
}

int ErrnoException::GetError() const
{
    return m_error;
}

std::string ErrnoException::GetDescription() const
{
	char buffer[2048] = { 0 };
    strerror_s(buffer, m_error);
	return std::string(buffer);
}
