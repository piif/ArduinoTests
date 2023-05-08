	.file	"kitchenTimer.ino.cpp"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.text._Z12prepareDigithh,"ax",@progbits
.global	_Z12prepareDigithh
	.type	_Z12prepareDigithh, @function
_Z12prepareDigithh:
.LFB113:
	.file 1 "/home/pif/GitHub/piif/ArduinoTests/kitchenTimer/kitchenTimer.ino"
	.loc 1 146 0
	.cfi_startproc
.LVL0:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 147 0
	in r25,0xb
	andi r25,lo8(3)
.LVL1:
	.loc 1 149 0
	sbrc r22,7
	rjmp .L2
	.loc 1 147 0
	ori r25,lo8(-60)
.LVL2:
.L3:
	.loc 1 150 0
	sbrc r22,6
	.loc 1 150 0 is_stmt 0 discriminator 1
	andi r25,lo8(127)
.LVL3:
.L4:
	.loc 1 151 0 is_stmt 1
	ldi r18,lo8(27)
	sbrs r22,5
	.loc 1 147 0
	ldi r18,lo8(31)
.L5:
.LVL4:
	.loc 1 152 0
	sbrc r22,4
	.loc 1 152 0 is_stmt 0 discriminator 1
	andi r18,lo8(-9)
.LVL5:
.L6:
	.loc 1 153 0 is_stmt 1
	sbrc r22,3
	.loc 1 153 0 is_stmt 0 discriminator 1
	andi r18,lo8(-17)
.LVL6:
.L7:
	.loc 1 154 0 is_stmt 1
	sbrc r22,2
	.loc 1 154 0 is_stmt 0 discriminator 1
	andi r18,lo8(-2)
.LVL7:
.L8:
	.loc 1 155 0 is_stmt 1
	sbrc r22,1
	.loc 1 155 0 is_stmt 0 discriminator 1
	andi r18,lo8(-3)
.LVL8:
.L9:
	.loc 1 156 0 is_stmt 1
	sbrc r22,0
	.loc 1 156 0 is_stmt 0 discriminator 1
	andi r25,lo8(-5)
.LVL9:
.L10:
	.loc 1 158 0 is_stmt 1
	mov r20,r24
	ldi r21,0
	cpi r24,lo8(1)
	breq .L12
.LVL10:
	brlo .L13
	cpi r24,lo8(2)
	breq .L14
.L11:
	.loc 1 166 0
	movw r30,r20
	subi r30,lo8(-(displayPortB))
	sbci r31,hi8(-(displayPortB))
	st Z,r18
	.loc 1 167 0
	subi r20,lo8(-(displayPortD))
	sbci r21,hi8(-(displayPortD))
.LVL11:
	movw r30,r20
	st Z,r25
/* epilogue start */
	.loc 1 168 0
	ret
.LVL12:
.L2:
	.loc 1 149 0 discriminator 1
	ori r25,lo8(-124)
.LVL13:
	rjmp .L3
.LVL14:
.L13:
	.loc 1 159 0
	ori r25,lo8(32)
.LVL15:
	.loc 1 160 0
	rjmp .L11
.L12:
	.loc 1 161 0
	ori r25,lo8(16)
.LVL16:
	.loc 1 162 0
	rjmp .L11
.L14:
	.loc 1 163 0
	ori r25,lo8(8)
.LVL17:
	.loc 1 164 0
	rjmp .L11
	.cfi_endproc
.LFE113:
	.size	_Z12prepareDigithh, .-_Z12prepareDigithh
	.section	.text._Z10setDisplayi,"ax",@progbits
.global	_Z10setDisplayi
	.type	_Z10setDisplayi, @function
_Z10setDisplayi:
.LFB112:
	.loc 1 119 0
	.cfi_startproc
.LVL18:
	push r28
.LCFI0:
	.cfi_def_cfa_offset 3
	.cfi_offset 28, -2
	push r29
.LCFI1:
	.cfi_def_cfa_offset 4
	.cfi_offset 29, -3
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r30,r24
	.loc 1 120 0
	sts display+1,r25
	sts display,r24
.LVL19:
	.loc 1 127 0
	ldi r28,0
	.loc 1 126 0
	ldi r29,0
	.loc 1 125 0
	ldi r18,0
	.loc 1 124 0
	cpi r30,-1
	cpc r31,r30
.LVL20:
	breq .L35
	.loc 1 129 0
	lds r18,mapSegments+10
.LVL21:
	.loc 1 131 0
	mov r28,r18
	.loc 1 130 0
	mov r29,r18
	.loc 1 128 0
	cpi r30,-2
	ldi r24,-1
	cpc r31,r24
	breq .L35
	.loc 1 135 0
	ldi r28,lo8(-1)
	.loc 1 134 0
	ldi r29,lo8(-1)
	.loc 1 133 0
	ldi r18,lo8(-1)
.LVL22:
	.loc 1 132 0
	cpi r30,-3
	ldi r24,-1
	cpc r31,r24
	breq .L35
	.loc 1 137 0
	movw r24,r30
	ldi r22,lo8(100)
	ldi r23,0
	call __divmodhi4
	movw r24,r22
	ldi r28,lo8(10)
	ldi r29,0
	movw r22,r28
	call __divmodhi4
	movw r26,r24
	subi r26,lo8(-(mapSegments))
	sbci r27,hi8(-(mapSegments))
	ld r18,X
	ori r18,lo8(1)
.LVL23:
	.loc 1 138 0
	movw r24,r30
	movw r22,r28
	call __divmodhi4
	movw r30,r24
.LVL24:
	movw r24,r22
	movw r22,r28
	call __divmodhi4
	movw r26,r24
	subi r26,lo8(-(mapSegments))
	sbci r27,hi8(-(mapSegments))
	ld r29,X
.LVL25:
	.loc 1 139 0
	subi r30,lo8(-(mapSegments))
	sbci r31,hi8(-(mapSegments))
	ld r28,Z
.LVL26:
.L35:
	.loc 1 141 0
	mov r22,r18
	ldi r24,0
	call _Z12prepareDigithh
.LVL27:
	.loc 1 142 0
	mov r22,r29
	ldi r24,lo8(1)
	call _Z12prepareDigithh
.LVL28:
	.loc 1 143 0
	mov r22,r28
	ldi r24,lo8(2)
/* epilogue start */
	.loc 1 144 0
	pop r29
.LVL29:
	pop r28
.LVL30:
	.loc 1 143 0
	jmp _Z12prepareDigithh
.LVL31:
	.cfi_endproc
.LFE112:
	.size	_Z10setDisplayi, .-_Z10setDisplayi
	.section	.text._Z11outputDigith,"ax",@progbits
.global	_Z11outputDigith
	.type	_Z11outputDigith, @function
_Z11outputDigith:
.LFB114:
	.loc 1 170 0
	.cfi_startproc
.LVL32:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 171 0
	in r18,0x5
	ldi r25,0
	andi r18,lo8(-32)
	movw r30,r24
	subi r30,lo8(-(displayPortB))
	sbci r31,hi8(-(displayPortB))
	ld r19,Z
	or r18,r19
	out 0x5,r18
	.loc 1 172 0
	in r18,0xb
	andi r18,lo8(3)
	subi r24,lo8(-(displayPortD))
	sbci r25,hi8(-(displayPortD))
.LVL33:
	movw r30,r24
	ld r24,Z
	or r24,r18
	out 0xb,r24
/* epilogue start */
	.loc 1 173 0
	ret
	.cfi_endproc
.LFE114:
	.size	_Z11outputDigith, .-_Z11outputDigith
	.section	.text._Z13updateDisplayv,"ax",@progbits
.global	_Z13updateDisplayv
	.type	_Z13updateDisplayv, @function
_Z13updateDisplayv:
.LFB115:
	.loc 1 175 0
	.cfi_startproc
	push r28
.LCFI2:
	.cfi_def_cfa_offset 3
	.cfi_offset 28, -2
/* prologue: function */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
	.loc 1 176 0
	lds r24,status
	lds r25,status+1
	or r24,r25
	breq .L41
	.loc 1 176 0 is_stmt 0 discriminator 1
	lds r24,display
	lds r25,display+1
	adiw r24,1
	brne .L42
.L41:
	.loc 1 178 0 is_stmt 1
	in r24,0xb
	andi r24,lo8(-57)
	out 0xb,r24
.L40:
/* epilogue start */
	.loc 1 185 0
	pop r28
	ret
.L42:
	.loc 1 183 0
	lds r28,_ZZ13updateDisplayvE5digit
	mov r24,r28
	call _Z11outputDigith
.LVL34:
	.loc 1 184 0
	mov r24,r28
	ldi r25,0
	adiw r24,1
	ldi r22,lo8(3)
	ldi r23,0
	call __divmodhi4
	sts _ZZ13updateDisplayvE5digit,r24
	rjmp .L40
	.cfi_endproc
