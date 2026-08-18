        .org 0x0000
L0:     jmp L1
        jmp L8
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x0134
        .dw 0x940c
        .dw 0x010b
        .dw 0x940c
        .dw 0x015d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x015e
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x940c
        .dw 0x005d
        .dw 0x02fe
L1:     clr r1
        out 0x3f, r1
        ser r28
        ldi r29, 8
        out 0x3e, r29
        out 0x3d, r28
        ldi r17, 1
        ldi r26, 0
        ldi r27, 1
        ldi r30, 76
        ldi r31, 8
        rjmp L3
L2:     lpm r0, Z+
        st X+, r0
L3:     cpi r26, 56
        cpc r27, r17
        brne L2
        ldi r18, 1
        ldi r26, 56
        ldi r27, 1
        rjmp L5
L4:     st X+, r1
L5:     cpi r26, 222
        cpc r27, r18
        brne L4
        ldi r17, 0
        ldi r28, 53
        ldi r29, 0
        rjmp L7
L6:     sbiw r29:r28, 1
        movw r31:r30, r29:r28
        call L54
L7:     cpi r28, 52
        cpc r29, r17
        brne L6
        call L16
        jmp L66
L8:     jmp L0
        .dw 0x9120
        .dw 0x01da
        .dw 0x9130
        .dw 0x01db
        .dw 0x9180
        .dw 0x01dc
        .dw 0x9190
        .dw 0x01dd
        .dw 0x1728
        .dw 0x0739
        .dw 0xf431
        .dw 0x9180
        .dw 0x00c1
        .dw 0x7d8f
        .dw 0x9380
        .dw 0x00c1
        .dw 0x9508
        .dw 0x91e0
        .dw 0x01dc
        .dw 0x91f0
        .dw 0x01dd
        .dw 0x56e6
        .dw 0x4ffe
        .dw 0x8120
        .dw 0x9180
        .dw 0x01dc
        .dw 0x9190
        .dw 0x01dd
        .dw 0x9583
        .dw 0x738f
        .dw 0xe090
        .dw 0x9390
        .dw 0x01dd
        .dw 0x9380
        .dw 0x01dc
        .dw 0x9320
        .dw 0x00c6
        .dw 0x9508
        .dw 0x93cf
        .dw 0x93df
        .dw 0x920f
        .dw 0xb7cd
        .dw 0xb7de
        .dw 0x9180
        .dw 0x0196
        .dw 0x9190
        .dw 0x0197
        .dw 0x9120
        .dw 0x0198
        .dw 0x9130
        .dw 0x0199
        .dw 0x9583
        .dw 0x738f
        .dw 0xe090
        .dw 0x1782
        .dw 0x0793
        .dw 0xf0f9
        .dw 0x9120
        .dw 0x00c6
        .dw 0x9180
        .dw 0x0196
        .dw 0x9190
        .dw 0x0197
        .dw 0x9583
        .dw 0x738f
        .dw 0xe090
        .dw 0x9140
        .dw 0x0198
        .dw 0x9150
        .dw 0x0199
        .dw 0x1784
        .dw 0x0795
        .dw 0xf059
        .dw 0x91e0
        .dw 0x0196
        .dw 0x91f0
        .dw 0x0197
        .dw 0x5aea
        .dw 0x4ffe
        .dw 0x8320
        .dw 0x9390
        .dw 0x0197
        .dw 0x9380
        .dw 0x0196
        .dw 0x900f
        .dw 0x91df
        .dw 0x91cf
        .dw 0x9508
        .dw 0x9180
        .dw 0x00c6
        .dw 0x8389
        .dw 0x8189
        .dw 0xcff7
