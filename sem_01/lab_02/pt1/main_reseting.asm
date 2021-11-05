.386P

; Описание структуры дескриптора сегмента
descr   struc
    limit   dw  0
    base_l  dw  0
    base_m  db  0
    attr_1  db  0
    attr_2  db  0
    base_h  db  0
descr   ends

; Сегмент данных
; use16 - использование 16-битных адресов по умолчанию.
data segment use16
    gdt_null descr   <0,0,0,0,0,0> ; нулевой дескриптор
    gdt_data descr   <data_size-1,0,0,92h,0,0> ; сегмент данных
    gdt_code descr   <code_size-1,0,0,98h,0,0> ; сегмент команд
    gdt_stack descr  <255,0,0,92h,0,0> ; сегмент стека
    gdt_screen descr <4095,8000h,0Bh,92h,0,0> ; видеопамять
    gdt_datacheck descr<0FFFFh,0,0,92h,11001111b,0>
    gdt_size=$-gdt_null

    pdescr  df  0

    realmode     db  'REAL MODE PRINT'
    realmode_len=$-realmode
    protectmode  db  'PROTECTED MODE PRINT'
    protectmode_len=$-protectmode

    color=1Fh
    col=26
    row=10

    data_size=$-gdt_null
data ends


text segment 'code' use16
    assume  cs:text, ds:data
    main    proc
        ; Вывод сообщения из реального режима
        xor ax, ax
        mov ax, data
        mov ds, ax
        mov ax, 0B800h
        mov es, ax

        mov di, (row + 1) * 160 + col * 2
        mov bx, offset realmode
        mov cx, realmode_len
        mov ah, color

        loop_01:
            mov al, byte ptr [bx]
            inc bx
            stosw
        loop loop_01

        ; Загрузка линейного адреса сегмента данных в GDT
        xor eax, eax
        mov ax, data
        shl eax, 4
        mov ebp, eax
        mov bx, offset gdt_data
        mov (descr ptr [bx]).base_l, ax
        shr eax, 16
        mov (descr ptr [bx]).base_m, al

        ; Загрузка линейного адреса сегмента команд в GDT
        xor eax, eax
        mov ax, cs
        shl eax, 4
        mov bx, offset gdt_code
        mov (descr ptr [bx]).base_l, ax
        shr eax, 16
        mov (descr ptr [bx]).base_m, al

        ; Загрузка линейного адреса сегмента стека в GDT
        xor eax, eax
        mov ax, ss
        shl eax, 4
        mov bx, offset gdt_stack
        mov (descr ptr [bx]).base_l, ax
        shr eax, 16
        mov (descr ptr [bx]).base_m, al

        ; Подготовка псевдодескриптора pdescr и загрузка в регистр GDTR
        mov dword ptr pdescr + 2, ebp
        mov word ptr pdescr, gdt_size - 1
        lgdt pdescr

        ; Подготовка к возврату
        mov AX,40h
        mov ES,AX
        mov word ptr ES:[67h],offset return
        mov ES:[69h],CS
        mov AL,0Fh
        out 70h,AL
        mov AL,0Ah
        out 71h,AL
        cli

        ; Переход в защищенный режим
        mov eax, cr0
        or  eax, 1
        mov cr0, eax

        ; Искусственно сконструированная команда дальнего перехода для смены CS:IP
        db  0EAh
        dw  offset continue
        dw  16

    continue:
        ; Заносим в ds селектор сегмента данных
        mov ax, 8
        mov ds, ax

        ; Заносим в ss селектор сегмента стека
        mov ax, 24
        mov ss, ax

        ; Заносим в es селектор сегмента видеопамяти
        mov ax, 32
        mov es, ax

        ; Заносим в GS селектор сегмента размера 4ГБ
        mov ax, 40
        mov gs, ax

        ; Вывод сообщения в защищенном режиме
        mov di, (row + 1) * 160 + col * 2
        mov bx, offset protectmode
        mov cx, protectmode_len
        mov ah, color

        loop_02:
            mov al, byte ptr [bx]
            inc bx
            stosw
        loop loop_02

    ; Возврат в реальный режим
        mov AL,0FEh
        out 64h,AL
        hit

    return:
    ; Восстановление вычислительное средыреального времени
        mov ax, data
        mov ds, ax
        mov ax, stk
        mov ss, ax
        mov sp, 256

        sti

    ; Вывод сообщения в реальном режиме
        mov di, (row + 2) * 160 + col * 2
        mov bx, offset realmode
        mov cx, realmode_len
        mov ah, color

        loop_03:
            mov al, byte ptr [bx]
            inc bx
            stosw
        loop loop_03

    ; Завершим программу обычным образом
        mov ax, 4C00h
        int 21h

    main    endp
    code_size=$-main
text    ends

stk segment stack 'stack'
    db  256 dup('^')
stk ends
end main
