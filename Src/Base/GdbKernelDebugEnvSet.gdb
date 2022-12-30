define kernelSym
  if GdbDebugInfo.KernelRuntime == 1
    set variable $KernelEntry = GdbDebugInfo.KernelEntry
    add-symbol-file /home/maouai233/Project/TextOS/Build/Kernel/Kernel.elf.symbol $KernelEntry
  end
end
define hook-stop
  kernelSym
end

