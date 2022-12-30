#!/usr/bin/env bash

sleep 5

while ps aux | grep qemu-system- | grep -v grep >/dev/null; do
	sleep 3
done

kill -9 $(ps aux | grep udk-gdb- | grep -v grep | awk '{print $2}') >/dev/null 2>&1
kill $(ps aux | grep gdb | grep -v grep | grep -v make | awk '{print $2}') >/dev/null 2>&1
rm -f $(cat /etc/udkdebugger.conf | grep -E "^[P ]ort" | awk '{print $NF}').{in,out}
