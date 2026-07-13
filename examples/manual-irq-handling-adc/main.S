        .org 0x0000
L0:     jmp L1
        jmp L4
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0057
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
        .dw 0x940c
        .dw 0x0046
L1:     clr r1
        out 0x3f, r1
        ser r28
        ldi r29, 8
        out 0x3e, r29
        out 0x3d, r28
        ldi r18, 1
        ldi r26, 0
        ldi r27, 1
        rjmp L3
L2:     st X+, r1
L3:     cpi r26, 2
        cpc r27, r18
        brne L2
        call L5
        jmp L18
L4:     jmp L0
        .dw 0xe08b
        .dw 0xe290
        .dw 0xb993
        .dw 0xef2f
        .dw 0xee31
        .dw 0xe044
        .dw 0x5021
        .dw 0x4030
        .dw 0x4040
        .dw 0xf7e1
        .dw 0xc000
        .dw 0x0000
        .dw 0x5081
        .dw 0xf7a1
        .dw 0x9508
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
        .dw 0x0100
        .dw 0x91f0
        .dw 0x0101
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
L5:     sei
        cbi 0x05, 5
        sbi 0x04, 5
        cbi 0x05, 5
        lds r24, 0x7a
        ldi r25, 0
        sbrs r24, 5
        rjmp L6
        lds r24, 0x7a
        andi r24, 223
        sts 0x7a, r24
        ldi r25, 1
L6:     lds r24, 0x7c
        andi r24, 240
        sts 0x7c, r24
        lds r24, 0x7c
        ori r24, 8
        sts 0x7c, r24
        lds r24, 0x7a
        cp r25, r1
        brne L7
        rjmp L16
L7:     ori r24, 32
L8:     sts 0x7a, r24
        lds r24, 0x7a
        ldi r25, 0
        sbrs r24, 5
        rjmp L9
        lds r24, 0x7a
        andi r24, 223
        sts 0x7a, r24
        ldi r25, 1
L9:     lds r24, 0x7c
        andi r24, 63
        sts 0x7c, r24
        lds r24, 0x7c
        ori r24, 192
        sts 0x7c, r24
        lds r24, 0x7a
        cp r25, r1
        brne L10
        rjmp L17
L10:    ori r24, 32
L11:    sts 0x7a, r24
        lds r24, 0x7a
        andi r24, 248
        sts 0x7a, r24
        lds r24, 0x7a
        ori r24, 7
        sts 0x7a, r24
        lds r24, 0x7c
        andi r24, 223
        sts 0x7c, r24
        lds r24, 0x64
        andi r24, 254
        sts 0x64, r24
        lds r24, 0x7a
        ori r24, 128
        sts 0x7a, r24
        ldi r24, 72
        ldi r25, 0
        sts 0x100, r24
        sts 0x101, r25
        lds r24, 0x7a
        ori r24, 8
        sts 0x7a, r24
L12:    lds r24, 0x7a
        ori r24, 64
        sts 0x7a, r24
L13:    lds r24, 0x7a
        sbrc r24, 6
        rjmp L13
        in r24, 0x3f
        cli
        lds r25, 0x78
        lds r25, 0x79
        out 0x3f, r24
        ser r18
        ldi r24, 167
        ldi r25, 97
L14:    subi r18, 1
        sbci r24, 0
        sbci r25, 0
        brne L14
        rjmp L15
L15:    nop
        rjmp L12
L16:    andi r24, 223
        rjmp L8
L17:    andi r24, 223
        rjmp L11
L18:    cli
L19:    rjmp L19
