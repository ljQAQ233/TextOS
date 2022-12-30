#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Boot/SimpleIni.h>

EFI_STATUS IniLoad (
        IN     CHAR16* IniRaw,
           OUT INI_INFO *IniInfo
        )
{
    if (IniInfo == NULL || IniRaw == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invlaid Parameters,can't be NULL\n"));
        return EFI_INVALID_PARAMETER;
    }
    /* Basic init */
    IniInfo->Count = 0;
    IniInfo->Kvs   = NULL;

    UINTN  IniRawLen = StrLen (IniRaw);

    EFI_STATUS Status = EFI_SUCCESS;

    /* use '\0' to separate ini string to make it easier to manage,the EDKII uses '\0' for the end of strings */
    for (UINTN Idx = 0;Idx < IniRawLen;Idx++)
    {
        if (IniRaw[Idx] == L'\n')
        {
            IniRaw[Idx] = L'\0';
        }
    }

    BOOLEAN Skip   = FALSE;
    UINTN IniTrLen = 0;

    /* Get the size of the valid infomation */
    for (UINTN Idx = 0;Idx < IniRawLen;Idx++)
    {
        if (IniRaw[Idx] == L';')
        {
            Skip = TRUE;
        }
        else if (IniRaw[Idx] == L'\0')
        {
            Skip = FALSE;
        }

        if (!Skip)
        {
            IniTrLen++;
        }
    }
    CHAR16* IniTr = AllocateZeroPool ((IniTrLen + 1) * sizeof(CHAR16));

    /* Remove the invalid line and the unsed line */
    Skip = (StrStr (IniRaw,L"=") == NULL) ? TRUE : FALSE;
    for (UINTN Idx = 0,TrIdx = 0;Idx < IniRawLen;Idx++)
    {
        if (IniRaw[Idx] == L';')
        {
            Skip = TRUE;
        }
        else if (IniRaw[Idx] == L'\0')
        {
            Skip = (StrStr (IniRaw + Idx + 1,L"=") == NULL) ? TRUE: FALSE;
        }

        if (!Skip)
        {
            IniTr[TrIdx++] = IniRaw[Idx];
        }
    }
    DEBUG ((DEBUG_INFO,"[  DEBUG  ] Ini transformed string : [%s]\n",IniTr));

    /* Get the count of the info that transformed using char '=' */
    for (UINTN Idx = 0;Idx < IniTrLen;Idx++)
    {
        if (IniTr[Idx] == '=')
        {
            IniInfo->Count++;
        }
    }
    /* Create some tabs for key and its value to save */
    IniInfo->Kvs = AllocateZeroPool (IniInfo->Count * sizeof (KV_TABLE));

    /* Create a table to save the KVs start address for indexing them */
    UINTN *KvIdxTab = AllocateZeroPool (IniInfo->Count * sizeof (UINTN));
    for (UINTN Idx = 0,KvIdx = 1;Idx < IniTrLen && KvIdx < IniInfo->Count;Idx++)
    {
        if (IniTr[Idx] == L'\0')
        {
            KvIdxTab[KvIdx++] = Idx + 1;
        }
    }

    UINTN  KeyLen,ValueLen;
    CHAR16 *KeyStart,*ValueStart;
    for (UINTN KvIdx = 0;KvIdx < IniInfo->Count;KvIdx++)
    {
        KeyStart = IniTr + KvIdxTab[KvIdx];
        KeyLen = StrStr (IniTr + KvIdxTab[KvIdx],L"=") - (IniTr + KvIdxTab[KvIdx]);
        for (CHAR16* KeyEnd = KeyStart + KeyLen - 1;*KeyEnd == L' ';KeyEnd--,KeyLen--)
        {
            ;
        }
        while (*KeyStart == ' ')
        {
            KeyStart++;
            KeyLen--;
        }
        IniInfo->Kvs[KvIdx].Key = AllocateZeroPool ((KeyLen + 1) * sizeof (CHAR16));
        StrnCpy (
                IniInfo->Kvs[KvIdx].Key,
                KeyStart,
                KeyLen
                );

        ValueStart = StrStr (IniTr + KvIdxTab[KvIdx],L"=") + 1;
        ValueLen = StrLen (ValueStart);
        for (CHAR16* ValueEnd = ValueStart + ValueLen - 1;*ValueEnd == L' ';ValueEnd--,ValueLen--)
        {
            ;
        }
        while (*ValueStart == L' ')
        {
            ValueStart++;
            ValueLen--;
        }
        IniInfo->Kvs[KvIdx].Value = AllocateZeroPool ((ValueLen + 1) * sizeof (CHAR16));
        StrnCpy (
                IniInfo->Kvs[KvIdx].Value,
                ValueStart,
                ValueLen
                );
    }
    IniKvPrint (*IniInfo);

    FreePool (IniTr);
    FreePool (KvIdxTab);

    return Status;
}

