
#ifndef STD_TYPES_H
#define STD_TYPES_H

#define _CPU_TYPE_8 8
#define _CPU_TYPE_16 16
#define _CPU_TYPE_32 32
#define _CPU_TYPE_64 64
/************/
#define _CPU_TYPE (_CPU_TYPE_8)
/************/
#if (_CPU_TYPE == (_CPU_TYPE_8))
typedef unsigned char u8;
typedef signed char   s8;

typedef unsigned short  u16;
typedef signed short  s16;

typedef unsigned long  u32;
typedef signed long  s32;

typedef float f32;
typedef double f64;
#define OK           0
#define NOK          1
#define NULL_POINTER 2
#define IDLE         3
#define BUSY         4
#define BUSY_FUNC    5
#elif (_CPU_TYPE == (_CPU_TYPE_16))
typedef unsigned char u8;
typedef signed char   s8;

typedef unsigned short  u16;
typedef signed short  s16;

typedef unsigned long  u32;
typedef signed long  s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

#elif (_CPU_TYPE == (_CPU_TYPE_32))
typedef unsigned char u8;
typedef signed char   s8;

typedef unsigned short  u16;
typedef signed short  s16;

typedef unsigned long  u32;
typedef signed long  s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;


#elif (_CPU_TYPE == (_CPU_TYPE_64))
typedef unsigned char u8;
typedef signed char   s8;

typedef unsigned short int u16;
typedef signed short int s16;

typedef unsigned long int u32;
typedef signed long int s32;

typedef float f32;
typedef double f64;
#endif
/************/
#endif  //_STD_TYPES_H

