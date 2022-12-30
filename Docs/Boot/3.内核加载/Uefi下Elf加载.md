# 注意
除了在定义适用于 32 / 64 位CPU的Elf Header及Elf Programme Header结构体除外,还添加与BootLoader相关的`ELF_HEADER`与`ELF_PHEADER`,使得**一个架构的BootLoader只能引导一个对应架构的内核**.
由于UEFI为64位定义了UINTN为UINT64,而32位为UINT32,这使得`ELF_HEADER`不需要被`typedef`而在原来基础上将**Addr相关内容的数据类型**替换为`UINTN`即可支持**相应的**内核.
由于在`ElfCheck`中,不需要涉及地址相关内容,可以直接将其识别为相应正确的内核,在具体检查时再修正,并不会对原内容产生影响(是废话了吧?)

# 参考资料
- https://b23.tv/vSWnVOz 操作系统实现 - 108 ELF 文件解析
- https://gitee.com/tangyugang/UEFI
- https://github.com/ajxs/uefi-elf-bootloader
- /usr/include/elf.h & `man elf`
