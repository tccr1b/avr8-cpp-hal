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
        .dw 0x0067
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
        .dw 0x00b9
        .dw 0x940c
        .dw 0x0090
        .dw 0x940c
        .dw 0x00e2
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
        .dw 0x0301
L1:     clr r1
        out 0x3f, r1
        ser r28
        ldi r29, 8
        out 0x3e, r29
        out 0x3d, r28
        ldi r17, 1
        ldi r26, 0
        ldi r27, 1
        ldi r30, 210
        ldi r31, 7
        rjmp L3
L2:     lpm r0, Z+
        st X+, r0
L3:     cpi r26, 2
        cpc r27, r17
        brne L2
        ldi r18, 1
        ldi r26, 2
        ldi r27, 1
        rjmp L5
L4:     st X+, r1
L5:     cpi r26, 160
        cpc r27, r18
        brne L4
        ldi r17, 0
        ldi r28, 53
        ldi r29, 0
        rjmp L7
L6:     sbiw r29:r28, 1
        movw r31:r30, r29:r28
        call L36
L7:     cpi r28, 52
        cpc r29, r17
        brne L6
        call L18
        jmp L37
L8:     jmp L0
L9:     in r24, 0x33
        ori r24, 1
        out 0x33, r24
        sleep
        ret
L10:    cbi 0x05, 5
        cbi 0x04, 5
        ret
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
        .dw 0x019e
        .dw 0x91f0
        .dw 0x019f
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
        .dw 0x019a
        .dw 0x91f0
        .dw 0x019b
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
        .dw 0x019c
        .dw 0x91f0
        .dw 0x019d
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
        .dw 0x940e
        .dw 0x016d
        .dw 0xef2f
        .dw 0xe780
        .dw 0xe092
        .dw 0x5021
        .dw 0x4080
        .dw 0x4090
        .dw 0xf7e1
        .dw 0xc000
        .dw 0x0000
        .dw 0x940e
        .dw 0x016d
        .dw 0xef2f
        .dw 0xe780
        .dw 0xe092
        .dw 0x5021
        .dw 0x4080
        .dw 0x4090
        .dw 0xf7e1
        .dw 0xc000
        .dw 0x0000
        .dw 0x940e
        .dw 0x016d
        .dw 0xef2f
        .dw 0xe780
        .dw 0xe092
        .dw 0x5021
        .dw 0x4080
        .dw 0x4090
        .dw 0xf7e1
        .dw 0xc000
        .dw 0x0000
        .dw 0x940e
        .dw 0x016d
        .dw 0x9a2d
        .dw 0x9508
        .dw 0x93cf
        .dw 0x93df
        .dw 0x920f
        .dw 0xb7cd
        .dw 0xb7de
        .dw 0x9180
        .dw 0x0152
        .dw 0x9190
        .dw 0x0153
        .dw 0x9120
        .dw 0x0154
        .dw 0x9130
        .dw 0x0155
        .dw 0x9583
        .dw 0x738f
        .dw 0xe090
        .dw 0x1782
        .dw 0x0793
        .dw 0xf0f9
        .dw 0x9120
        .dw 0x00c6
        .dw 0x9180
        .dw 0x0152
        .dw 0x9190
        .dw 0x0153
        .dw 0x9583
        .dw 0x738f
        .dw 0xe090
        .dw 0x9140
        .dw 0x0154
        .dw 0x9150
        .dw 0x0155
        .dw 0x1784
        .dw 0x0795
        .dw 0xf059
        .dw 0x91e0
        .dw 0x0152
        .dw 0x91f0
        .dw 0x0153
        .dw 0x5eee
        .dw 0x4ffe
        .dw 0x8320
        .dw 0x9390
        .dw 0x0153
        .dw 0x9380
        .dw 0x0152
        .dw 0x900f
        .dw 0x91df
        .dw 0x91cf
        .dw 0x9508
        .dw 0x9180
        .dw 0x00c6
        .dw 0x8389
        .dw 0x8189
        .dw 0xcff7
        .dw 0x9120
        .dw 0x0196
        .dw 0x9130
        .dw 0x0197
        .dw 0x9180
        .dw 0x0198
        .dw 0x9190
        .dw 0x0199
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
        .dw 0x0198
        .dw 0x91f0
        .dw 0x0199
        .dw 0x5aea
        .dw 0x4ffe
        .dw 0x8120
        .dw 0x9180
        .dw 0x0198
        .dw 0x9190
        .dw 0x0199
        .dw 0x9583
        .dw 0x738f
        .dw 0xe090
        .dw 0x9390
        .dw 0x0199
        .dw 0x9380
        .dw 0x0198
        .dw 0x9320
        .dw 0x00c6
        .dw 0x9508