.LFE115:
	.size	_Z13updateDisplayv, .-_Z13updateDisplayv
	.section	.text._Z10updateRingm,"ax",@progbits
.global	_Z10updateRingm
	.type	_Z10updateRingm, @function
_Z10updateRingm:
.LFB116:
	.loc 1 187 0
	.cfi_startproc
.LVL35:
	push r12
.LCFI3:
	.cfi_def_cfa_offset 3
	.cfi_offset 12, -2
	push r13
.LCFI4:
	.cfi_def_cfa_offset 4
	.cfi_offset 13, -3
	push r14
.LCFI5:
	.cfi_def_cfa_offset 5
	.cfi_offset 14, -4
	push r15
.LCFI6:
	.cfi_def_cfa_offset 6
	.cfi_offset 15, -5
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	movw r12,r22
	movw r14,r24
	.loc 1 188 0
	lds r24,ring
	cpse r24,__zero_reg__
	rjmp .L48
.LVL36:
.LBB4:
.LBB5:
	.loc 1 189 0
	ldi r24,lo8(13)
	call _Z6noToneh
.LVL37:
	.loc 1 190 0
	ldi r24,lo8(3)
	ldi r25,0
	sts status+1,r25
	sts status,r24
	.loc 1 191 0
	ldi r24,-60
	add r12,r24
	ldi r24,9
	adc r13,r24
	adc r14,__zero_reg__
	adc r15,__zero_reg__
.LVL38:
	sts timerNext,r12
	sts timerNext+1,r13
	sts timerNext+2,r14
	sts timerNext+3,r15
	.loc 1 192 0
	ldi r24,lo8(-2)
	ldi r25,lo8(-1)
/* epilogue start */
.LBE5:
.LBE4:
	.loc 1 204 0
	pop r15
	pop r14
	pop r13
	pop r12
.LVL39:
.LBB7:
.LBB6:
	.loc 1 192 0
	jmp _Z10setDisplayi
.LVL40:
.L48:
.LBE6:
.LBE7:
	.loc 1 194 0
	sbrs r24,0
	rjmp .L49
	.loc 1 195 0
	ldi r18,0
	ldi r19,0
	movw r20,r18
	ldi r22,lo8(-24)
	ldi r23,lo8(3)
.LVL41:
	ldi r24,lo8(13)
	call _Z4tonehjm
.LVL42:
	.loc 1 196 0
	ldi r24,lo8(-3)
	ldi r25,lo8(-1)
.L51:
	.loc 1 199 0
	call _Z10setDisplayi
.LVL43:
	.loc 1 201 0
	ldi r24,-12
	add r12,r24
	ldi r24,1
	adc r13,r24
	adc r14,__zero_reg__
	adc r15,__zero_reg__
	sts timerNext,r12
	sts timerNext+1,r13
	sts timerNext+2,r14
	sts timerNext+3,r15
	.loc 1 202 0
	lds r24,ring
	subi r24,lo8(-(-1))
	sts ring,r24
/* epilogue start */
	.loc 1 204 0
	pop r15
	pop r14
	pop r13
	pop r12
	ret
.LVL44:
.L49:
	.loc 1 198 0
	ldi r24,lo8(13)
	call _Z6noToneh
.LVL45:
	.loc 1 199 0
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
	rjmp .L51
	.cfi_endproc
.LFE116:
	.size	_Z10updateRingm, .-_Z10updateRingm
	.section	.text._Z13updateButtonsm,"ax",@progbits
.global	_Z13updateButtonsm
	.type	_Z13updateButtonsm, @function
_Z13updateButtonsm:
.LFB117:
	.loc 1 206 0
	.cfi_startproc
.LVL46:
	push r12
.LCFI7:
	.cfi_def_cfa_offset 3
	.cfi_offset 12, -2
	push r13
.LCFI8:
	.cfi_def_cfa_offset 4
	.cfi_offset 13, -3
	push r14
.LCFI9:
	.cfi_def_cfa_offset 5
	.cfi_offset 14, -4
	push r15
.LCFI10:
	.cfi_def_cfa_offset 6
	.cfi_offset 15, -5
	push r17
.LCFI11:
	.cfi_def_cfa_offset 7
	.cfi_offset 17, -6
	push r28
.LCFI12:
	.cfi_def_cfa_offset 8
	.cfi_offset 28, -7
	push r29
.LCFI13:
	.cfi_def_cfa_offset 9
	.cfi_offset 29, -8
/* prologue: function */
/* frame size = 0 */
/* stack size = 7 */
.L__stack_usage = 7
	movw r12,r22
	movw r14,r24
.LVL47:
.LBB8:
	.loc 1 208 0
	lds r24,buttonLastChanged
	lds r25,buttonLastChanged+1
	lds r26,buttonLastChanged+2
	lds r27,buttonLastChanged+3
	sbiw r24,0
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	breq .L53
	.loc 1 208 0 is_stmt 0 discriminator 1
	subi r24,-100
	sbci r25,-1
	sbci r26,-1
	sbci r27,-1
	cp r24,r12
	cpc r25,r13
	cpc r26,r14
	cpc r27,r15
	brlo .+2
	rjmp .L52
.L53:
.LBB9:
	.loc 1 209 0 is_stmt 1
	sts buttonLastChanged,r12
	sts buttonLastChanged+1,r13
	sts buttonLastChanged+2,r14
	sts buttonLastChanged+3,r15
	.loc 1 210 0
	ldi r24,lo8(15)
	call digitalRead
.LVL48:
	movw r28,r24
	ldi r17,lo8(1)
	or r24,r25
	breq .L55
	ldi r17,0
.L55:
.LVL49:
	.loc 1 211 0
	ldi r24,lo8(14)
	call digitalRead
.LVL50:
	movw r18,r24
	ldi r25,lo8(1)
	cp r18,__zero_reg__
	cpc r19,__zero_reg__
	breq .L56
	ldi r25,0
.L56:
.LVL51:
	.loc 1 212 0
	lds r24,BUp
	cpse r24,r25
	rjmp .L57
.L59:
.LBE9:
.LBE8:
	.loc 1 207 0
	ldi r24,0
.LVL52:
.L58:
.LBB12:
.LBB10:
	.loc 1 231 0
	lds r25,BDown
	cp r25,r17
	brne .+2
	rjmp .L63
	.loc 1 232 0
	sts BDown,r17
	.loc 1 233 0
	or r28,r29
	breq .+2
	rjmp .L63
.LVL53:
	.loc 1 236 0
	lds r24,status
	lds r25,status+1
	cpi r24,1
	cpc r25,__zero_reg__
	breq .+2
	rjmp .L64
	.loc 1 237 0
	lds r24,display
	lds r25,display+1
	cpi r24,6
	cpc r25,__zero_reg__
	brge .+2
	rjmp .L65
	.loc 1 238 0
	sbiw r24,5
	call _Z10setDisplayi
.LVL54:
.L66:
	.loc 1 251 0
	ldi r18,lo8(5)
	ldi r19,0
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(112)
	ldi r23,lo8(3)
	ldi r24,lo8(13)
/* epilogue start */
.LBE10:
.LBE12:
	.loc 1 254 0
	pop r29
	pop r28
	pop r17
.LVL55:
	pop r15
	pop r14
	pop r13
	pop r12
.LBB13:
.LBB11:
	.loc 1 251 0
	jmp _Z4tonehjm
.LVL56:
.L57:
	.loc 1 213 0
	sts BUp,r25
	.loc 1 214 0
	or r18,r19
	brne .L59
.LVL57:
	.loc 1 217 0
	lds r24,status
	lds r25,status+1
	cpi r24,1
	cpc r25,__zero_reg__
	brne .L60
	.loc 1 218 0
	lds r24,display
	lds r25,display+1
	cpi r24,-30
	ldi r18,3
	cpc r25,r18
	brge .L71
	.loc 1 219 0
	adiw r24,5
	call _Z10setDisplayi
.LVL58:
.L71:
	.loc 1 216 0
	ldi r24,lo8(1)
	rjmp .L58
.LVL59:
.L60:
	.loc 1 221 0
	sbiw r24,0
	breq .L61
	.loc 1 221 0 is_stmt 0 discriminator 1
	cpi r24,3
	cpc r25,__zero_reg__
	brne .L62
.L61:
	.loc 1 222 0 is_stmt 1
	ldi r24,lo8(5)
	ldi r25,0
	call _Z10setDisplayi
.LVL60:
	.loc 1 223 0
	ldi r24,lo8(1)
	ldi r25,0
	sts status+1,r25
	sts status,r24
	.loc 1 224 0
	call millis
.LVL61:
	subi r22,24
	sbci r23,-4
	sbci r24,-1
	sbci r25,-1
	sts timerNext,r22
	sts timerNext+1,r23
	sts timerNext+2,r24
	sts timerNext+3,r25
	rjmp .L71
