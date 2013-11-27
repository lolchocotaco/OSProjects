# Sameer Chauhan
# Write and Exit System calls

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
 
    # Leave the program calling 
    # exit(return_code)
 
    movq    $60, %rax         # sys_exit is 60
    movq    $1, %rdi         # save argument of 1 (0 is success)
    syscall				 # call exit

.data
    
message: 
        .ascii "Hello\n"
        length = . - message        # text and \n
