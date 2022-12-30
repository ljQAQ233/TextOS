#!/usr/bin/env bash 
 while ! LANG= echo q | sudo -S netstat -tnlp | grep udk-| grep -v 20715 >/dev/null 2>&1;do 
   sleep 1 
 done 

st gdb --eval-command="target remote :`LANG= sudo netstat -tnlp | grep udk-| grep -v 20715 | awk '{print $4}' | awk -F ':' '{print $NF}'`" --eval-command="source /opt/intel/udkdebugger/script/udk_gdb_script" --eval-command="source Base/GdbKernelDebugEnvSet.gdb" --eval-command="c" --eval-command="loadthis"
exit 0