L9:     lds r18, 0x1da
        lds r19, 0x1db
        lds r20, 0x1dc
        lds r21, 0x1dd
        inc r18
        andi r18, 63
        ldi r19, 0
        cp r18, r20
        cpc r19, r21
        breq L9
        lds r18, 0x1da
        lds r19, 0x1db
        inc r18
        andi r18, 63
        ldi r19, 0
        lds r20, 0x1dc
        lds r21, 0x1dd
        cp r18, r20
        cpc r19, r21
        breq L10
        lds r30, 0x1da
        lds r31, 0x1db
        subi r30, 102
        sbci r31, 254
        st Z, r24
        sts 0x1db, r19
        sts 0x1da, r18
L10:    lds r24, 0xc1
        ori r24, 32
        sts 0xc1, r24
        sei
        ret
L11:    push r28
        push r29
        rcall L12
L12:    rcall L13
L13:    in r28, 0x3d
        in r29, 0x3e
        std Y+1, r24
        std Y+2, r25
        std Y+3, r22
        std Y+4, r23
L14:    ldd r30, Y+3
        ldd r31, Y+4
        ld r24, Z
        cpse r24, r1
        rjmp L15
        ldd r24, Y+1
        ldd r25, Y+2
        pop r0
        pop r0
        pop r0
        pop r0
        pop r29
        pop r28
        ret
L15:    ldd r18, Y+3
        ldd r19, Y+4
        subi r18, 255
        sbci r19, 255
        std Y+3, r18
        std Y+4, r19
        call L9
        rjmp L14
        .dw 0x921f
        .dw 0x920f
        .dw 0xb60f
        .dw 0x920f
        .dw 0x2411
        .dw 0x932f
        .dw 0x933f
        .dw 0x934f
        .dw 0x935f
        .dw 0x936f
        .dw 0x937f
        .dw 0x938f
        .dw 0x939f
        .dw 0x93af
        .dw 0x93bf
        .dw 0x93ef
        .dw 0x93ff
        .dw 0x91e0
        .dw 0x0154
        .dw 0x91f0
        .dw 0x0155
        .dw 0x9730
        .dw 0xf009
        .dw 0x9509
        .dw 0x91ff
        .dw 0x91ef
        .dw 0x91bf
        .dw 0x91af
        .dw 0x919f
        .dw 0x918f
        .dw 0x917f
        .dw 0x916f
        .dw 0x915f
        .dw 0x914f
        .dw 0x913f
        .dw 0x912f
        .dw 0x900f
        .dw 0xbe0f
        .dw 0x900f
        .dw 0x901f
        .dw 0x9518
        .dw 0x921f
        .dw 0x920f
        .dw 0xb60f
        .dw 0x920f
        .dw 0x2411
        .dw 0x932f
        .dw 0x933f
        .dw 0x934f
        .dw 0x935f
        .dw 0x936f
        .dw 0x937f
        .dw 0x938f
        .dw 0x939f
        .dw 0x93af
        .dw 0x93bf
        .dw 0x93ef
        .dw 0x93ff
        .dw 0x91e0
        .dw 0x0152
        .dw 0x91f0
        .dw 0x0153
        .dw 0x9730
        .dw 0xf009
        .dw 0x9509
        .dw 0x91ff
        .dw 0x91ef
        .dw 0x91bf
        .dw 0x91af
        .dw 0x919f
        .dw 0x918f
        .dw 0x917f
        .dw 0x916f
        .dw 0x915f
        .dw 0x914f
        .dw 0x913f
        .dw 0x912f
        .dw 0x900f
        .dw 0xbe0f
        .dw 0x900f
        .dw 0x901f
        .dw 0x9518
        .dw 0x9518
        .dw 0x9518
