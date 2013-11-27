# Sameer Chauhan
# Write-only system call

.text

    .global _start            # tells ld where to start
 
_start:
 
    # calls the write system write function as
    # write(stdout, message, length)
    # rdi, rsi, & rdx are arguments
 
    movq    $1, %rax        # sys_write is 1
    movq    $1, %rdi        # stdout is 1
    movq    $message, %rsi  # message address
    movq    $length, %rdx   # message string length
    syscall              # call write
 
.data
    
message: 
        .ascii "Hello\n"
        length = . - message        # text and \n
