#ifndef __BASE_H__
#define __BASE_H__

/* Import types */
#include <TextOS/Type.h>

//#define packed(packed)   __attribute__((packed))

/* Tools like MAX and MIN */
#define MAX(A,B) \
    (A > B) ? A : B
#define MIN(A,B) \
    (A < B) ? A : B

/* The reserved value */
#define RESERVED 0x00

/* The Bool Type Value */
#define TRUE     (1 == 1)
#define FALSE    (1 == 0)

/* Enabled flags */
#define ENABLE   TRUE
#define DISABLE  FALSE

#define NULL     (VOID*)0

/* Some C keywords */
#define VOID     void
#define CONST    const
#define STATIC   static
#define INLINE   inline

/* The Symbol of the in or out args */
#define IN
#define OUT

/* Return Status Type */
typedef UINT64    RT_STATUS;

/* The value for type STATUS */
#define RT_FAILED       -1
#define RT_INVALID_ARGS -2
#define RT_OUT_OF_RANGE -3
#define RT_SUCCESS       1

#define RTC_ERROR(Status) \
    (Status <= 0 || Status == NULL)
#define RTC_OK(Status) \
    (Status > 0)

#endif
