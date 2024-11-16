section .text
        global drawJulia
drawJulia:
        mov eax, 4
        cvtsi2sd xmm12, eax
        cvtsi2sd xmm9, edx              ; temp1 = (double) lines; convert scalar integer to scalar double
        cvtsi2sd xmm10, esi             ; temp2 = (double) lineSize;
        divsd xmm5, xmm9                ; sizeIm /= temp1; dividing 2 floats
        divsd xmm3, xmm10               ; sizeRe /= temp2;
        xor r9, r9                      ; y = 0;
for_y:
        cvtsi2sd xmm6, r9               ; imZ_for_curr_y = (double)y;
        mulsd xmm6, xmm5                ; imZ_for_curr_y *= sizeIm; multiply 2 doubles
        addsd xmm6, xmm4                ; imZ_for_curr_y += minIm;
        xor r8, r8                      ; x = 0;
for_x:
        movsd xmm7, xmm6                ; imZ = imZ_for_curr_y;
        cvtsi2sd xmm8, r8               ; reZ = (double) x;
        mulsd xmm8, xmm3                ; reZ *= sizeRe;
        addsd xmm8, xmm2                ; reZ += minRe;
        mov rcx, 255                    ; counter = 255;
for_counter:
        movsd xmm9, xmm8                ; temp1 = reZ;
        mulsd xmm9, xmm8                ; temp1 *= reZ;
        movsd xmm10, xmm7               ; temp2 = imZ;
        mulsd xmm10, xmm7               ; temp2 *= imZ;
        movsd xmm11, xmm9               ; temp3 = temp1;
        addsd xmm11, xmm10              ; temp3 += temp2;
        ucomisd xmm11, xmm12            ; if (temp3 >= four) break; unordered compare scalar double precision floating
        jae for_x_end                   ; if (temp3 >= four) break; jump above or equal
        mulsd xmm7, xmm8                ; imZ *= reZ;
        addsd xmm7, xmm7                ; imZ += imZ;
        addsd xmm7, xmm1                ; imZ += imConst;
        movsd xmm8, xmm9                ; reZ = temp1;
        subsd xmm8, xmm10               ; reZ -= temp2;
        addsd xmm8, xmm0                ; reZ += reConst;
        loop for_counter                ; counter--; if (counter == 0) break;
for_x_end:
        mov byte [rdi], cl              ; *buffer = counter;
        inc rdi                         ; buffer++;
        inc r8                          ; x++;
        cmp r8, rsi                     ; if (x >= lineSize) break;
        jl for_x                        ; if (x >= lineSize) break;
        inc r9                          ; y++;
        cmp r9, rdx                     ; if (y >= lines) break;
        jl for_y
        