.LVL62:
.L62:
	.loc 1 225 0
	sbiw r24,2
	brne .L71
	.loc 1 226 0
	sts ring,__zero_reg__
	.loc 1 227 0
	movw r24,r14
	movw r22,r12
	call _Z10updateRingm
.LVL63:
	rjmp .L71
.L65:
	.loc 1 239 0
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	brlt .+2
	rjmp .L66
	.loc 1 240 0
	ldi r25,0
	ldi r24,0
	call _Z10setDisplayi
.LVL64:
	.loc 1 241 0
	ldi r24,lo8(3)
	ldi r25,0
	sts status+1,r25
	sts status,r24
	.loc 1 242 0
	ldi r24,-60
	add r12,r24
	ldi r24,9
	adc r13,r24
	adc r14,__zero_reg__
	adc r15,__zero_reg__
	sts timerNext,r12
	sts timerNext+1,r13
	sts timerNext+2,r14
	sts timerNext+3,r15
	rjmp .L66
.L64:
	.loc 1 244 0
	sbiw r24,2
	breq .+2
	rjmp .L66
	.loc 1 245 0
	sts ring,__zero_reg__
	.loc 1 246 0
	movw r24,r14
	movw r22,r12
	call _Z10updateRingm
.LVL65:
	rjmp .L66
.LVL66:
.L63:
	.loc 1 250 0
	cpse r24,__zero_reg__
	rjmp .L66
.LVL67:
.L52:
/* epilogue start */
.LBE11:
.LBE13:
	.loc 1 254 0
	pop r29
	pop r28
	pop r17
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.cfi_endproc
.LFE117:
	.size	_Z13updateButtonsm, .-_Z13updateButtonsm
	.section	.text.__vector_13,"ax",@progbits
.global	__vector_13
	.type	__vector_13, @function
__vector_13:
.LFB118:
	.loc 1 258 0
	.cfi_startproc
	push r1
.LCFI14:
	.cfi_def_cfa_offset 3
	.cfi_offset 1, -2
	push r0
.LCFI15:
	.cfi_def_cfa_offset 4
	.cfi_offset 0, -3
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
.LCFI16:
	.cfi_def_cfa_offset 5
	.cfi_offset 24, -4
	push r25
.LCFI17:
	.cfi_def_cfa_offset 6
	.cfi_offset 25, -5
	push r26
.LCFI18:
	.cfi_def_cfa_offset 7
	.cfi_offset 26, -6
	push r27
.LCFI19:
	.cfi_def_cfa_offset 8
	.cfi_offset 27, -7
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 7 */
.L__stack_usage = 7
	.loc 1 259 0
	lds r24,myMillis
	lds r25,myMillis+1
	lds r26,myMillis+2
	lds r27,myMillis+3
	adiw r24,1
	adc r26,__zero_reg__
	adc r27,__zero_reg__
	sts myMillis,r24
	sts myMillis+1,r25
	sts myMillis+2,r26
	sts myMillis+3,r27
/* epilogue start */
	.loc 1 260 0
	pop r27
	pop r26
	pop r25
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.cfi_endproc
.LFE118:
	.size	__vector_13, .-__vector_13
	.section	.rodata.setup.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Setup OK"
	.section	.text.setup,"ax",@progbits
.global	setup
	.type	setup, @function
setup:
.LFB119:
	.loc 1 262 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL68:
.LBB16:
.LBB17:
	.file 2 "/home/pif/.arduino15/packages/arduino/hardware/avr/1.8.4/cores/arduino/HardwareSerial.h"
	.loc 2 121 0
	ldi r18,lo8(6)
	ldi r20,0
	ldi r21,lo8(-62)
	ldi r22,lo8(1)
	ldi r23,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN14HardwareSerial5beginEmh
.LVL69:
.LBE17:
.LBE16:
	.loc 1 266 0
	ldi r22,lo8(1)
	ldi r24,lo8(5)
	call pinMode
.LVL70:
	.loc 1 267 0
	ldi r22,lo8(1)
	ldi r24,lo8(4)
	call pinMode
.LVL71:
	.loc 1 268 0
	ldi r22,lo8(1)
	ldi r24,lo8(3)
	call pinMode
.LVL72:
	.loc 1 270 0
	ldi r22,lo8(1)
	ldi r24,lo8(6)
	call pinMode
.LVL73:
	.loc 1 271 0
	ldi r22,lo8(1)
	ldi r24,lo8(7)
	call pinMode
.LVL74:
	.loc 1 272 0
	ldi r22,lo8(1)
	ldi r24,lo8(10)
	call pinMode
.LVL75:
	.loc 1 273 0
	ldi r22,lo8(1)
	ldi r24,lo8(11)
	call pinMode
.LVL76:
	.loc 1 274 0
	ldi r22,lo8(1)
	ldi r24,lo8(12)
	call pinMode
.LVL77:
	.loc 1 275 0
	ldi r22,lo8(1)
	ldi r24,lo8(8)
	call pinMode
.LVL78:
	.loc 1 276 0
	ldi r22,lo8(1)
	ldi r24,lo8(9)
	call pinMode
.LVL79:
	.loc 1 277 0
	ldi r22,lo8(1)
	ldi r24,lo8(2)
	call pinMode
.LVL80:
	.loc 1 279 0
	ldi r22,lo8(2)
	ldi r24,lo8(14)
	call pinMode
.LVL81:
	.loc 1 280 0
	ldi r22,lo8(2)
	ldi r24,lo8(15)
	call pinMode
.LVL82:
	.loc 1 282 0
	ldi r22,lo8(1)
	ldi r24,lo8(13)
	call pinMode
.LVL83:
	.loc 1 291 0
	ldi r24,lo8(-23)
	ldi r25,lo8(3)
	sts 134+1,r25
	sts 134,r24
	.loc 1 292 0
	ldi r24,lo8(-12)
	ldi r25,lo8(1)
	sts 136+1,r25
	sts 136,r24
	.loc 1 295 0
	ldi r24,lo8(2)
	sts 128,r24
	.loc 1 296 0
	ldi r24,lo8(18)
	sts 129,r24
	.loc 1 297 0
	sts 130,__zero_reg__
	.loc 1 298 0
	ldi r24,lo8(1)
	sts 111,r24
	.loc 1 304 0
	ldi r22,lo8(.LC0)
	ldi r23,hi8(.LC0)
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	jmp _ZN5Print7printlnEPKc
.LVL84:
	.cfi_endproc
.LFE119:
	.size	setup, .-setup
	.section	.rodata.loop.str1.1,"aMS",@progbits,1
.LC1:
	.string	"myMillis "
	.section	.text.loop,"ax",@progbits
.global	loop
	.type	loop, @function
loop:
.LFB120:
	.loc 1 309 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 311 0
	ldi r24,lo8(14)
	call digitalRead
.LVL85:
	movw r18,r24
	ldi r25,lo8(1)
	cp r18,__zero_reg__
	cpc r19,__zero_reg__
	breq .L87
	ldi r25,0
.L87:
.LVL86:
	.loc 1 312 0
	lds r24,_ZZ4loopE3BUp
	cp r24,r25
	brne .+2
	rjmp .L86
	.loc 1 313 0
	sts _ZZ4loopE3BUp,r25
	.loc 1 314 0
	or r18,r19
	breq .+2
	rjmp .L89
	.loc 1 316 0
	ldi r22,lo8(.LC1)
	ldi r23,hi8(.LC1)
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print5printEPKc
.LVL87:
	lds r20,myMillis
	lds r21,myMillis+1
	lds r22,myMillis+2
	lds r23,myMillis+3
	ldi r18,lo8(10)
	ldi r19,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEmi
.LVL88:
	.loc 1 317 0
	lds r22,128
	ldi r20,lo8(16)
	ldi r21,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEhi
.LVL89:
	.loc 1 318 0
	lds r22,129
	ldi r20,lo8(16)
	ldi r21,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEhi
.LVL90:
	.loc 1 319 0
	lds r22,130
	ldi r20,lo8(16)
	ldi r21,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEhi
.LVL91:
	.loc 1 320 0
	lds r22,134
	lds r23,134+1
	ldi r20,lo8(10)
	ldi r21,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEji
.LVL92:
	.loc 1 321 0
	lds r22,137
	lds r24,136
	mov r23,r22
	mov r22,r24
	ldi r20,lo8(10)
	ldi r21,0
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	call _ZN5Print7printlnEii
.LVL93:
	.loc 1 323 0
	lds r24,128
	andi r24,lo8(63)
	ori r24,lo8(-128)
.L93:
	.loc 1 326 0
	sts 128,r24
.L86:
/* epilogue start */
	.loc 1 330 0
	ret
.LVL94:
.L89:
	.loc 1 326 0
	lds r24,128
	andi r24,lo8(63)
	rjmp .L93
	.cfi_endproc
