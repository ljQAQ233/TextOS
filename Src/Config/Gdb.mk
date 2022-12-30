UDK_DBG_HOME := /opt/intel/udkdebugger
# the Root of Software UdkDebugger we have Installed brfore
UDK_DBG_CFG  := /etc/udkdebugger.conf
# the Config file for udkdebugger

UDK_DBG_EXEC := $(UDK_DBG_HOME)/bin/udk-gdb-server
# the Executable file of UDK DEBUGGER

GDB_STARTER_CMD := st gdb \
                 --eval-command=\"target remote :\`LANG= sudo netstat -tnlp \| grep udk-\| grep -v 20715 \| awk \'{print \$$4}\' \| awk -F \':\' \'{print \$$NF}\'\`\" \
                 --eval-command=\"source $(UDK_DBG_HOME)/script/udk_gdb_script\" \
                 --eval-command=\"source Base/GdbKernelDebugEnvSet.gdb\" \
                 --eval-command=\"c\" \
                 --eval-command=\"loadthis\"
# Exec Some Cmd for Init before debugging

export UDK_DBG_HOME UDK_DBG_CFG UDK_DBG_EXEC GDB_STARTER_CMD