L16:    in r28, 0x3d
        in r29, 0x3e
        sbiw r29:r28, 21
        in r0, 0x3f
        cli
        out 0x3e, r29
        out 0x3f, r0
        out 0x3d, r28
        ldi r20, 128
        ldi r21, 37
        ldi r23, 0
        sts 0x149, r20
        sts 0x14a, r21
        sts 0x14b, r1
        sts 0x14c, r1
        ldi r24, 3
        sts 0x14d, r24
        sts 0x148, r1
        sts 0x14f, r1
        ldi r24, 1
        sts 0x151, r24
        sts 0x150, r24
        sts 0x14e, r1
        lds r24, 0x64
        andi r24, 253
        sts 0x64, r24
        lds r25, 0x148
        lds r24, 0xc2
        andi r24, 63
        sts 0xc2, r24
        lds r24, 0xc2
        or r24, r25
        sts 0xc2, r24
        cpse r25, r1
        rjmp L27
        lds r24, 0xc2
        andi r24, 254
        sts 0xc2, r24
L17:    lds r24, 0x64
        andi r24, 253
        sts 0x64, r24
        lds r14, 0x149
        lds r15, 0x14a
        lds r16, 0x14b
        lds r17, 0x14c
        sts 0xc5, r1
        sts 0xc4, r1
        lds r24, 0x61
        andi r24, 15
        ldi r20, 0
        ldi r21, 36
        ldi r22, 244
        movw r13:r12, r23:r22
        movw r11:r10, r21:r20
        rjmp L19
L18:    lsr r13
        ror r12
        ror r11
        ror r10
L19:    dec r24
        brpl L18
        std Y+18, r10
        std Y+19, r11
        std Y+20, r12
        std Y+21, r13
        lds r24, 0xc2
        sbrc r24, 6
        rjmp L29
        andi r24, 192
        brne L23
        movw r23:r22, r11:r10
        movw r25:r24, r13:r12
        movw r19:r18, r15:r14
        movw r21:r20, r17:r16
        ldi r30, 4
L20:    lsl r18
        rol r19
        rol r20
        rol r21
        dec r30
        brne L20
        call L49
        swap r18
        swap r19
        andi r19, 240
        eor r19, r18
        andi r18, 240
        eor r19, r18
        ldi r20, 0
        ldi r21, 0
        movw r23:r22, r11:r10
        movw r25:r24, r13:r12
        call L49
        ldi r26, 100
        ldi r27, 0
        call L52
        movw r19:r18, r15:r14
        movw r21:r20, r17:r16
        call L49
        subi r18, 100
        sbc r19, r1
        brpl L21
        neg r19
        neg r18
        sbc r19, r1
L21:    cpi r18, 4
        cpc r19, r1
        brlt L28
        lds r24, 0xc4
        cpse r24, r1
        rjmp L22
        lds r24, 0xc5
        cpse r24, r1
        rjmp L22
        lds r24, 0xc0
        ori r24, 2
        sts 0xc0, r24
L22:    ldi r26, 8
L23:    ldi r27, 0
        movw r19:r18, r15:r14
        movw r21:r20, r17:r16
        call L52
        movw r19:r18, r23:r22
        movw r21:r20, r25:r24
        ldd r22, Y+18
        ldd r23, Y+19
        ldd r24, Y+20
        ldd r25, Y+21
        call L49
        subi r18, 1
        sbc r19, r1
        sts 0xc5, r19
        sts 0xc4, r18
        lds r24, 0x14d
        cpi r24, 2
        brne L24
        rjmp L45
L24:    brcc L30
        cpse r24, r1
        rjmp L44
        lds r24, 0xc2
        andi r24, 249
L25:    sts 0xc2, r24
        lds r24, 0xc1
        andi r24, 251
L26:    sts 0xc1, r24
        rjmp L32
L27:    lds r24, 0xc0
        andi r24, 253
        sts 0xc0, r24
        rjmp L17
L28:    lds r24, 0xc0
        andi r24, 253
        sts 0xc0, r24
        ldi r26, 16
        rjmp L23
L29:    lds r24, 0xc0
        andi r24, 253
        sts 0xc0, r24
        ldi r26, 2
        rjmp L23
L30:    cpi r24, 3
        brne L31
        rjmp L46
L31:    cpi r24, 4
        brne L32
        rjmp L47
