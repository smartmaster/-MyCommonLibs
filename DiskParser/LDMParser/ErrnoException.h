#pragma once

class ErrnoException : public std::runtime_error
{
public:
    ErrnoException(const char* message, int error = errno);

    int GetError() const;
    std::string GetDescription() const;

private:
    int m_error;
};