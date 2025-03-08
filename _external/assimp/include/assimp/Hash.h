#pragma once
#ifndef AI_HASH_H_INCLUDED
#define AI_HASH_H_INCLUDED

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

// ------------------------------------------------------------------------------------------------
// Hashing function taken from
// http://www.azillionmonkeys.com/qed/hash.html
// (incremental version)
//
// This code is Copyright 2004-2008 by Paul Hsieh. It is used here in the belief that
// Assimp's license is considered compatible with Pauls's derivative license as specified
// on his web page.
//
// (stdint.h should have been been included here)
// ------------------------------------------------------------------------------------------------
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#  define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#  define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

// ------------------------------------------------------------------------------------------------
inline uint32_t SuperFastHash (const char * data, uint32_t len = 0, uint32_t hash = 0) {
    uint32_t tmp;
    int rem;

    if (data == NULL) return 0;
    if (len == 0)len = (uint32_t)::strlen(data);

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= abs(data[sizeof(uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += *data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

#endif // !! AI_HASH_H_INCLUDED