L32:    lds r24, 0x14f
        lds r25, 0xc2
        andi r25, 207
        sts 0xc2, r25
        lds r25, 0xc2
        or r24, r25
        sts 0xc2, r24
        lds r24, 0x14e
        cpi r24, 1
        breq L33
        rjmp L48
L33:    lds r24, 0xc2
        ori r24, 8
L34:    sts 0xc2, r24
        ldi r24, 133
        ldi r25, 0
        sts 0x152, r24
        sts 0x153, r25
        lds r24, 0xc1
        ori r24, 128
        sts 0xc1, r24
        sei
        ldi r24, 95
        ldi r25, 0
        sts 0x154, r24
        sts 0x155, r25
        lds r24, 0xc1
        ori r24, 32
        sts 0xc1, r24
        sei
        lds r24, 0x151
        cp r24, r1
        breq L35
        lds r24, 0xc1
        ori r24, 16
        sts 0xc1, r24
L35:    lds r24, 0x150
        cp r24, r1
        breq L36
        lds r24, 0xc1
        ori r24, 8
        sts 0xc1, r24
L36:    ldi r22, 1
        ldi r23, 1
        ldi r24, 0
        ldi r25, 1
        call L11
        ldi r22, 34
        ldi r23, 1
        call L11
        cbi 0x05, 5
L37:    ldi r16, 0
        ldi r17, 0
L38:    ldi r20, 10
        movw r23:r22, r29:r28
        subi r22, 255
        sbci r23, 255
        movw r25:r24, r17:r16
        call L55
        movw r23:r22, r29:r28
        subi r22, 255
        sbci r23, 255
        ldi r24, 0
        ldi r25, 1
        call L11
        ldi r22, 37
        ldi r23, 1
        call L11
L39:    sbic 0x1f, 1
        rjmp L39
        in r24, 0x3f
        cli
        out 0x22, r17
        out 0x21, r16
        sbi 0x1f, 0
        out 0x3f, r24
        in r15, 0x20
        ldi r24, 17
        ldi r30, 39
        ldi r31, 1
        movw r27:r26, r29:r28
        adiw r27:r26, 1
L40:    ld r0, Z+
        st X+, r0
        dec r24
        brne L40
        ldi r24, 48
        call L9
        ldi r24, 120
        call L9
        mov r24, r15
        swap r24
        andi r24, 15
        movw r31:r30, r29:r28
        adiw r31:r30, 1
        add r30, r24
        adc r31, r1
        ld r24, Z
        call L9
        mov r24, r15
        andi r24, 15
        movw r31:r30, r29:r28
        adiw r31:r30, 1
        add r30, r24
        adc r31, r1
        ld r24, Z
        call L9
        ldi r22, 34
        ldi r23, 1
        ldi r24, 0
        ldi r25, 1
        call L11
        ser r18
        ldi r24, 225
        ldi r25, 4
L41:    subi r18, 1
        sbci r24, 0
        sbci r25, 0
        brne L41
        rjmp L42
L42:    nop
        subi r16, 255
        sbci r17, 255
        cp r16, r1
        ldi r18, 2
        cpc r17, r18
        breq L43
        rjmp L38
L43:    rjmp L37
L44:    lds r24, 0xc2
        andi r24, 251
        sts 0xc2, r24
        lds r24, 0xc2
        ori r24, 2
        rjmp L25
L45:    lds r24, 0xc2
        ori r24, 4
        sts 0xc2, r24
        lds r24, 0xc2
        andi r24, 253
        rjmp L25
L46:    lds r24, 0xc2
        ori r24, 6
        rjmp L25
L47:    lds r24, 0xc2
        ori r24, 6
        sts 0xc2, r24
        lds r24, 0xc1
        ori r24, 4
        rjmp L26
