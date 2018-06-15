org 0x7C00
bits 16
jmp _initialize
%include "inc/print.inc"

_initialize:
  mov [BOOT_DRIVE], dl

  xor ax, ax
  mov ds, ax
  mov es, ax

  mov ax, 0x7E00
  mov ss, ax

  mov ah, 00h
  int 10h

  mov ah, 01h
  mov cx, 0007h
  int 10h
reset_disk:
  mov ah, 0
  mov dl, [BOOT_DRIVE]
  int 13h
  jc .error
  jmp .success
  .error:
  mov si, reset_disk_error
  call print
  hlt
  .success:
  mov si, reset_disk_success
  call print

  mov cx, 5 ; repeat 5 times
read_sector:
  sub cx, byte 1
  push cx

  mov bx, 0x8000
  mov es, bx
  xor bx, bx ; [es:bx] 0x8000:0x0 

  mov ax, 0202h
  mov cx, 0002h
  mov dh, 00h
  mov dl, [BOOT_DRIVE]
  int 13h

  pop cx
  or cx, byte 0
  jz .error
  jc read_sector
  jmp .success
  .error:
  mov si, read_sector_error
  call print

  .success:
  mov si, read_sector_success
  call print
  jmp 0x8000:0x0

read_sector_success db "Sectors successfully read", 0xa, 0
read_sector_error db "Sectors could not be read", 0xa, 0

reset_disk_success db "Disk successfully reset", 0xa, 0
reset_disk_error db "could not reset disk", 0xa, 0
BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xAA55
