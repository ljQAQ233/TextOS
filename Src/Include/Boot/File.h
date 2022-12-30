/* 这个文件存放着File.c的默认参数以及函数声明 */

#ifndef __FILE_H__
#define __FILE_H__

#define FILE_INFO_STRUCT_SIZE_DEFAULT 512

extern EFI_FILE_PROTOCOL *gFileProtocol;

EFI_STATUS
InitializeFileServices (
  IN EFI_HANDLE ImageHandle
  );

EFI_STATUS
FileOpen (
  IN      CHAR16             *Path,
  IN      UINT64             Mode,
     OUT  EFI_FILE_PROTOCOL  **OutFile
  );

EFI_STATUS
FileRead (
  IN      EFI_FILE_PROTOCOL  *File,
  IN OUT  VOID               *Data,
  IN OUT  UINTN              *ReadSize
  );

EFI_STATUS
FileReadAuto (
  IN     EFI_FILE_PROTOCOL   *File,
     OUT VOID            	 **Data,
     OUT UINTN               *ReadSize OPTIONAL
);

EFI_STATUS
FileReadPhysicalAddr(
  IN     EFI_FILE_PROTOCOL    *File,
     OUT EFI_PHYSICAL_ADDRESS **Addr,
  IN     UINTN                *Size
  );

EFI_STATUS
FileWrite (
  IN      EFI_FILE_PROTOCOL  *File,
  IN      VOID               *Buffer,
  IN      UINTN              *Size
  );

EFI_STATUS
FileGetInfo (
  IN      EFI_FILE_PROTOCOL  *File,
     OUT  EFI_FILE_INFO      **FileInfo
  );

EFI_STATUS
FileSetPosition (
  IN      EFI_FILE_PROTOCOL  *File,
  IN      UINT64             Position
  );

EFI_STATUS
FileClose(
  IN      EFI_FILE_PROTOCOL  *File
  );

#endif
