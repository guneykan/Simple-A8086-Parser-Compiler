code segment
  call myread
  mov vi,cx
  push offset vaprev
  push 7
  pop ax
  pop bp
  mov [bp],ax
  push offset vn
  push 1
  pop ax
  pop bp
  mov [bp],ax
LABL1:
  push vi w
  pop ax
  cmp ax,0
  if z jmp LABL2
  push offset vn
  push vn w
  push 1
  mov	dx,0
  pop cx
  pop ax
  add ax,cx
  push ax
  pop ax
  pop bp
  mov [bp],ax
  push offset vk
  push vn w
  pop ax
  pop bp
  mov [bp],ax
  push offset vm
  push vaprev w
  pop ax
  pop bp
  mov [bp],ax
LABL3:
  push vm w
  pop ax
  cmp ax,0
  if z jmp LABL4
  push offset vt
  push vm w
  pop ax
  pop bp
  mov [bp],ax
  push offset vm
  push vk w
  push vm w
  xor dx,dx
  pop cx
  pop ax
  div cx
  push dx
  pop ax
  pop bp
  mov [bp],ax
  push offset vk
  push vt w
  pop ax
  pop bp
  mov [bp],ax
  jmp LABL3
LABL4:
  push offset vanew
  push vaprev w
  push vk w
  mov	dx,0
  pop cx
  pop ax
  add ax,cx
  push ax
  pop ax
  pop bp
  mov [bp],ax
  push offset vi
  push vi w
  push 1
  pop cx
  pop ax
  sub ax,cx
  push ax
  pop ax
  pop bp
  mov [bp],ax
  push vanew w
  push vaprev w
  pop cx
  pop ax
  sub ax,cx
  push ax
  push 1
  pop cx
  pop ax
  sub ax,cx
  push ax
  pop ax
  cmp ax,0
  if z jmp LABL5
  push vanew w
  push vaprev w
  pop cx
  pop ax
  sub ax,cx
  push ax
  pop ax
  call myprint
LABL5 :
  push offset vaprev
  push vanew w
  pop ax
  pop bp
  mov [bp],ax
  jmp LABL1
LABL2:
  int  20h       ; exit to dos 

myread:
  MOV  CX,0
morechar:
  mov  ah,01h 
  int  21H
  mov  dx,0
  mov  dl,al
  mov  ax,cx
  cmp  dl,0D
  je   myret
  sub  dx,48d
  mov  bp,dx
  mov  ax,cx
  mov  cx,10d
  mul  cx
  add  ax,bp
  mov  cx,ax
  jmp morechar
  myret:
  ret 

myprint:
  mov    si,10d
  xor    dx,dx
  push   0Ah    ; push newline
  mov    cx,1d
nonzero:
  div    si
  add    dx,48d
  push   dx
  inc    cx
  xor    dx,dx
  cmp    ax,0h
  jne    nonzero
writeloop:
  pop    dx
  mov    ah,02h
  int    21h
  dec    cx
  jnz    writeloop
  ret

vanew dw (?)
vaprev dw (?)
vi dw (?)
vk dw (?)
vm dw (?)
vn dw (?)
vt dw (?)
code ends
