#pragma once

#include <stdint.h>
#include <stdio.h>

typedef uint8_t byte;
typedef uint16_t word;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef void (*destructor)(void *element);

#define DEBUG

#define SUCCESS 0
#define MEMORY_ALLOCATION_ERROR -1
#define MEMORY_DEALLOCATION_ERROR -6
#define FILE_OPEN_ERROR -2
#define NULL_DESTRUCTOR_ERROR -3
#define NULL_ARRAY_ERROR -4
#define ATLAS_GET_SPRITE_WITH_WRONG_SIZE -5
#define OTHER_ERROR -999

#define print_error(msg, error_code)                          \
	printf msg;                                               \
	printf("\nError Code: %s %d\n", #error_code, error_code); \
	exit(error_code);


#define new(TYPE) malloc(sizeof(TYPE))