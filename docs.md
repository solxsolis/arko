; // buffer <=> rdi
; // lineSize <=> rsi
; // lines <=> rdx
; counter <=> ecx
; x <=> r8
; y <=> r9, 
; // reConst <=> xmm0
; // imConst <=> xmm1
; // minRe <=> xmm2
; // sizeRe <=> xmm3
; // minIm <=> xmm4
; // sizeIm <=> xmm5
; // imZ_for_curr_y <=> xmm6
; // imZ <=> xmm7
; // reZ <=> xmm8
; // temp1 <=> xmm9
; // temp2 <=> xmm10
; // temp3 <=> xmm11
; // four <=> xmm12


void drawJulia(uint8_t * buffer, uint32_t lineSize, uint32_t lines, double reConst, double imConst, double minRe, double sizeRe, double minIm, double sizeIm) {
    // buffer <=> edi (32)
    // lineSize <=> esi (32)
    // lines <=> edx (32)
    // reConst <=> xmm0
    // imConst <=> xmm1
    // minRe <=> xmm2
    // sizeRe <=> xmm3
    // minIm <=> xmm4
    // sizeIm <=> xmm5
    const double FOUR = 4.0; // four: dq 4.0
    int x, y, counter; // x <=> r8, y <=> r9, counter <=> rcx
    double imZ_for_curr_y, imZ, reZ, temp1, temp2, temp3, four;
    // imZ_for_curr_y <=> xmm6
    // imZ <=> xmm7
    // reZ <=> xmm8
    // temp1 <=> xmm9
    // temp2 <=> xmm10
    // temp3 <=> xmm11
    // four <=> xmm12
    four = FOUR;
    temp1 = (double) lines;
    temp2 = (double) lineSize;
    sizeIm /= temp1;
    sizeRe /= temp2;
    y = 0;
    while (true) {
        imZ_for_curr_y = (double)y;
        imZ_for_curr_y *= sizeIm;
        imZ_for_curr_y += minIm;
        x = 0;
        while(true) {
            imZ = imZ_for_curr_y;
            // reZ = x * sizeRe + minRe;
            reZ = (double) x;
            reZ *= sizeRe;
            reZ += minRe;

            counter = 255;
            while (true) {
                temp1 = reZ;
                temp1 *= reZ;
                temp2 = imZ;
                temp2 *= imZ;
                temp3 = temp1;
                temp3 += temp2;
                if (temp3 >= four) break;
                imZ *= reZ;
                imZ += imZ;
                imZ += imConst;
                reZ = temp1;
                reZ -= temp2;
                reZ += reConst;
                counter--;
                if (counter == 0) break;
            }
            *buffer = counter;
            buffer++;
            x++;
            if (x >= lineSize) break;
        }
        y++;
        if (y >= lines) break;
    }
}