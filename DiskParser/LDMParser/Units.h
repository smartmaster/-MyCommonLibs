#pragma once

class Units
{
public:
    Units(uint64_t units);

    friend std::ostream& operator<<(std::ostream& stream, const Units& units);

private:
    uint64_t m_units;
};