L11:    ldi r24, 9
        ldi r30, 0
        ldi r31, 0
        sts 0x57, r24
        lpm r24, Z
        andi r24, 15
        ret
        .dw 0xb185
        .dw 0xe290
        .dw 0x2789
        .dw 0xb985
        .dw 0x9508
L12:    sbic 0x04, 5
        rjmp L13
        clr r24
        sbis 0x05, 5
        inc r24
        lsl r24
        ret
L13:    ldi r24, 1
        ret
L14:    call L12
        cpse r24, r1
        rjmp L15
        in r24, 0x35
        andi r24, 239
        out 0x35, r24
        sbi 0x05, 5
        ldi r24, 1
        ret
L15:    ldi r24, 0
        ret
L16:    call L12
        cpse r24, r1
        rjmp L17
        cbi 0x05, 5
        ldi r24, 1
        ret
L17:    ldi r24, 0
        ret
L18:    rcall L19
L19:    rcall L20
L20:    in r28, 0x3d
        in r29, 0x3e
        cbi 0x05, 5
        sbi 0x04, 5
        cbi 0x05, 5
        sbi 0x05, 5
        cbi 0x04, 5
        cbi 0x05, 5
        call L10
        call L14
        in r0, 0x35
        sbrc r0, 4
        rjmp L21
        sbis 0x04, 5
        in r24, 0x05
L21:    call L12
        call L16
        call L14
        cp r24, r1
        breq L22
        call L10
        call L14
L22:    call L16
        cp r24, r1
        breq L23
        call L10
        call L16
L23:    in r24, 0x04
        call L12
        call L10
        call L14
        in r24, 0x03
        lds r24, 0x61
        in r24, 0x33
        andi r24, 243
        out 0x33, r24
        call L9
        in r24, 0x3f
        cli
        ldi r25, 128
        sts 0x61, r25
        ldi r25, 5
        sts 0x61, r25
        out 0x3f, r24
        lds r24, 0x61
        lds r24, 0x61
        lds r24, 0x64
        ori r24, 128
        sts 0x64, r24
        lds r24, 0x64
        ori r24, 1
        sts 0x64, r24
        lds r24, 0xc2
        andi r24, 63
        sts 0xc2, r24
        lds r24, 0xc4
        cpse r24, r1
        rjmp L24
        lds r24, 0xc5
        cpse r24, r1
        rjmp L24
        lds r24, 0xc0
        ori r24, 2
        sts 0xc0, r24
L24:    lds r24, 0xc2
        ldi r18, 2
        sbrc r24, 6
        rjmp L26
        andi r24, 192
        brne L25
        lds r24, 0xc0
        ldi r18, 8
        sbrs r24, 1
L25:    ldi r18, 16
L26:    lds r19, 0x61
        andi r19, 15
        ldi r24, 0
        ldi r25, 36
        ldi r26, 244
        ldi r27, 0
        movw r23:r22, r27:r26
        movw r21:r20, r25:r24
        rjmp L28
L27:    lsr r23
        ror r22
        ror r21
        ror r20