L48:    lds r24, 0xc2
        andi r24, 247
        rjmp L34
        .dw 0x93cf
        .dw 0x93df
        .dw 0xd000
        .dw 0xd000
        .dw 0x920f
        .dw 0xb7cd
        .dw 0xb7de
        .dw 0x9180
        .dw 0x0140
        .dw 0x8389
        .dw 0x9180
        .dw 0x0141
        .dw 0x838a
        .dw 0x9180
        .dw 0x0142
        .dw 0x838b
        .dw 0x9180
        .dw 0x0143
        .dw 0x838c
        .dw 0x9180
        .dw 0x0144
        .dw 0x838d
        .dw 0x91b0
        .dw 0x0145
        .dw 0x91f0
        .dw 0x0146
        .dw 0x91e0
        .dw 0x0147
        .dw 0x8129
        .dw 0x813a
        .dw 0x814b
        .dw 0x815c
        .dw 0x2f68
        .dw 0x2f7b
        .dw 0x2f8f
        .dw 0x2f9e
        .dw 0xe0a1
        .dw 0x940e
        .dw 0x03c2
        .dw 0x9320
        .dw 0x0140
        .dw 0x9330
        .dw 0x0141
        .dw 0x9340
        .dw 0x0142
        .dw 0x9350
        .dw 0x0143
        .dw 0x9360
        .dw 0x0144
        .dw 0x9370
        .dw 0x0145
        .dw 0x9380
        .dw 0x0146
        .dw 0x9390
        .dw 0x0147
        .dw 0x8129
        .dw 0x813a
        .dw 0x814b
        .dw 0x815c
        .dw 0x816d
        .dw 0x2f7b
        .dw 0x2f8f
        .dw 0x2f9e
        .dw 0xe0a0
        .dw 0x940e
        .dw 0x03ce
        .dw 0xf441
        .dw 0x9210
        .dw 0x01db
        .dw 0x9210
        .dw 0x01da
        .dw 0x9210
        .dw 0x01dd
        .dw 0x9210
        .dw 0x01dc
        .dw 0x9180
        .dw 0x0138
        .dw 0x8389
        .dw 0x9180
        .dw 0x0139
        .dw 0x838a
        .dw 0x9180
        .dw 0x013a
        .dw 0x838b
        .dw 0x9180
        .dw 0x013b
        .dw 0x838c
        .dw 0x9180
        .dw 0x013c
        .dw 0x838d
        .dw 0x91b0
        .dw 0x013d
        .dw 0x91f0
        .dw 0x013e
        .dw 0x91e0
        .dw 0x013f
        .dw 0x8129
        .dw 0x813a
        .dw 0x814b
        .dw 0x815c
        .dw 0x2f68
        .dw 0x2f7b
        .dw 0x2f8f
        .dw 0x2f9e
        .dw 0xe0a1
        .dw 0x940e
        .dw 0x03c2
        .dw 0x9320
        .dw 0x0138
        .dw 0x9330
        .dw 0x0139
        .dw 0x9340
        .dw 0x013a
        .dw 0x9350
        .dw 0x013b
        .dw 0x9360
        .dw 0x013c
        .dw 0x9370
        .dw 0x013d
        .dw 0x9380
        .dw 0x013e
        .dw 0x9390
        .dw 0x013f
        .dw 0x8129
        .dw 0x813a
        .dw 0x814b
        .dw 0x815c
        .dw 0x816d
        .dw 0x2f7b
        .dw 0x2f8f
        .dw 0x2f9e
        .dw 0xe0a0
        .dw 0x940e
        .dw 0x03ce
        .dw 0xf441
        .dw 0x9210
        .dw 0x0197
        .dw 0x9210
        .dw 0x0196
        .dw 0x9210
        .dw 0x0199
        .dw 0x9210
        .dw 0x0198
        .dw 0x900f
        .dw 0x900f
        .dw 0x900f
        .dw 0x900f
        .dw 0x900f
        .dw 0x91df
        .dw 0x91cf
        .dw 0x9508
L49:    ldi r26, 33
        mov r1, r26
        sub r26, r26
        sub r27, r27
        movw r31:r30, r27:r26
        rjmp L51
