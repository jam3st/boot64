# boot64

gdb -ex "add-auto-load-safe-path $PWD" -ex "file x/boot" -ex "set arch i386:x86-64:intel"  -ex "target remote 192.168.1.2:1234"  -ex "hbreak hbreak_start" -ex "continue"

