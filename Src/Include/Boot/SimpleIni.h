#ifndef __SIMPLE_INI__
#define __SIMPLE_INI__

typedef struct {
  CHAR16 *Key;
  CHAR16 *Value;
} KV_TABLE;

typedef struct {
  UINT32   Count;
  KV_TABLE *Kvs;
} INI_INFO;

EFI_STATUS IniLoad (
        IN     CHAR16   *IniRaw,
           OUT INI_INFO *IniInfo
        );

VOID IniKvPrint (
        IN INI_INFO IniInfo
        );

CHAR16* IniGetValueC16 (
        IN     INI_INFO  IniInfo,
        IN     CHAR16    *KeyName
        );

EFI_STATUS IniGetValueC16S (
        IN     INI_INFO  IniInfo,
        IN     CHAR16    *KeyName,
           OUT CHAR16    **Value
        );

UINT64 IniGetValueU64 (
        IN     INI_INFO IniInfo,
        IN     CHAR16   *KeyName
        );

EFI_STATUS IniGetValueU64S (
        IN     INI_INFO IniInfo,
        IN     CHAR16   *KeyName,
           OUT UINT64   *Value
        );

#endif
