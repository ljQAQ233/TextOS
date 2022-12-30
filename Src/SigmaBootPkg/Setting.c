#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/SimpleFileSystem.h>

#include <Guid/FileInfo.h>

#include <Boot/Boot.h>
#include <Boot/File.h>
#include <Boot/Setting.h>

STATIC UINT8 Magic[2];

EFI_STATUS SettingInit (
        IN EFI_HANDLE ImageHandle
        )
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_FILE_PROTOCOL *File = NULL;
    Status = FileOpen (
            SETTING_PATH,
            EFI_FILE_MODE_READ,
            &File
        );
    if (EFI_ERROR(Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to open the setting File\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Opened the setting file\n"));

    EFI_FILE_INFO *Info;

    Status = FileGetInfo (
        File,
        &Info
        );
    if (EFI_ERROR(Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Got setting file info\n"));
        return Status;
    }
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Got setting file info\n"));

    CHAR16 *Raw     = NULL;
    UINTN  ReadSize = 2;
    Status = FileRead (
            File,
            Magic,
            &ReadSize
            );
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Falied to read .ini file to get magic number to check classes\n"));
        return Status;
    }
    else
    {
        if (Magic[0] == 0xFF && Magic[1] == 0xFE)
        {
            ReadSize = Info->FileSize - 2;
//            Raw = AllocateZeroPool (ReadSize);
            Raw = AllocatePages (EFI_SIZE_TO_PAGES(ReadSize));
            Status = FileRead (
                    File,
                    Raw,
                    &ReadSize
                    );
            if (EFI_ERROR (Status))
            {
                DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to read .ini file,CHAR16 String\n"));
                goto Exit;
            }
        }
        else
        {
            Status = FileSetPosition (File,0);
            if (EFI_ERROR (Status))
            {
                DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to reset  the file position\n"));
                return Status;
            }
            ReadSize = Info->FileSize;
            CHAR8* C8Raw = AllocateZeroPool (ReadSize);
            Status = FileRead (
                    File,
                    C8Raw,
                    &ReadSize
                    );
            if (EFI_ERROR (Status))
            {
                DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to read .ini file,CHAR16 String\n"));
                goto Exit;
            }
            // Raw = AllocateZeroPool (ReadSize * (sizeof (CHAR16) / sizeof (CHAR8)));
            Raw = AllocatePages (EFI_SIZE_TO_PAGES(ReadSize * (sizeof (CHAR16) / sizeof (CHAR8))));
            AsciiStrToUnicodeStr (C8Raw,Raw);
            
            FreePool (C8Raw);
        }
    }

    Status = IniLoad (
            Raw,
            &Setting
            );
    if (EFI_ERROR (Status))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to load .ini file\n"));
        goto Exit;
    }

Exit:
    if (Raw != NULL)
    {
        FreePages (Raw,EFI_SIZE_TO_PAGES(ReadSize));
    }
    if (Info != NULL)
    {
        FreePool (Info);
    }

    return Status;
}

