.entry:
    str_data: bytes "Hello"
    nums: bytes 20
    buffer: reserve 500 bytes
    
    ; Код программы
    mov %ax 42
    mov %bx nums
    push %cx
    add %ax %bx
    hlt
