;
; AssemblerApplication11.asm
//lab 2 part b

//.include "m2560def.inc"

.set array_size = 5
.set insert_size = 5

.dseg
	list: .byte 30
.cseg
	original: .db 1, 4, 8, 10, 12		//sorted array
  	insert: .db 0, 28, 1, 5, 13			// array to be inserted

start:
	ldi ZL, low(2*original)
	ldi ZH, high(2*original)
	ldi XL, low(list)
	ldi XH, high(list)
	ldi r20, array_size
	ldi r23, insert_size
	ldi r24, array_size

initial_loop:
	lpm r16, Z+
	st X+, r16
	dec r20
	cpi r20, 0
	breq insert_setup
	rjmp initial_loop

insert_setup:
	ldi ZL, low(2*insert)
	ldi ZH, high(2*insert)
	ldi XL, low(list)
	ldi XH, high(list)
	
	ldi r28, low(RAMEND)
	ldi r29, high(RAMEND)
	out SPH, r29
	out SPL, r28

	inc r23

insert_loop:

	lpm r16, Z+
	mov r19, r24

	dec r23
	cpi r23, 0
	breq halt

	rcall insert_request
	rjmp insert_loop
	
insert_request:
	push YL
	push YH
	push r16
	push r19
	push r26
	push r27
/*	in YL, SPL
	in YH, SPH
	sbiw Y, 4
	out SPH, YH
	out SPL, YL	*/

	std Y+1, r16
	std Y+2, r24
	std Y+3, r26
	std Y+4, r27

	number_check:
		ld r17, X+
		cp r16, r17
		breq return
		cp r16, r17
		brlo pre_insert
		dec r19
		cpi r19, 0
		breq store
		rjmp number_check

	pre_insert:	
		inc r24
		ld r18, -X

	insertion:
		ld r17, X
		st X+, r16
		mov r16, r17
		cpi r19, 0
		breq return
		dec r19
		rjmp insertion

	store:
		st X, r16
		inc r24
		rjmp return

	return:
		pop r27
		pop r26
		pop r19
		pop r16
		pop YH
		pop YL
			
		//rjmp insert_loop
	ret

halt:
	rjmp halt

