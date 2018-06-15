%ifndef print_bl
%define print_bl
bits 16

move_cursor_right: ; bp+16 = col, bp+8 = row, bp+4 = page

  mov ah, 03h ; get cursor
  mov bx, 0
  int 10h

  mov ah, 02h
  add dl, byte 1
  int 10h

  ret 
move_cursor_down:
  mov ah, 03h ; get cursor
  mov bx, 0
  int 10h

  ; dh = row dl = col
  mov ah, 02h
  add dh, byte 1
  xor dl, dl
  int 10h

  ret
print:
  lodsb
  or al, al
  jz .end
  cmp al, 0xa
  je .new_line

  .char:
    mov ah, 09h
    mov bx, 000fh
    mov cx, 1
    int 10h
    call move_cursor_right
    jmp print
  .new_line:
    call move_cursor_down
    jmp print
  .end:
    ret
  %endif