.LFE120:
	.size	loop, .-loop
	.section	.bss._ZZ4loopE3BUp,"aw",@nobits
	.type	_ZZ4loopE3BUp, @object
	.size	_ZZ4loopE3BUp, 1
_ZZ4loopE3BUp:
	.zero	1
.global	myMillis
	.section	.bss.myMillis,"aw",@nobits
	.type	myMillis, @object
	.size	myMillis, 4
myMillis:
	.zero	4
	.section	.bss._ZZ13updateDisplayvE5digit,"aw",@nobits
	.type	_ZZ13updateDisplayvE5digit, @object
	.size	_ZZ13updateDisplayvE5digit, 1
_ZZ13updateDisplayvE5digit:
	.zero	1
.global	mapSegments
	.section	.data.mapSegments,"aw",@progbits
	.type	mapSegments, @object
	.size	mapSegments, 16
mapSegments:
	.byte	-4
	.byte	96
	.byte	-38
	.byte	-14
	.byte	102
	.byte	-74
	.byte	-66
	.byte	-32
	.byte	-2
	.byte	-10
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
.global	buttonLastChanged
	.section	.bss.buttonLastChanged,"aw",@nobits
	.type	buttonLastChanged, @object
	.size	buttonLastChanged, 4
buttonLastChanged:
	.zero	4
.global	BUp
	.section	.bss.BUp,"aw",@nobits
	.type	BUp, @object
	.size	BUp, 1
BUp:
	.zero	1
.global	BDown
	.section	.bss.BDown,"aw",@nobits
	.type	BDown, @object
	.size	BDown, 1
BDown:
	.zero	1
.global	ring
	.section	.bss.ring,"aw",@nobits
	.type	ring, @object
	.size	ring, 1
ring:
	.zero	1
.global	timerNext
	.section	.bss.timerNext,"aw",@nobits
	.type	timerNext, @object
	.size	timerNext, 4
timerNext:
	.zero	4
.global	displayPortD
	.section	.bss.displayPortD,"aw",@nobits
	.type	displayPortD, @object
	.size	displayPortD, 3
displayPortD:
	.zero	3
.global	displayPortB
	.section	.bss.displayPortB,"aw",@nobits
	.type	displayPortB, @object
	.size	displayPortB, 3
displayPortB:
	.zero	3
.global	displaySegments
	.section	.bss.displaySegments,"aw",@nobits
	.type	displaySegments, @object
	.size	displaySegments, 3
displaySegments:
	.zero	3
.global	display
	.section	.data.display,"aw",@progbits
	.type	display, @object
	.size	display, 2
display:
	.word	-1
	.section	.bss.status,"aw",@nobits
	.type	status, @object
	.size	status, 2
status:
	.zero	2
	.text
