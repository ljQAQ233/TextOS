#ifndef __ELF_H__
#define __ELF_H__

#define ELF_MAGIC 0x464C457F

/* Elf Classes
 * Like Cpu Bits or Word Length
 */
#define ELFCLASSNONE   0
#define ELFCLASS32     1
#define ELFCLASS64     2

/* Elf Data 
 * Data Encodeing Type
 */
#define ELFDATANONE    0
#define ELFDATA_LSB    1
#define ELFDATA_MSB    2

/*
 * Elf Version 
 * The Current Version is 1
 */
#define EV_NONE        0
#define EV_CURRENT     1

/* Elf for which OS - OSABI 
 * We mustn't use it now,so only define the valid Macro 'ELFOSABI_LINUX'
 */
#define ELFOSABI_NONE  0
#define ELFOSABI_LINUX 3

/* Elf Pad Area 
 * Fill the Pad Area
 */
#define ELFRESERVED    0

/* Elf Machines
 * The Bootloader Supports the following types
 */
#define EM_386         3
#define EM_X86_64      62
#define EM_ARM         40
#define EM_AARCH64     183

/* Elf File Type 
 * There are more types But we use only the following
 */
#define ET_NONE        0 // Invalid Type
#define ET_REL         1 // Relocatable File
#define ET_EXEC        2 // Executable File
#define ET_DYN         3 // Shared Obj File

#if defined(MDE_CPU_X64)
  #define ELF_SUPPORTED_ARCH  EM_X86_64
  #define ELF_SUPPORTED_CLASS ELFCLASS64
#elif defined(MDE_CPU_IA32)
  #define ELF_SUPPORTED_ARCH  EM_386
  #define ELF_SUPPORTED_CLASS ELFCLASS32
#elif defined(MDE_CPU_AARCH64)
  #define ELF_SUPPORTED_ARCH  EM_AARCH64
  #define ELF_SUPPORTED_CLASS ELFCLASS64
#elif defined(MDE_CPU_ARM)
  #define ELF_SUPPORTED_ARCH  EM_ARM
  #define ELF_SUPPORTED_CLASS ELFCLASS32
#endif

#pragma pack(1)

typedef struct {
    UINT32 Sign;
    UINT8  Class;
    UINT8  Data;
    UINT8  Version;
    UINT8  OSABI;
    UINT8  ABIVersion;
    UINT8  Pad[6];
    UINT8  Nident;
    UINT16 Type;
    UINT16 Machine;
    UINT32 ObjFileVersion;
    UINT64 Entry;
    UINT64 PhOffset;
    UINT64 ShOffset;
    UINT32 Flags;
    UINT16 EhSize;
    UINT16 PhentSize;
    UINT16 PhNum;
    UINT16 ShentSize;
    UINT16 ShNum;
    UINT16 ShStrIdx;
} ELF_HEADER_64;

typedef struct {
    UINT32 Sign;
    UINT8  Class;
    UINT8  Data;
    UINT8  Version;
    UINT8  OSABI;
    UINT8  ABIVersion;
    UINT8  Pad[6];
    UINT8  Nident;
    UINT16 Type;
    UINT16 Machine;
    UINT32 ObjFileVersion;
    UINT32 Entry;
    UINT32 PhOffset;
    UINT32 ShOffset;
    UINT32 Flags;
    UINT16 EhSize;
    UINT16 PhentSize;
    UINT16 PhNum;
    UINT16 ShentSize;
    UINT16 ShNum;
    UINT16 ShStrIdx;
} ELF_HEADER_32;

typedef struct {
    UINT32 Sign;            // String Must be {0x7F,'E','L','F'},Type UINT32 Must be 0x464C457F
    UINT8  Class;           // Supported Bits
    UINT8  Data;            // Way to Encode
    UINT8  Version;         // Elf Version, 1
    UINT8  OSABI;           // Describes the file is for which OS to run
    UINT8  ABIVersion;      // Application Binary interface Version
    UINT8  Pad[6];          // Zero to Fill
    UINT8  Nident;          // Size of Ident
    UINT16 Type;            // Which Elf Type,Exec or Others
    UINT16 Machine;         // Arch / Machine
    UINT32 ObjFileVersion;  // This Member identifies the Object file Version
    UINTN  Entry;           // Virtual Addr
    UINTN  PhOffset;        // Programme Header Offset
    UINTN  ShOffset;        // Section Header Offset
    UINT32 Flags;           // In Standardization,Macros Start with EF_
    UINT16 EhSize;          // ELF header's size(bytes)
    UINT16 PhentSize;       // Programme Header Table Entry Size
    UINT16 PhNum;           // the Num of Programme Header Table Entry
    UINT16 ShentSize;       // Section Header Table Entry Size
    UINT16 ShNum;           // the Num of Section Header Table Entry
    UINT16 ShStrIdx;        // Section Header String Table Index
} ELF_HEADER;

#pragma pack()

#define PT_NONE 0
#define PT_LOAD 1

#pragma pack(1)

typedef struct {
  UINT32 Type;
  UINT32 Flgs;
  UINT64 Offset;
  UINT64 VirtualAddr;
  UINT64 PhysicalAddr;
  UINT64 FileSiz;
  UINT64 MemSiz;
  UINT64 Align;
} ELF_PHEADER_64;

typedef struct {
  UINT32 Type;
  UINT32 Offset;
  UINT32 VirtualAddr;
  UINT32 PhysicalAddr;
  UINT32 FileSiz;
  UINT32 MemSiz;
  UINT32 Flgs;
  UINT32 Align;
} ELF_PHEADER_32;

typedef struct {
  UINT32 Name;
  UINT32 Type;
  UINT64 Flags;
  UINT64 Addr;
  UINT64 Offset;
  UINT64 Siz;
  UINT32 Link;
  UINT32 Info;
  UINT64 AddrAlign;
  UINT64 EnTabSiz;
} ELF_SHEADER_64;

typedef struct {
  UINT32 Name;
  UINT32 Type;
  UINT32 Flags;
  UINT32 Addr;
  UINT32 Offset;
  UINT32 Siz;
  UINT32 Link;
  UINT32 Info;
  UINT32 AddrAlign;
  UINT32 EnTabSiz;
} ELF_SHEADER_32;

typedef struct {
  UINT32 Name;
  UINT32 Type;
  UINTN  Flags;
  UINTN  Addr;
  UINTN  Offset;
  UINTN  Siz;
  UINT32 Link;
  UINT32 Info;
  UINTN  AddrAlign;
  UINTN  EnTabSiz;
} ELF_SHEADER;

#pragma pack()

#if defined(MDE_CPU_X64) || defined(MDE_CPU_AARCH64)
  typedef ELF_PHEADER_64 ELF_PHEADER;
#elif defined(MDE_CPU_IA32) || defined(MDE_CPU_ARM)
  typedef ELF_PHEADER_32 ELF_PHEADER;
#endif

EFI_STATUS
ELfCheck (
  IN VOID *ElfBuffer
  );

VOID
LoadSegment (
  IN     VOID        *ElfBuffer,
  IN     ELF_PHEADER ProgHdr,
     OUT VOID        *Destination
  );

EFI_STATUS LoadSegments (
  IN     VOID                 *ElfBuffer,
     OUT EFI_PHYSICAL_ADDRESS *KernelEntry
  );

#endif
