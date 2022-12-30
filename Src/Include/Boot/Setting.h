#ifndef __SETTING_H__
#define __SETTING_H__

#include <Boot/SimpleIni.h>

EFI_STATUS
SettingInit (
    IN EFI_HANDLE ImageHandle
);

/* Use KeyName to get value,CHAR16 type */
#define SettingGetValueC16(KeyName) \
            IniGetValueC16(Setting,KeyName)

/* Use KeyName to get value,UINT64 type */
#define SettingGetValueU64(KeyName) \
            IniGetValueU64(Setting,KeyName)

/* Use KeyName to get value,CHAR16 type */
#define SettingGetValueC16S(KeyName,Value) \
            IniGetValueC16S(Setting,KeyName,Value)

/* Use KeyName to get value,UINT64 type */
#define SettingGetValueU64S(KeyName,Value) \
            IniGetValueU64S(Setting,KeyName,Value)

#endif
