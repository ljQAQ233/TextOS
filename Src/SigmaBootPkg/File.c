#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#include <Boot/File.h>

EFI_STATUS InitializeFileServices (
        IN EFI_HANDLE ImageHandle
        )
{
	EFI_STATUS Status = EFI_SUCCESS;

	UINTN 	   HandleCount   = 0;
	EFI_HANDLE *HandleBuffer = NULL;

	Status = gBS->LocateHandleBuffer (
            ByProtocol,
            &gEfiSimpleFileSystemProtocolGuid,
            NULL,
            &HandleCount,
            &HandleBuffer
            );
	if (EFI_ERROR(Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] LocateHandleBuffer : gEfiSimpleFileSystemProtocolGuid\n"));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] LocateHandleBuffer : gEfiSimpleFileSystemProtocolGuid\n"));

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
	Status = gBS->OpenProtocol (
		HandleBuffer[0],
		&gEfiSimpleFileSystemProtocolGuid,
		(VOID **)&FileSystem,
		ImageHandle,
		NULL,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
		);
	if (EFI_ERROR(Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] OpenProtocol : File Protocol\n"));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] OpenProtocol : FileProtocol\n"));
	FreePool (HandleBuffer);

	Status = FileSystem->OpenVolume (
		FileSystem,
		&gFileProtocol
		);
	if (EFI_ERROR(Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] Failed to open volume\n"));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Opened volume successfully\n"));

	return Status;
}

EFI_STATUS FileOpen (
        IN      CHAR16            *Path,
        IN      UINT64            Mode,
           OUT  EFI_FILE_PROTOCOL **File
	)
{
	EFI_STATUS Status = EFI_SUCCESS;

	EFI_FILE_PROTOCOL *FileTmp;
	Status = gFileProtocol->Open(
		gFileProtocol,
        &FileTmp,
		Path,Mode,
        0
		);
	if(EFI_ERROR(Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] Open File : %S\n",Path));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Open File : %S\n",Path));

	*File = FileTmp;

	return Status;
}

EFI_STATUS FileRead (
        IN     EFI_FILE_PROTOCOL *File,
           OUT VOID              *Data,
        IN OUT UINTN             *ReadSize
)
{
	EFI_STATUS Status = EFI_SUCCESS;

	Status = gFileProtocol->Read (
		File,
	 	ReadSize,
		Data
		);
	if (EFI_ERROR (Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] Read File\n"));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Read File\n"));

	return Status;
}

/*
EFI_STATUS FileReadPhysicalAddr(
	IN     EFI_FILE_PROTOCOL    *File,
	   OUT EFI_PHYSICAL_ADDRESS **Addr,
        IN     UINTN                *Size
	)
{
    EFI_STATUS Status;

    Status = File->Read(
	    File,
	    *Addr,
	    Size
	    );
    if(EFI_ERROR(Status))
    {
	DEBUG ((DEBUG_INFO ,"[  ERROR  ] Read File\n"));
	return Status;
    }
    DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Read File\n"));

    return Status;
}
*/

EFI_STATUS FileReadAuto (
	IN     EFI_FILE_PROTOCOL *File,
       OUT VOID              **Data,
       OUT UINTN             *ReadSize OPTIONAL
)
{
	EFI_STATUS Status = EFI_SUCCESS;

	EFI_FILE_INFO *Info = NULL;
	Status = FileGetInfo (
		File,
		&Info
		);
	if (EFI_ERROR (Status))
	{
	    DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to get file Info\n"));
        if (Info != NULL)
        {
            FreePool (Info);
            Info  = NULL;
        }
	    return Status;
	}

	UINTN Size = Info->FileSize;
	*Data = AllocatePool (Size);
	if (*Data == NULL)
	{
	    DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to get Memory\n"));
        FreePool (Info);
	    return EFI_OUT_OF_RESOURCES;
	}

    DEBUG ((DEBUG_INFO,"[  DEBUG  ] File Size : %u\n",Size));
	Status = FileRead (
            File,
            *Data,
            &Size
            );
	if (EFI_ERROR(Status))
	{
		DEBUG ((DEBUG_INFO,"[  ERROR  ] Auto Read File\n"));
        FreePool (*Data);
        FreePool (Info);
        return Status;
	}
	DEBUG ((DEBUG_INFO,"[ SUCCESS ] Auto Read File\n"));

    FreePool (Info);
    DEBUG ((DEBUG_INFO,"[ SUCCESS ] Free Info Buffer\n"));

    if (ReadSize != NULL)
    {
        *ReadSize = Size;
    }

	return Status;
}

EFI_STATUS FileWrite (
	IN     EFI_FILE_PROTOCOL *File,
	IN     VOID              *DataBuffer,
	IN     UINTN             *Size
)
{
	EFI_STATUS Status = EFI_SUCCESS;

	Status = File->Write (
            File,
            Size,
            DataBuffer
            );

	if (EFI_ERROR (Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] Write File\n"));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Write File\n"));

	return Status;
}

EFI_STATUS FileGetInfo (
	IN     EFI_FILE_PROTOCOL *File,
	   OUT EFI_FILE_INFO     **FileInfo
)
{
	EFI_STATUS Status = EFI_SUCCESS;

	UINTN InfoSize = 0;

	Status = File->GetInfo (
		File,
		&gEfiFileInfoGuid,
		&InfoSize,
		*FileInfo
		);
	if (Status == EFI_BUFFER_TOO_SMALL)
	{
        *FileInfo = AllocatePool (InfoSize);
	    DEBUG ((DEBUG_INFO,"[  DEBUG  ] Get Info Memory Space and Try again\n"));
	    Status = File->GetInfo (
                File,
                &gEfiFileInfoGuid,
                &InfoSize,
                *FileInfo
            );
	    if (EFI_ERROR (Status))
	    {
            DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to Get Info and Exit\n"));
            return EFI_UNSUPPORTED;
	    }
	}
	DEBUG ((DEBUG_INFO,"[ SUCCESS ] Got File Info\n"));

	return Status;

}

EFI_STATUS FileSetPosition (
	IN EFI_FILE_PROTOCOL    *File,
	IN UINT64               Position
	)
{
	EFI_STATUS Status = EFI_SUCCESS;

	Status = File->SetPosition (File,Position);
	if(EFI_ERROR(Status))
	{
		DEBUG ((DEBUG_INFO ,"[  ERROR  ] Set Position\n"));
		return Status;
	}
	DEBUG ((DEBUG_INFO ,"[ SUCCESS ] Set Position\n"));

	return Status;
}

UINT64 FileGetPosition (
    IN EFI_FILE_PROTOCOL *File
    )
{
    UINT64 Position = 0;
    
    if (EFI_ERROR (File->GetPosition (File,&Position)))
    {
        DEBUG ((DEBUG_INFO,"[  ERROR  ] Failed to get position of the ptr of this file\n"));
        return 0;
    }
    DEBUG ((DEBUG_INFO,"[  ERROR  ] Got position of this file ptr\n"));

    return Position;
}

EFI_STATUS FileClose (
	IN EFI_FILE_PROTOCOL *File
	)
{
	EFI_STATUS Status = File->Close(File);
	if(EFI_ERROR(Status))
	{
		DEBUG((DEBUG_INFO,"[  ERROR  ] Closed File\n"));
		return Status;
	}
	DEBUG((DEBUG_INFO,"[  ERROR  ] Closed File\n"));

	return Status;
}