L50:    rol r26
        rol r27
        rol r30
        rol r31
        cp r26, r18
        cpc r27, r19
        cpc r30, r20
        cpc r31, r21
        brcs L51
        sub r26, r18
        sbc r27, r19
        sbc r30, r20
        sbc r31, r21
L51:    rol r22
        rol r23
        rol r24
        rol r25
        dec r1
        brne L50
        com r22
        com r23
        com r24
        com r25
        movw r19:r18, r23:r22
        movw r21:r20, r25:r24
        movw r23:r22, r27:r26
        movw r25:r24, r31:r30
        ret
L52:    call L53
        mul r26, r21
        add r25, r0
        mul r27, r20
        add r25, r0
        mul r26, r20
        add r24, r0
        adc r25, r1
        clr r1
        ret
        .dw 0x2400
        .dw 0xfda7
        .dw 0x9400
        .dw 0x0f2a
        .dw 0x1d30
        .dw 0x1d40
        .dw 0x1d50
        .dw 0x1d60
        .dw 0x1d70
        .dw 0x1d80
        .dw 0x1d90
        .dw 0x9508
        .dw 0x2400
        .dw 0xfda7
        .dw 0x9400
        .dw 0x172a
        .dw 0x0530
        .dw 0x0540
        .dw 0x0550
        .dw 0x0560
        .dw 0x0570
        .dw 0x0580
        .dw 0x0590
        .dw 0x9508
L53:    mul r26, r18
        movw r23:r22, r1:r0
        mul r27, r19
        movw r25:r24, r1:r0
        mul r26, r19
        add r23, r0
        adc r24, r1
        clr r1
        adc r25, r1
        mul r27, r18
        add r23, r0
        adc r24, r1
        clr r1
        adc r25, r1
        ret
L54:    lsl r30
        rol r31
        lpm r0, Z+
        lpm r31, Z
        mov r30, r0
        ijmp
L55:    clr r27
        cpi r20, 10
        brne L56
        tst r25
        brpl L56
        ldi r27, 45
        com r25
        neg r24
        sbci r25, 255
L56:    jmp L57
        .dw 0x27bb
L57:    movw r31:r30, r23:r22
        clr r21
L58:    clr r26
L59:    lsl r24
        rol r25
        rol r26
        cp r26, r20
        brcs L60
        sub r26, r20
        inc r24
L60:    subi r21, 16
        brne L59
        subi r26, 208
        cpi r26, 58
        brcs L61
        subi r26, 217
L61:    st Z+, r26
        sbiw r25:r24, 0
        brne L58
        cpse r27, r1
        st Z+, r27
        st Z+, r1
        movw r25:r24, r23:r22
        jmp L62
L62:    movw r27:r26, r25:r24
        movw r31:r30, r25:r24
L63:    mov r22, r23
        ld r23, Z+
        tst r23
        brne L63
        sbiw r31:r30, 2
        rjmp L65
L64:    ld r23, X
        st X+, r22
        st Z, r23
        ld r22, -Z
L65:    cp r26, r30
        cpc r27, r31
        brcs L64
        ret
L66:    cli
L67:    rjmp L67
        .dw 0x5302
        .dw 0x7265
        .dw 0x6169
        .dw 0x206c
        .dw 0x6e69
        .dw 0x7469
        .dw 0x6169
        .dw 0x696c
        .dw 0x657a
        .dw 0x2064
        .dw 0x7573
        .dw 0x6363
        .dw 0x7365
        .dw 0x6673
        .dw 0x6c75
        .dw 0x796c
        .dw 0x002e
        .dw 0x0a0d
        .dw 0x2000
        .dw 0x3000
        .dw 0x3231
        .dw 0x3433
        .dw 0x3635
        .dw 0x3837
        .dw 0x4139
        .dw 0x4342
        .dw 0x4544
        .dw 0x0046