.Letext0:
	.file 3 "/home/pif/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include/stdlib.h"
	.file 4 "/home/pif/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/lib/gcc/avr/7.3.0/include/stddef.h"
	.file 5 "/home/pif/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include/stdint.h"
	.file 6 "/home/pif/.arduino15/packages/arduino/hardware/avr/1.8.4/cores/arduino/Arduino.h"
	.file 7 "/home/pif/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include/stdio.h"
	.file 8 "/home/pif/.arduino15/packages/arduino/hardware/avr/1.8.4/cores/arduino/Print.h"
	.file 9 "/home/pif/.arduino15/packages/arduino/hardware/avr/1.8.4/variants/eightanaloginputs/../standard/pins_arduino.h"
	.file 10 "/home/pif/.arduino15/packages/arduino/hardware/avr/1.8.4/cores/arduino/Stream.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xb7d
	.word	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF94
	.byte	0x4
	.long	.LASF95
	.long	.LASF96
	.long	.Ldebug_ranges0+0x38
	.long	0
	.long	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF7
	.byte	0x4
	.byte	0xd8
	.long	0x34
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF0
	.uleb128 0x4
	.byte	0x2
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.long	.LASF1
	.uleb128 0x5
	.long	.LASF2
	.byte	0x3
	.word	0x138
	.long	0x29
	.byte	0x1
	.byte	0x1
	.uleb128 0x5
	.long	.LASF3
	.byte	0x3
	.word	0x13d
	.long	0x65
	.byte	0x1
	.byte	0x1
	.uleb128 0x6
	.byte	0x2
	.long	0x6b
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x7
	.long	0x6b
	.uleb128 0x5
	.long	.LASF5
	.byte	0x3
	.word	0x142
	.long	0x65
	.byte	0x1
	.byte	0x1
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF6
	.uleb128 0x2
	.long	.LASF8
	.byte	0x5
	.byte	0x7e
	.long	0x9c
	.uleb128 0x7
	.long	0x8c
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF9
	.uleb128 0x2
	.long	.LASF10
	.byte	0x5
	.byte	0x80
	.long	0x34
	.uleb128 0x7
	.long	0xa3
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF11
	.uleb128 0x8
	.long	0xb3
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF12
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF13
	.uleb128 0x2
	.long	.LASF14
	.byte	0x6
	.byte	0x7a
	.long	0x34
	.uleb128 0x2
	.long	.LASF15
	.byte	0x6
	.byte	0x7e
	.long	0xe3
	.uleb128 0x3
	.byte	0x1
	.byte	0x2
	.long	.LASF16
	.uleb128 0x2
	.long	.LASF17
	.byte	0x6
	.byte	0x7f
	.long	0x8c
	.uleb128 0x9
	.long	0xae
	.long	0x100
	.uleb128 0xa
	.byte	0
	.uleb128 0xb
	.long	.LASF18
	.byte	0x6
	.byte	0xa4
	.long	0xf5
	.byte	0x1
	.byte	0x1
	.uleb128 0xb
	.long	.LASF19
	.byte	0x6
	.byte	0xa5
	.long	0xf5
	.byte	0x1
	.byte	0x1
	.uleb128 0xb
	.long	.LASF20
	.byte	0x6
	.byte	0xa6
	.long	0xf5
	.byte	0x1
	.byte	0x1
	.uleb128 0x9
	.long	0x97
	.long	0x132
	.uleb128 0xa
	.byte	0
	.uleb128 0xb
	.long	.LASF21
	.byte	0x6
	.byte	0xa8
	.long	0x127
	.byte	0x1
	.byte	0x1
	.uleb128 0xb
	.long	.LASF22
	.byte	0x6
	.byte	0xaa
	.long	0x127
	.byte	0x1
	.byte	0x1
	.uleb128 0xb
	.long	.LASF23
	.byte	0x6
	.byte	0xab
	.long	0x127
	.byte	0x1
	.byte	0x1
	.uleb128 0x6
	.byte	0x2
	.long	0x72
	.uleb128 0x3
	.byte	0x4
	.byte	0x4
	.long	.LASF24
	.uleb128 0x3
	.byte	0x4
	.byte	0x4
	.long	.LASF25
	.uleb128 0xc
	.long	.LASF97
	.byte	0xe
	.byte	0x7
	.byte	0xf4
	.long	0x1ef
	.uleb128 0xd
	.string	"buf"
	.byte	0x7
	.byte	0xf5
	.long	0x65
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0xe
	.long	.LASF26
	.byte	0x7
	.byte	0xf6
	.long	0x9c
	.byte	0x2
	.byte	0x23
	.uleb128 0x2
	.uleb128 0xe
	.long	.LASF27
	.byte	0x7
	.byte	0xf7
	.long	0x8c
	.byte	0x2
	.byte	0x23
	.uleb128 0x3
	.uleb128 0xf
	.long	.LASF28
	.byte	0x7
	.word	0x107
	.long	0x3b
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x10
	.string	"len"
	.byte	0x7
	.word	0x108
	.long	0x3b
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.uleb128 0x10
	.string	"put"
	.byte	0x7
	.word	0x109
	.long	0x209
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x10
	.string	"get"
	.byte	0x7
	.word	0x10a
	.long	0x21e
	.byte	0x2
	.byte	0x23
	.uleb128 0xa
	.uleb128 0xf
	.long	.LASF29
	.byte	0x7
	.word	0x10b
	.long	0x224
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0
	.uleb128 0x11
	.long	0x3b
	.long	0x203
	.uleb128 0x12
	.long	0x6b
	.uleb128 0x12
	.long	0x203
	.byte	0
	.uleb128 0x6
	.byte	0x2
	.long	0x16d
	.uleb128 0x6
	.byte	0x2
	.long	0x1ef
	.uleb128 0x11
	.long	0x3b
	.long	0x21e
	.uleb128 0x12
	.long	0x203
	.byte	0
	.uleb128 0x6
	.byte	0x2
	.long	0x20f
	.uleb128 0x13
	.byte	0x2
	.uleb128 0x9
	.long	0x203
	.long	0x231
	.uleb128 0xa
	.byte	0
	.uleb128 0x5
	.long	.LASF30
	.byte	0x7
	.word	0x197
	.long	0x226
	.byte	0x1
	.byte	0x1
	.uleb128 0x14
	.long	.LASF98
	.byte	0x7
	.byte	0x2
	.long	0x34
	.byte	0xa
	.byte	0x29
	.long	0x263
	.uleb128 0x15
	.long	.LASF31
	.byte	0
	.uleb128 0x15
	.long	.LASF32
	.byte	0x1
	.uleb128 0x15
	.long	.LASF33
	.byte	0x2
	.byte	0
	.uleb128 0x16
	.long	.LASF34
	.byte	0x1
	.long	0x2a7
	.uleb128 0x17
	.byte	0x2
	.byte	0x5d
	.long	0x2b1
	.uleb128 0x18
	.byte	0x1
	.long	.LASF38
	.byte	0x2
	.byte	0x79
	.long	.LASF99
	.byte	0x1
	.long	0x289
	.long	0x295
	.uleb128 0x19
	.long	0xa59
	.byte	0x1
	.uleb128 0x12
	.long	0xb3
	.byte	0
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF36
	.long	.LASF38
	.byte	0x2
	.byte	0x7a
	.long	.LASF36
	.byte	0
	.uleb128 0x16
	.long	.LASF35
	.byte	0x1
	.long	0x342
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF65
	.byte	0x8
	.byte	0x39
	.long	.LASF100
	.long	0x29
	.byte	0x1
	.long	0x2ca
	.long	0x2db
	.uleb128 0x19
	.long	0x342
	.byte	0x1
	.uleb128 0x12
	.long	0x159
	.uleb128 0x12
	.long	0x29
	.byte	0
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF37
	.long	.LASF39
	.byte	0x8
	.byte	0x43
	.long	.LASF37
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF40
	.long	.LASF41
	.byte	0x8
	.byte	0x55
	.long	.LASF40
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF42
	.long	.LASF41
	.byte	0x8
	.byte	0x51
	.long	.LASF42
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF43
	.long	.LASF41
	.byte	0x8
	.byte	0x53
	.long	.LASF43
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF44
	.long	.LASF41
	.byte	0x8
	.byte	0x52
	.long	.LASF44
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF45
	.long	.LASF41
	.byte	0x8
	.byte	0x4f
	.long	.LASF45
	.byte	0
	.uleb128 0x6
	.byte	0x2
	.long	0x2a7
	.uleb128 0xb
	.long	.LASF46
	.byte	0x2
	.byte	0x8f
	.long	0x263
	.byte	0x1
	.byte	0x1
	.uleb128 0x3
	.byte	0x4
	.byte	0x4
	.long	.LASF47
	.uleb128 0x1c
	.long	.LASF101
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF48
	.uleb128 0x1d
	.string	"SS"
	.byte	0x9
	.byte	0x2b
	.long	0x97
	.byte	0xa
	.uleb128 0x1e
	.long	.LASF49
	.byte	0x9
	.byte	0x2c
	.long	0x97
	.byte	0xb
	.uleb128 0x1e
	.long	.LASF50
	.byte	0x9
	.byte	0x2d
	.long	0x97
	.byte	0xc
	.uleb128 0x1d
	.string	"SCK"
	.byte	0x9
	.byte	0x2e
	.long	0x97
	.byte	0xd
	.uleb128 0x1d
	.string	"SDA"
	.byte	0x9
	.byte	0x33
	.long	0x97
	.byte	0x12
	.uleb128 0x1d
	.string	"SCL"
	.byte	0x9
	.byte	0x34
	.long	0x97
	.byte	0x13
	.uleb128 0x1d
	.string	"A0"
	.byte	0x9
	.byte	0x41
	.long	0x97
	.byte	0xe
	.uleb128 0x1d
	.string	"A1"
	.byte	0x9
	.byte	0x42
	.long	0x97
	.byte	0xf
	.uleb128 0x1d
	.string	"A2"
	.byte	0x9
	.byte	0x43
	.long	0x97
	.byte	0x10
	.uleb128 0x1d
	.string	"A3"
	.byte	0x9
	.byte	0x44
	.long	0x97
	.byte	0x11
	.uleb128 0x1d
	.string	"A4"
	.byte	0x9
	.byte	0x45
	.long	0x97
	.byte	0x12
	.uleb128 0x1d
	.string	"A5"
	.byte	0x9
	.byte	0x46
	.long	0x97
	.byte	0x13
	.uleb128 0x1d
	.string	"A6"
	.byte	0x9
	.byte	0x47
	.long	0x97
	.byte	0x14
	.uleb128 0x1d
	.string	"A7"
	.byte	0x9
	.byte	0x48
	.long	0x97
	.byte	0x15
	.uleb128 0x1f
	.byte	0x7
	.byte	0x2
	.long	0x34
	.byte	0x1
	.byte	0x4a
	.long	0x42d
	.uleb128 0x20
	.string	"OFF"
	.byte	0
	.uleb128 0x15
	.long	.LASF51
	.byte	0x1
	.uleb128 0x15
	.long	.LASF52
	.byte	0x2
	.uleb128 0x15
	.long	.LASF53
	.byte	0x3
	.byte	0
	.uleb128 0x21
	.long	.LASF54
	.byte	0x1
	.byte	0x4f
	.long	0x407
	.byte	0x5
	.byte	0x3
	.long	status
	.uleb128 0x22
	.long	.LASF55
	.byte	0x1
	.byte	0x54
	.long	0x3b
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	display
	.uleb128 0x9
	.long	0xea
	.long	0x460
	.uleb128 0x23
	.long	0x34
	.byte	0x2
	.byte	0
	.uleb128 0x22
	.long	.LASF56
	.byte	0x1
	.byte	0x55
	.long	0x450
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	displaySegments
	.uleb128 0x22
	.long	.LASF57
	.byte	0x1
	.byte	0x56
	.long	0x450
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	displayPortB
	.uleb128 0x22
	.long	.LASF58
	.byte	0x1
	.byte	0x57
	.long	0x450
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	displayPortD
	.uleb128 0x22
	.long	.LASF59
	.byte	0x1
	.byte	0x5a
	.long	0xb3
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	timerNext
	.uleb128 0x22
	.long	.LASF60
	.byte	0x1
	.byte	0x5d
	.long	0xea
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	ring
	.uleb128 0x22
	.long	.LASF61
	.byte	0x1
	.byte	0x61
	.long	0xea
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	BDown
	.uleb128 0x24
	.string	"BUp"
	.byte	0x1
	.byte	0x61
	.long	0xea
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	BUp
	.uleb128 0x22
	.long	.LASF62
	.byte	0x1
	.byte	0x63
	.long	0xb3
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	buttonLastChanged
	.uleb128 0x9
	.long	0xea
	.long	0x500
	.uleb128 0x23
	.long	0x34
	.byte	0xf
	.byte	0
	.uleb128 0x22
	.long	.LASF63
	.byte	0x1
	.byte	0x67
	.long	0x4f0
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	mapSegments
	.uleb128 0x25
	.long	.LASF64
	.byte	0x1
	.word	0x101
	.long	0xba
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	myMillis
	.uleb128 0x26
	.byte	0x1
	.long	.LASF66
	.byte	0x1
	.word	0x135
	.long	.LFB120
	.long	.LFE120
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x61d
	.uleb128 0x27
	.string	"BUp"
	.byte	0x1
	.word	0x136
	.long	0xea
	.byte	0x5
	.byte	0x3
	.long	_ZZ4loopE3BUp
	.uleb128 0x28
	.long	.LASF67
	.byte	0x1
	.word	0x137
	.long	0xea
	.long	.LLST18
	.uleb128 0x29
	.long	.LVL85
	.long	0xb37
	.long	0x574
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3e
	.byte	0
	.uleb128 0x29
	.long	.LVL87
	.long	0x2db
	.long	0x590
	.uleb128 0x2a
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	.LC1
	.byte	0
	.uleb128 0x29
	.long	.LVL88
	.long	0x2ec
	.long	0x5a8
	.uleb128 0x2a
	.byte	0x6
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x3a
	.byte	0
	.uleb128 0x29
	.long	.LVL89
	.long	0x2fd
	.long	0x5c0
	.uleb128 0x2a
	.byte	0x6
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x40
	.byte	0
	.uleb128 0x29
	.long	.LVL90
	.long	0x2fd
	.long	0x5d8
	.uleb128 0x2a
	.byte	0x6
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x40
	.byte	0
	.uleb128 0x29
	.long	.LVL91
	.long	0x2fd
	.long	0x5f0
	.uleb128 0x2a
	.byte	0x6
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x40
	.byte	0
	.uleb128 0x29
	.long	.LVL92
	.long	0x30e
	.long	0x608
	.uleb128 0x2a
	.byte	0x6
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x3a
	.byte	0
	.uleb128 0x2b
	.long	.LVL93
	.long	0x31f
	.uleb128 0x2a
	.byte	0x6
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x3a
	.byte	0
	.byte	0
	.uleb128 0x26
	.byte	0x1
	.long	.LASF68
	.byte	0x1
	.word	0x106
	.long	.LFB119
	.long	.LFE119
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x7e6
	.uleb128 0x2c
	.long	0xa64
	.long	.LBB16
	.long	.LBE16
	.byte	0x1
	.word	0x108
	.long	0x67c
	.uleb128 0x2d
	.long	0xa80
	.long	.LLST17
	.uleb128 0x2e
	.long	0xa76
	.uleb128 0x2b
	.long	.LVL69
	.long	0x295
	.uleb128 0x2a
	.byte	0xc
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x4
	.byte	0x8
	.byte	0xe1
	.byte	0x39
	.byte	0x24
	.uleb128 0x2a
	.byte	0x1
	.byte	0x62
	.byte	0x1
	.byte	0x36
	.byte	0
	.byte	0
	.uleb128 0x29
	.long	.LVL70
	.long	0xb44
	.long	0x694
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x35
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL71
	.long	0xb44
	.long	0x6ac
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x34
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL72
	.long	0xb44
	.long	0x6c4
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x33
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL73
	.long	0xb44
	.long	0x6dc
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x36
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL74
	.long	0xb44
	.long	0x6f4
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x37
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL75
	.long	0xb44
	.long	0x70c
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3a
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL76
	.long	0xb44
	.long	0x724
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3b
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL77
	.long	0xb44
	.long	0x73c
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3c
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL78
	.long	0xb44
	.long	0x754
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x38
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL79
	.long	0xb44
	.long	0x76c
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x39
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL80
	.long	0xb44
	.long	0x784
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x32
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x29
	.long	.LVL81
	.long	0xb44
	.long	0x79c
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3e
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x32
	.byte	0
	.uleb128 0x29
	.long	.LVL82
	.long	0xb44
	.long	0x7b4
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3f
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x32
	.byte	0
	.uleb128 0x29
	.long	.LVL83
	.long	0xb44
	.long	0x7cc
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3d
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x1
	.byte	0x31
	.byte	0
	.uleb128 0x2f
	.long	.LVL84
	.byte	0x1
	.long	0x330
	.uleb128 0x2a
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x5
	.byte	0x3
	.long	.LC0
	.byte	0
	.byte	0
	.uleb128 0x30
	.byte	0x1
	.long	.LASF102
	.byte	0x1
	.word	0x102
	.long	.LFB118
	.long	.LFE118
	.long	.LLST16
	.byte	0x1
	.uleb128 0x31
	.byte	0x1
	.long	.LASF71
	.byte	0x1
	.byte	0xce
	.long	.LASF73
	.long	.LFB117
	.long	.LFE117
	.long	.LLST11
	.byte	0x1
	.long	0x900
	.uleb128 0x32
	.string	"now"
	.byte	0x1
	.byte	0xce
	.long	0xb3
	.long	.LLST12
	.uleb128 0x33
	.long	.LASF69
	.byte	0x1
	.byte	0xcf
	.long	0xd8
	.long	.LLST13
	.uleb128 0x34
	.long	.Ldebug_ranges0+0x18
	.uleb128 0x33
	.long	.LASF70
	.byte	0x1
	.byte	0xd2
	.long	0xea
	.long	.LLST14
	.uleb128 0x33
	.long	.LASF67
	.byte	0x1
	.byte	0xd3
	.long	0xea
	.long	.LLST15
	.uleb128 0x29
	.long	.LVL48
	.long	0xb37
	.long	0x86d
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3f
	.byte	0
	.uleb128 0x29
	.long	.LVL50
	.long	0xb37
	.long	0x880
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3e
	.byte	0
	.uleb128 0x35
	.long	.LVL54
	.long	0x9e1
	.uleb128 0x36
	.long	.LVL56
	.byte	0x1
	.long	0xb51
	.long	0x8b9
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3d
	.uleb128 0x2a
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.byte	0xa
	.word	0x370
	.uleb128 0x2a
	.byte	0xc
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x35
	.byte	0
	.uleb128 0x35
	.long	.LVL58
	.long	0x9e1
	.uleb128 0x29
	.long	.LVL60
	.long	0x9e1
	.long	0x8da
	.uleb128 0x2a
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x35
	.byte	0
	.uleb128 0x35
	.long	.LVL61
	.long	0xb62
	.uleb128 0x35
	.long	.LVL63
	.long	0x900
	.uleb128 0x35
	.long	.LVL64
	.long	0x9e1
	.uleb128 0x35
	.long	.LVL65
	.long	0x900
	.byte	0
	.byte	0
	.uleb128 0x37
	.byte	0x1
	.long	.LASF103
	.byte	0x1
	.byte	0xbb
	.long	.LASF104
	.byte	0x1
	.long	0x91d
	.uleb128 0x38
	.string	"now"
	.byte	0x1
	.byte	0xbb
	.long	0xb3
	.byte	0
	.uleb128 0x31
	.byte	0x1
	.long	.LASF72
	.byte	0x1
	.byte	0xaf
	.long	.LASF74
	.long	.LFB115
	.long	.LFE115
	.long	.LLST7
	.byte	0x1
	.long	0x95c
	.uleb128 0x21
	.long	.LASF75
	.byte	0x1
	.byte	0xb6
	.long	0xea
	.byte	0x5
	.byte	0x3
	.long	_ZZ13updateDisplayvE5digit
	.uleb128 0x2b
	.long	.LVL34
	.long	0x95c
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x2
	.byte	0x8c
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0x39
	.byte	0x1
	.long	.LASF76
	.byte	0x1
	.byte	0xaa
	.long	.LASF77
	.long	.LFB114
	.long	.LFE114
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x989
	.uleb128 0x3a
	.long	.LASF75
	.byte	0x1
	.byte	0xaa
	.long	0xea
	.long	.LLST6
	.byte	0
	.uleb128 0x39
	.byte	0x1
	.long	.LASF78
	.byte	0x1
	.byte	0x92
	.long	.LASF79
	.long	.LFB113
	.long	.LFE113
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x9e1
	.uleb128 0x3a
	.long	.LASF75
	.byte	0x1
	.byte	0x92
	.long	0xea
	.long	.LLST0
	.uleb128 0x3b
	.long	.LASF80
	.byte	0x1
	.byte	0x92
	.long	0xea
	.byte	0x1
	.byte	0x66
	.uleb128 0x33
	.long	.LASF81
	.byte	0x1
	.byte	0x93
	.long	0xea
	.long	.LLST1
	.uleb128 0x33
	.long	.LASF82
	.byte	0x1
	.byte	0x93
	.long	0xea
	.long	.LLST2
	.byte	0
	.uleb128 0x31
	.byte	0x1
	.long	.LASF83
	.byte	0x1
	.byte	0x77
	.long	.LASF84
	.long	.LFB112
	.long	.LFE112
	.long	.LLST3
	.byte	0x1
	.long	0xa59
	.uleb128 0x3a
	.long	.LASF85
	.byte	0x1
	.byte	0x77
	.long	0x3b
	.long	.LLST4
	.uleb128 0x33
	.long	.LASF80
	.byte	0x1
	.byte	0x79
	.long	0x450
	.long	.LLST5
	.uleb128 0x29
	.long	.LVL27
	.long	0x989
	.long	0xa2f
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x30
	.byte	0
	.uleb128 0x29
	.long	.LVL28
	.long	0x989
	.long	0xa48
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x31
	.uleb128 0x2a
	.byte	0x1
	.byte	0x66
	.byte	0x2
	.byte	0x8d
	.sleb128 0
	.byte	0
	.uleb128 0x2f
	.long	.LVL31
	.byte	0x1
	.long	0x989
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x32
	.byte	0
	.byte	0
	.uleb128 0x6
	.byte	0x2
	.long	0x263
	.uleb128 0x7
	.long	0xa59
	.uleb128 0x3c
	.long	0x274
	.long	0xa76
	.byte	0x3
	.long	0xa76
	.long	0xa8c
	.uleb128 0x3d
	.long	.LASF105
	.long	0xa5f
	.byte	0x1
	.uleb128 0x3e
	.long	.LASF86
	.byte	0x2
	.byte	0x79
	.long	0xb3
	.byte	0
	.uleb128 0x3f
	.long	0x900
	.long	.LASF104
	.long	.LFB116
	.long	.LFE116
	.long	.LLST8
	.byte	0x1
	.long	0xb37
	.uleb128 0x2d
	.long	0x911
	.long	.LLST9
	.uleb128 0x40
	.long	0x900
	.long	.Ldebug_ranges0+0
	.long	0xaef
	.uleb128 0x2d
	.long	0x911
	.long	.LLST10
	.uleb128 0x29
	.long	.LVL37
	.long	0xb6f
	.long	0xad8
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3d
	.byte	0
	.uleb128 0x2f
	.long	.LVL40
	.byte	0x1
	.long	0x9e1
	.uleb128 0x2a
	.byte	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.byte	0x2
	.byte	0x9
	.byte	0xfe
	.byte	0
	.byte	0
	.uleb128 0x29
	.long	.LVL42
	.long	0xb51
	.long	0xb1e
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3d
	.uleb128 0x2a
	.byte	0x6
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.byte	0xa
	.word	0x3e8
	.uleb128 0x2a
	.byte	0xc
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x63
	.byte	0x93
	.uleb128 0x1
	.byte	0x64
	.byte	0x93
	.uleb128 0x1
	.byte	0x65
	.byte	0x93
	.uleb128 0x1
	.byte	0x1
	.byte	0x30
	.byte	0
	.uleb128 0x35
	.long	.LVL43
	.long	0x9e1
	.uleb128 0x2b
	.long	.LVL45
	.long	0xb6f
	.uleb128 0x2a
	.byte	0x1
	.byte	0x68
	.byte	0x1
	.byte	0x3d
	.byte	0
	.byte	0
	.uleb128 0x41
	.byte	0x1
	.byte	0x1
	.long	.LASF87
	.long	.LASF87
	.byte	0x6
	.byte	0x88
	.uleb128 0x41
	.byte	0x1
	.byte	0x1
	.long	.LASF88
	.long	.LASF88
	.byte	0x6
	.byte	0x86
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF89
	.long	.LASF90
	.byte	0x6
	.byte	0xf7
	.long	.LASF89
	.uleb128 0x41
	.byte	0x1
	.byte	0x1
	.long	.LASF91
	.long	.LASF91
	.byte	0x6
	.byte	0x8d
	.uleb128 0x1a
	.byte	0x1
	.byte	0x1
	.long	.LASF92
	.long	.LASF93
	.byte	0x6
	.byte	0xf8
	.long	.LASF92
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x8
	.byte	0
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x3b
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2a
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0xa
	.uleb128 0x2111
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2c
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2d
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2e
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2f
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x2115
	.uleb128 0xc
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x30
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x31
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x32
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x33
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x34
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x35
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x36
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x2115
	.uleb128 0xc
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x37
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x39
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x3c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3d
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x3e
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3f
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x40
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x41
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST18:
	.long	.LVL86
	.long	.LVL87-1
	.word	0x1
	.byte	0x69
	.long	.LVL94
	.long	.LFE120
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LLST17:
	.long	.LVL68
	.long	.LVL69
	.word	0x5
	.byte	0x8
	.byte	0xe1
	.byte	0x39
	.byte	0x24
	.byte	0x9f
	.long	0
	.long	0
