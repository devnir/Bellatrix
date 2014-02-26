#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_



#ifdef __cplusplus
#define C_CPP "C"
#else
#define C_CPP
#endif

#ifndef  FALSE
#define  FALSE                     0u
#endif

#ifndef  TRUE
#define  TRUE                         1u
#endif
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */
typedef signed   long long INT64S;
typedef unsigned long long INT64U;
typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

typedef INT64U TWORD64u;
typedef INT64S TWORD64s;
typedef INT32U TWORD32u; 
typedef INT32S TWORD32s;

#endif //
