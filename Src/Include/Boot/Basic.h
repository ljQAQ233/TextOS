#ifndef __BASIC_H__
#define __BASIC_H__

#ifdef __SRC_LEVEL_DEBUG__
  #define Break() CpuBreakpoint()
#else
  #define Break() 
#endif

#define DEFAULT_MAX_HOR  700

EFI_STATUS ExitBootServices ();

#endif