L28:    dec r19
        brpl L27
        std Y+1, r20
        std Y+2, r21
        std Y+3, r22
        std Y+4, r23
        ldi r19, 0
        ldi r26, 128
        ldi r27, 37
        call L32
        movw r19:r18, r23:r22
        movw r21:r20, r25:r24
        ldd r22, Y+1
        ldd r23, Y+2
        ldd r24, Y+3
        ldd r25, Y+4
        call L33
        subi r18, 1
        sbc r19, r1
        sts 0xc5, r19
        sts 0xc4, r18
        lds r24, 0xc2
        ori r24, 6
        sts 0xc2, r24
        lds r24, 0xc1
        andi r24, 251
        sts 0xc1, r24
        lds r24, 0xc2
        andi r24, 207
        sts 0xc2, r24
        lds r24, 0xc2
        andi r24, 247
        sts 0xc2, r24
        ldi r24, 8
        ldi r25, 1
        sts 0x19c, r24
        sts 0x19d, r25
        lds r24, 0xc1
        ori r24, 128
        sts 0xc1, r24
        sei
        ldi r24, 63
        ldi r25, 1
        sts 0x19a, r24
        sts 0x19b, r25
        lds r24, 0xc1
        ori r24, 32
        sts 0xc1, r24
        sei
        lds r24, 0xc1
        ori r24, 16
        sts 0xc1, r24
        lds r24, 0xc1
        ori r24, 8
        sts 0xc1, r24
        lds r24, 0xc1
        andi r24, 31
        sts 0xc1, r24
        lds r24, 0xc1
        andi r24, 239
        sts 0xc1, r24
        lds r24, 0xc1
        andi r24, 247
        sts 0xc1, r24
        lds r24, 0xc0
        andi r24, 253
        sts 0xc0, r24
        sts 0xc5, r1
        sts 0xc4, r1
        in r24, 0x04
        ori r24, 44
        out 0x04, r24
        cbi 0x04, 4
        in r24, 0x05
        ori r24, 20
        out 0x05, r24
        in r24, 0x2c
        ori r24, 16
        out 0x2c, r24
        in r24, 0x2d
        ori r24, 1
        out 0x2d, r24
        in r24, 0x2c
        andi r24, 252
        out 0x2c, r24
        in r24, 0x2c
        ori r24, 64
        out 0x2c, r24
        in r24, 0x2c
        andi r24, 251
        out 0x2c, r24
        in r24, 0x2c
        andi r24, 247
        out 0x2c, r24
        ldi r24, 8
        out 0x2e, r24
L29:    in r0, 0x2d
        sbrs r0, 7
        rjmp L29
        in r24, 0x2e
        lds r24, 0x7a
        ori r24, 128
        sts 0x7a, r24
        lds r24, 0xc1
        andi r24, 191
        sts 0xc1, r24
        lds r24, 0x64
        ori r24, 1
        sts 0x64, r24
        call L9
        lds r24, 0x64
        andi r24, 254
        sts 0x64, r24
        ldi r24, 9
        ldi r30, 0
        ldi r31, 0
        sts 0x57, r24
        lpm r24, Z
        sbrc r24, 7
        rjmp L30
        call L11
        call L11
L30:    lds r24, 0x7a
        ori r24, 128
        sts 0x7a, r24
        lds r24, 0x61
        in r24, 0x3f
        cli
        ldi r25, 128
        sts 0x61, r25
        ldi r25, 4
        sts 0x61, r25
        out 0x3f, r24
        in r24, 0x33
        andi r24, 241
        out 0x33, r24
        call L9
        lds r24, 0xbc
        ori r24, 4
        sts 0xbc, r24
        lds r24, 0xba
        andi r24, 254
        sts 0xba, r24
        lds r24, 0xbc
        ori r24, 4
        sts 0xbc, r24
        ldi r24, 9
        ldi r30, 0
        ldi r31, 0
        sts 0x57, r24
        lpm r24, Z
        call L11
        ldi r24, 227
        ldi r25, 0
        sts 0x19e, r24
        sts 0x19f, r25
        ldi r24, 7
        sts 0x101, r24
        ldi r24, 64
        sts 0x100, r24
        in r24, 0x3f
        cli
        wdr
        ldi r25, 24
        sts 0x60, r25
        ldi r18, 15
        sts 0x60, r18
        out 0x3f, r24
        lds r24, 0x60
        in r18, 0x3f
        cli
        wdr
        sts 0x60, r25
        andi r24, 39
        ori r24, 64
        sts 0x60, r24
        out 0x3f, r18
        sei
