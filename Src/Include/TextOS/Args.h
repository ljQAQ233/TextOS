#ifndef __ARGS_H__
#define __ARGS_H__

typedef __builtin_va_list VA_LIST;

#define VA_START(Marker, Parameter) \
        __builtin_va_start (Marker, Parameter)

#define VA_ARG(Marker, TYPE) \
        ((sizeof (TYPE) < sizeof (UINTN)) ? (TYPE)(__builtin_va_arg (Marker, UINTN)) : (TYPE)(__builtin_va_arg (Marker, TYPE)))

#define VA_END(Marker) \
        __builtin_va_end (Marker)

#define VA_COPY(Dest, Start) \
        __builtin_va_copy (Dest, Start)

#endif
