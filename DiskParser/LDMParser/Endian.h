#pragma once


inline uint16_t ByteReverse(uint16_t a)
{
    uint16_t h = a >> 8;
    uint16_t l = a << 8;
    return h | l;
}

inline uint32_t ByteReverse(uint32_t a)
{
    uint8_t dst[4];
    const uint8_t* src = reinterpret_cast<const uint8_t*>(&a);
    dst[0] = src[3];
    dst[1] = src[2];
    dst[2] = src[1];
    dst[3] = src[0];
    return *reinterpret_cast<uint32_t*>(dst);
}

inline uint64_t ByteReverse(uint64_t a)
{
    uint8_t dst[8];
    const uint8_t* src = reinterpret_cast<const uint8_t*>(&a);
    dst[0] = src[7];
    dst[1] = src[6];
    dst[2] = src[5];
    dst[3] = src[4];
    dst[4] = src[3];
    dst[5] = src[2];
    dst[6] = src[1];
    dst[7] = src[0];
    return *reinterpret_cast<uint64_t*>(dst);
}
