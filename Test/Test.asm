mov ax , 0b800h
mov ds , ax

mov byte [0x00],'H'
mov byte [0x02],'E'
mov byte [0x04],'L'
mov byte [0x06],'L'
mov byte [0x08],'O'
mov byte [0x0A],'W'
mov byte [0x0C],'O'
mov byte [0x0E],'R'
mov byte [0x10],'L'
mov byte [0x12],'D'
mov byte [0x14],'!'

jmp $

times 510-($-$$) db 0
dw 0xaa55