L31:    rjmp L31
        .dw 0x93cf
        .dw 0x93df
        .dw 0xd000
        .dw 0xd000
        .dw 0x920f
        .dw 0xb7cd
        .dw 0xb7de
        .dw 0x9180
        .dw 0x010a
        .dw 0x8389
        .dw 0x9180
        .dw 0x010b
        .dw 0x838a
        .dw 0x9180
        .dw 0x010c
        .dw 0x838b
        .dw 0x9180
        .dw 0x010d
        .dw 0x838c
        .dw 0x9180
        .dw 0x010e
        .dw 0x838d
        .dw 0x91b0
        .dw 0x010f
        .dw 0x91f0
        .dw 0x0110
        .dw 0x91e0
        .dw 0x0111
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
        .dw 0x03c9
        .dw 0x9320
        .dw 0x010a
        .dw 0x9330
        .dw 0x010b
        .dw 0x9340
        .dw 0x010c
        .dw 0x9350
        .dw 0x010d
        .dw 0x9360
        .dw 0x010e
        .dw 0x9370
        .dw 0x010f
        .dw 0x9380
        .dw 0x0110
        .dw 0x9390
        .dw 0x0111
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
        .dw 0x03d5
        .dw 0xf441
        .dw 0x9210
        .dw 0x0197
        .dw 0x9210
        .dw 0x0196
        .dw 0x9210
        .dw 0x0199
        .dw 0x9210
        .dw 0x0198
        .dw 0x9180
        .dw 0x0102
        .dw 0x8389
        .dw 0x9180
        .dw 0x0103
        .dw 0x838a
        .dw 0x9180
        .dw 0x0104
        .dw 0x838b
        .dw 0x9180
        .dw 0x0105
        .dw 0x838c
        .dw 0x9180
        .dw 0x0106
        .dw 0x838d
        .dw 0x91b0
        .dw 0x0107
        .dw 0x91f0
        .dw 0x0108
        .dw 0x91e0
        .dw 0x0109
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
        .dw 0x03c9
        .dw 0x9320
        .dw 0x0102
        .dw 0x9330
        .dw 0x0103
        .dw 0x9340
        .dw 0x0104
        .dw 0x9350
        .dw 0x0105
        .dw 0x9360
        .dw 0x0106
        .dw 0x9370
        .dw 0x0107
        .dw 0x9380
        .dw 0x0108
        .dw 0x9390
        .dw 0x0109
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
        .dw 0x03d5
        .dw 0xf441
        .dw 0x9210
        .dw 0x0153
        .dw 0x9210
        .dw 0x0152
        .dw 0x9210
        .dw 0x0155
        .dw 0x9210
        .dw 0x0154
        .dw 0x900f
        .dw 0x900f
        .dw 0x900f
        .dw 0x900f
        .dw 0x900f
        .dw 0x91df
        .dw 0x91cf
        .dw 0x9508
L32:    mul r26, r18
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
L33:    ldi r26, 33
        mov r1, r26
        sub r26, r26
        sub r27, r27
        movw r31:r30, r27:r26
        rjmp L35
L34:    rol r26
        rol r27
        rol r30
        rol r31
        cp r26, r18
        cpc r27, r19
        cpc r30, r20
        cpc r31, r21
        brcs L35
        sub r26, r18
        sbc r27, r19
        sbc r30, r20
        sbc r31, r21
L35:    rol r22
        rol r23
        rol r24
        rol r25
        dec r1
        brne L34
        com r22
        com r23
        com r24
        com r25
        movw r19:r18, r23:r22
        movw r21:r20, r25:r24
        movw r23:r22, r27:r26
        movw r25:r24, r31:r30
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
L36:    lsl r30
        rol r31
        lpm r0, Z+
        lpm r31, Z
        mov r30, r0
        ijmp
L37:    cli
L38:    rjmp L38
        .dw 0x2108