CHAR16* IniGetValueC16 (
        IN     INI_INFO  IniInfo,
        IN     CHAR16*   KeyName
        )
{
    if (IniInfo.Count == 0)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid INI_INFO : Count = 0\n"));
        return NULL;
    }
    if (IniInfo.Kvs == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid INI_INFO : Kvs = NULL\n"));
        return NULL;
    }
    if (KeyName == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid Parameter : KeyName\n"));
        return NULL;
    }

    for (UINTN Idx = 0;Idx < IniInfo.Count;Idx++)
    {
        if (StrCmp (IniInfo.Kvs[Idx].Key,KeyName) == 0)
        {
            return IniInfo.Kvs[Idx].Value;
        }
        if (Idx == IniInfo.Count - 1)
        {
            return NULL;
        }
    }

    return NULL;
}

EFI_STATUS IniGetValueC16S (
        IN     INI_INFO  IniInfo,
        IN     CHAR16*   KeyName,
           OUT CHAR16**  Value
        )
{
    if (IniInfo.Count == 0)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid INI_INFO : Count = 0\n"));
        return EFI_INVALID_PARAMETER;
    }
    if (IniInfo.Kvs == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid INI_INFO : Kvs = NULL\n"));
        return EFI_INVALID_PARAMETER;
    }
    if (KeyName == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Invalid Parameter : KeyName\n"));
        return EFI_INVALID_PARAMETER;
    }

    for (UINTN Idx = 0;Idx < IniInfo.Count;Idx++)
    {
        if (StrCmp (IniInfo.Kvs[Idx].Key,KeyName) == 0)
        {
            if (*Value == NULL)
            {
                *Value = AllocateZeroPool (StrLen (IniInfo.Kvs[Idx].Key) * sizeof (CHAR16));
            }
            StrCpy (*Value,IniInfo.Kvs[Idx].Value);
        }
        if (Idx == IniInfo.Count - 1)
        {
            return EFI_UNSUPPORTED;
        }
    }

    return EFI_SUCCESS;
}

UINT64 IniGetValueU64 (
        IN     INI_INFO IniInfo,
        IN     CHAR16   *KeyName
        )
{
    CHAR16 *String = IniGetValueC16 (IniInfo,KeyName);
    if (String == NULL)
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to get value of %S\n",KeyName));
        return -1;
    }
    
    return StrDecimalToUint64 (String);
}

EFI_STATUS IniGetValueU64S (
        IN     INI_INFO IniInfo,
        IN     CHAR16   *KeyName,
           OUT UINT64   *Value
        )
{
    EFI_STATUS Status = EFI_SUCCESS;

    CHAR16 *String = IniGetValueC16 (IniInfo,KeyName);
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to get value of %S\n",KeyName));
        return Status;
    }
    
    *Value = StrDecimalToUint64 (String);

    return Status;
}

VOID IniKvPrint (
        IN INI_INFO IniInfo
        )
{
    for (UINTN KvIdx = 0;KvIdx < IniInfo.Count;KvIdx++)
    {
        Print (L"[ INIINFO ] KvIdx : %02u - Key : [%s] \n",KvIdx,IniInfo.Kvs[KvIdx].Key);
        Print (L"[ INIINFO ] KvIdx : %02u - Value : [%s] \n",KvIdx,IniInfo.Kvs[KvIdx].Value);
    }
}