.LLST16:
	.long	.LFB118
	.long	.LCFI14
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI14
	.long	.LCFI15
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI15
	.long	.LCFI16
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI16
	.long	.LCFI17
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI17
	.long	.LCFI18
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	.LCFI18
	.long	.LCFI19
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 7
	.long	.LCFI19
	.long	.LFE118
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 8
	.long	0
	.long	0
.LLST11:
	.long	.LFB117
	.long	.LCFI7
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI7
	.long	.LCFI8
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI8
	.long	.LCFI9
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI9
	.long	.LCFI10
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI10
	.long	.LCFI11
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	.LCFI11
	.long	.LCFI12
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 7
	.long	.LCFI12
	.long	.LCFI13
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 8
	.long	.LCFI13
	.long	.LFE117
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 9
	.long	0
	.long	0
.LLST12:
	.long	.LVL46
	.long	.LVL48-1
	.word	0xc
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL48-1
	.long	.LFE117
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x66
	.byte	0x9f
	.long	0
	.long	0
.LLST13:
	.long	.LVL47
	.long	.LVL52
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL52
	.long	.LVL53
	.word	0x1
	.byte	0x68
	.long	.LVL53
	.long	.LVL54
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL56
	.long	.LVL57
	.word	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL57
	.long	.LVL66
	.word	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL66
	.long	.LVL67
	.word	0x1
	.byte	0x68
	.long	0
	.long	0
