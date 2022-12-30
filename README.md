# 前言

这是一个Uefi引导的操作系统项目,在B站不定期更新,有错误请提交issue,谢谢!学生党,更新慢,请见谅.

# Src

与一般的目录结构大不相同...

- Base    常用工具 & 开发资源
- Boot    基于EDKII项目改,使用Makefile
- Config  Makefile配置
- Include 头文件目录
   - Boot SigmaBootPkg的头文件
   - StdLib 未开发
   - TextOS 内核头文件
- Kernel  内核源代码
   - Arch 平台代码
   - Console 内核控制台
   - Graphics 显示相关
   - Kernel   暂未使用
   - Lib  库支持
   - Sys  系统相关
   - Tools 工具
- Res     启动及运行时资源
- SigmaBootPkg Boot源码

# TODO

- 中断函数
- 内存分页 & 内存管理
- 硬盘支持

# 备注

`Bmp.c`中的Bmp格式转换的实现也可以不自己造轮子,而引用**新版**MdeModulePkg的`Library/BaseBmpSupportLib/BaseBmpSupportLib.inf`

# 资料

- Docs
- https://wiki.osdev.org
- https://gitee.com/tangyugang/UEFI
- https://github.com/ajxs/uefi-elf-bootloader
- 罗冰《Uefi编程实践》https://gitee.com/luobing4365/uefi-practical-programming
- 戴振华《Uefi原理与编程》

另外参考B站大佬 @踌躇月光 & @Lunaixsky 的视频.
