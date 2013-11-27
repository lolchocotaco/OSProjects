execve("./prob4.exe", ["./prob4.exe"], [/* 36 vars */]) = 0
mprotect(0x1, 6291672, PROT_WRITE|PROT_EXEC) = -1 EINVAL (Invalid argument)
_exit(1)                                = ?
execve("./prob4.exe", ["./prob4.exe"], [/* 40 vars */]) = 0
write(1, 0x1, 6)                        = -1 EFAULT (Bad address)
_exit(1)                                = ?