.LLST14:
	.long	.LVL49
	.long	.LVL55
	.word	0x1
	.byte	0x61
	.long	.LVL56
	.long	.LVL67
	.word	0x1
	.byte	0x61
	.long	0
	.long	0
.LLST15:
	.long	.LVL51
	.long	.LVL52
	.word	0x1
	.byte	0x69
	.long	.LVL56
	.long	.LVL58-1
	.word	0x1
	.byte	0x69
	.long	.LVL59
	.long	.LVL60-1
	.word	0x1
	.byte	0x69
	.long	.LVL62
	.long	.LVL63-1
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LLST7:
	.long	.LFB115
	.long	.LCFI2
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI2
	.long	.LFE115
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	0
	.long	0
.LLST6:
	.long	.LVL32
	.long	.LVL33
	.word	0x1
	.byte	0x68
	.long	.LVL33
	.long	.LFE114
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x68
	.byte	0x9f
	.long	0
	.long	0
.LLST0:
	.long	.LVL0
	.long	.LVL10
	.word	0x1
	.byte	0x68
	.long	.LVL10
	.long	.LVL11
	.word	0x1
	.byte	0x64
	.long	.LVL11
	.long	.LVL14
	.word	0x1
	.byte	0x68
	.long	.LVL14
	.long	.LFE113
	.word	0x1
	.byte	0x64
	.long	0
	.long	0
.LLST1:
	.long	.LVL0
	.long	.LVL4
	.word	0x2
	.byte	0x4f
	.byte	0x9f
	.long	.LVL4
	.long	.LVL12
	.word	0x1
	.byte	0x62
	.long	.LVL12
	.long	.LVL14
	.word	0x2
	.byte	0x4f
	.byte	0x9f
	.long	.LVL14
	.long	.LFE113
	.word	0x1
	.byte	0x62
	.long	0
	.long	0
.LLST2:
	.long	.LVL1
	.long	.LVL2
	.word	0x6
	.byte	0x89
	.sleb128 0
	.byte	0x9
	.byte	0xc4
	.byte	0x21
	.byte	0x9f
	.long	.LVL2
	.long	.LVL12
	.word	0x1
	.byte	0x69
	.long	.LVL12
	.long	.LVL13
	.word	0x6
	.byte	0x89
	.sleb128 0
	.byte	0x9
	.byte	0xc4
	.byte	0x21
	.byte	0x9f
	.long	.LVL13
	.long	.LFE113
	.word	0x1
	.byte	0x69
	.long	0
	.long	0
.LLST3:
	.long	.LFB112
	.long	.LCFI0
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI0
	.long	.LCFI1
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI1
	.long	.LFE112
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	0
	.long	0
.LLST4:
	.long	.LVL18
	.long	.LVL20
	.word	0x6
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL20
	.long	.LVL24
	.word	0x6
	.byte	0x6e
	.byte	0x93
	.uleb128 0x1
	.byte	0x6f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL24
	.long	.LVL27-1
	.word	0x5
	.byte	0x3
	.long	display
	.long	.LVL27-1
	.long	.LFE112
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x68
	.byte	0x9f
	.long	0
	.long	0
.LLST5:
	.long	.LVL19
	.long	.LVL21
	.word	0xc
	.byte	0x30
	.byte	0x9f
	.byte	0x93
	.uleb128 0x1
	.byte	0x30
	.byte	0x9f
	.byte	0x93
	.uleb128 0x1
	.byte	0x30
	.byte	0x9f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL21
	.long	.LVL22
	.word	0x9
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.long	.LVL22
	.long	.LVL23
	.word	0x15
	.byte	0x3
	.long	mapSegments+10
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.long	mapSegments+10
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.long	mapSegments+10
	.byte	0x93
	.uleb128 0x1
	.long	.LVL23
	.long	.LVL25
	.word	0x11
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.long	mapSegments+10
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.long	mapSegments+10
	.byte	0x93
	.uleb128 0x1
	.long	.LVL25
	.long	.LVL26
	.word	0xd
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.byte	0x3
	.long	mapSegments+10
	.byte	0x93
	.uleb128 0x1
	.long	.LVL26
	.long	.LVL27-1
	.word	0x9
	.byte	0x62
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.long	.LVL27-1
	.long	.LVL29
	.word	0x8
	.byte	0x93
	.uleb128 0x1
	.byte	0x6d
	.byte	0x93
	.uleb128 0x1
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.long	.LVL29
	.long	.LVL30
	.word	0x5
	.byte	0x93
	.uleb128 0x2
	.byte	0x6c
	.byte	0x93
	.uleb128 0x1
	.long	.LVL30
	.long	.LVL31-1
	.word	0x5
	.byte	0x93
	.uleb128 0x2
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.long	0
	.long	0
