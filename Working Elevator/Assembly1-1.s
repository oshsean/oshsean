//part a
.dseg
	list: .byte 30
.cseg
	original: .db "ertxrytfuygiuhojdjfkg", '\0'

main:
	ldi ZL, low(2*original)
	ldi ZH, high(2*original)
	ldi XL, low(list)
	ldi XH, high(list)

	ldi r28, low(RAMEND)
	ldi r29, high(RAMEND)
	out SPH, r29
	out SPL, r28


pushLoop:	// stores all elements of the string on the stack

	lpm r16, Z+	//next element
	cpi r16, 0
	breq popLoop
	push r16
	inc r20
	rjmp pushLoop


popLoop:	//removes element from stack to store in data memory
	pop r16
	st X+, r16

	dec r20
	cpi r20, 0
	breq halt

	rjmp popLoop

;null:
;	ldi r19, 0
;	st X+, r19

halt:
	rjmp halt