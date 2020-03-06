#include "stdafx.h"
#include "stdafx.include.h"
#include "Units.h"
#include "ArraySize.h"

Units::Units(uint64_t units)
    : m_units(units)
{
}

std::ostream& operator<<(std::ostream& stream, const Units& units)
{
    static const char* unitsName[] = {"B", "KB", "MB", "GB", "TB"};

    uint64_t size = units.m_units;
    size_t i = 0;

    for (; size != 0 && i < ARRAYSIZE(unitsName) - 1; ++i)
    {
        if (size % 1024 == 0)
        {
            size /= 1024;
        }
        else
        {
            break;
        }
    }

    stream << size << " " << unitsName[i];

    return stream;
}