.LLST8:
	.long	.LFB116
	.long	.LCFI3
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.long	.LCFI3
	.long	.LCFI4
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 3
	.long	.LCFI4
	.long	.LCFI5
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 4
	.long	.LCFI5
	.long	.LCFI6
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 5
	.long	.LCFI6
	.long	.LFE116
	.word	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 6
	.long	0
	.long	0
.LLST9:
	.long	.LVL35
	.long	.LVL37-1
	.word	0xc
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL37-1
	.long	.LVL40
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x66
	.byte	0x9f
	.long	.LVL40
	.long	.LVL41
	.word	0xc
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL41
	.long	.LVL44
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x66
	.byte	0x9f
	.long	.LVL44
	.long	.LVL45-1
	.word	0xc
	.byte	0x66
	.byte	0x93
	.uleb128 0x1
	.byte	0x67
	.byte	0x93
	.uleb128 0x1
	.byte	0x68
	.byte	0x93
	.uleb128 0x1
	.byte	0x69
	.byte	0x93
	.uleb128 0x1
	.long	.LVL45-1
	.long	.LFE116
	.word	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x66
	.byte	0x9f
	.long	0
	.long	0
.LLST10:
	.long	.LVL36
	.long	.LVL38
	.word	0xc
	.byte	0x5c
	.byte	0x93
	.uleb128 0x1
	.byte	0x5d
	.byte	0x93
	.uleb128 0x1
	.byte	0x5e
	.byte	0x93
	.uleb128 0x1
	.byte	0x5f
	.byte	0x93
	.uleb128 0x1
	.long	.LVL38
	.long	.LVL39
	.word	0x4
	.byte	0x7c
	.sleb128 -2500
	.byte	0x9f
	.long	.LVL39
	.long	.LVL40-1
	.word	0xb
	.byte	0x3
	.long	timerNext
	.byte	0x6
	.byte	0xa
	.word	0x9c4
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
	.section	.debug_aranges,"",@progbits
	.long	0x5c
	.word	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.word	0
	.word	0
	.long	.LFB113
	.long	.LFE113-.LFB113
	.long	.LFB112
	.long	.LFE112-.LFB112
	.long	.LFB114
	.long	.LFE114-.LFB114
	.long	.LFB115
	.long	.LFE115-.LFB115
	.long	.LFB116
	.long	.LFE116-.LFB116
	.long	.LFB117
	.long	.LFE117-.LFB117
	.long	.LFB118
	.long	.LFE118-.LFB118
	.long	.LFB119
	.long	.LFE119-.LFB119
	.long	.LFB120
	.long	.LFE120-.LFB120
	.long	0
	.long	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.LBB4
	.long	.LBE4
	.long	.LBB7
	.long	.LBE7
	.long	0
	.long	0
	.long	.LBB9
	.long	.LBE9
	.long	.LBB10
	.long	.LBE10
	.long	.LBB11
	.long	.LBE11
	.long	0
	.long	0
	.long	.LFB113
	.long	.LFE113
	.long	.LFB112
	.long	.LFE112
	.long	.LFB114
	.long	.LFE114
	.long	.LFB115
	.long	.LFE115
	.long	.LFB116
	.long	.LFE116
	.long	.LFB117
	.long	.LFE117
	.long	.LFB118
	.long	.LFE118
	.long	.LFB119
	.long	.LFE119
	.long	.LFB120
	.long	.LFE120
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF63:
	.string	"mapSegments"
.LASF21:
	.string	"digital_pin_to_port_PGM"
.LASF70:
	.string	"newBDown"
.LASF43:
	.string	"_ZN5Print7printlnEji"
.LASF7:
	.string	"size_t"
.LASF23:
	.string	"digital_pin_to_timer_PGM"
.LASF2:
	.string	"__malloc_margin"
.LASF26:
	.string	"unget"
.LASF13:
	.string	"long long unsigned int"
.LASF59:
	.string	"timerNext"
.LASF34:
	.string	"HardwareSerial"
.LASF71:
	.string	"updateButtons"
.LASF12:
	.string	"long long int"
.LASF6:
	.string	"signed char"
.LASF90:
	.string	"tone"
.LASF81:
	.string	"Bbits"
.LASF69:
	.string	"mustBeep"
.LASF75:
	.string	"digit"
.LASF94:
	.string	"GNU C++11 7.3.0 -mn-flash=1 -mmcu=avr5 -g -Os -std=gnu++11 -fpermissive -ffunction-sections -fdata-sections -fno-threadsafe-statics -mn-flash=1 -mno-skip-bug -fno-rtti -fno-enforce-eh-specs -fno-exceptions"
.LASF3:
	.string	"__malloc_heap_start"
.LASF39:
	.string	"print"
.LASF1:
	.string	"long int"
.LASF29:
	.string	"udata"
.LASF93:
	.string	"noTone"
.LASF74:
	.string	"_Z13updateDisplayv"
.LASF104:
	.string	"_Z10updateRingm"
.LASF103:
	.string	"updateRing"
.LASF10:
	.string	"uint16_t"
.LASF25:
	.string	"double"
.LASF61:
	.string	"BDown"
.LASF105:
	.string	"this"
.LASF19:
	.string	"port_to_input_PGM"
.LASF17:
	.string	"byte"
.LASF91:
	.string	"millis"
.LASF53:
	.string	"STOPPING"
.LASF18:
	.string	"port_to_mode_PGM"
.LASF55:
	.string	"display"
.LASF85:
	.string	"value"
.LASF0:
	.string	"unsigned int"
.LASF89:
	.string	"_Z4tonehjm"
.LASF38:
	.string	"begin"
.LASF76:
	.string	"outputDigit"
.LASF66:
	.string	"loop"
.LASF11:
	.string	"long unsigned int"
.LASF20:
	.string	"port_to_output_PGM"
.LASF57:
	.string	"displayPortB"
.LASF92:
	.string	"_Z6noToneh"
.LASF58:
	.string	"displayPortD"
.LASF100:
	.string	"_ZN5Print5writeEPKcj"
.LASF5:
	.string	"__malloc_heap_end"
.LASF28:
	.string	"size"
.LASF48:
	.string	"short unsigned int"
.LASF36:
	.string	"_ZN14HardwareSerial5beginEmh"
.LASF40:
	.string	"_ZN5Print7printlnEmi"
.LASF16:
	.string	"bool"
.LASF72:
	.string	"updateDisplay"
.LASF73:
	.string	"_Z13updateButtonsm"
.LASF101:
	.string	"decltype(nullptr)"
.LASF79:
	.string	"_Z12prepareDigithh"
.LASF65:
	.string	"write"
.LASF47:
	.string	"long double"
.LASF30:
	.string	"__iob"
.LASF80:
	.string	"segments"
.LASF68:
	.string	"setup"
.LASF37:
	.string	"_ZN5Print5printEPKc"
.LASF84:
	.string	"_Z10setDisplayi"
.LASF24:
	.string	"float"
.LASF77:
	.string	"_Z11outputDigith"
.LASF78:
	.string	"prepareDigit"
.LASF9:
	.string	"unsigned char"
.LASF97:
	.string	"__file"
.LASF64:
	.string	"myMillis"
.LASF51:
	.string	"RUNNING"
.LASF41:
	.string	"println"
.LASF83:
	.string	"setDisplay"
.LASF86:
	.string	"baud"
.LASF42:
	.string	"_ZN5Print7printlnEhi"
.LASF4:
	.string	"char"
.LASF98:
	.string	"LookaheadMode"
.LASF22:
	.string	"digital_pin_to_bit_mask_PGM"
.LASF49:
	.string	"MOSI"
.LASF15:
	.string	"boolean"
.LASF52:
	.string	"RINGING"
.LASF62:
	.string	"buttonLastChanged"
.LASF96:
	.string	"/home/pif/GitHub/piif/ArduinoTests/kitchenTimer"
.LASF99:
	.string	"_ZN14HardwareSerial5beginEm"
.LASF14:
	.string	"word"
.LASF31:
	.string	"SKIP_ALL"
.LASF50:
	.string	"MISO"
.LASF95:
	.string	"build/sketch/kitchenTimer.ino.cpp"
.LASF54:
	.string	"status"
.LASF88:
	.string	"pinMode"
.LASF56:
	.string	"displaySegments"
.LASF44:
	.string	"_ZN5Print7printlnEii"
.LASF33:
	.string	"SKIP_WHITESPACE"
.LASF35:
	.string	"Print"
.LASF8:
	.string	"uint8_t"
.LASF82:
	.string	"Dbits"
.LASF27:
	.string	"flags"
.LASF60:
	.string	"ring"
.LASF102:
	.string	"__vector_13"
.LASF67:
	.string	"newBUp"
.LASF45:
	.string	"_ZN5Print7printlnEPKc"
.LASF87:
	.string	"digitalRead"
.LASF32:
	.string	"SKIP_NONE"
.LASF46:
	.string	"Serial"
	.ident	"GCC: (GNU) 7.3.0"
.global __do_copy_data
.global __do_clear_bss
