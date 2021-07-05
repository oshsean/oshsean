;
; AssemblerApplication12.asm
;
//lab 2 part c

//.include "m2560def.inc"

.set array_size = 5
.set insert_size = 5

.dseg
	list: .byte 30
.cseg
	original: .db 10, 9, 7, 15, 17		//sorted array
  	insert: .db 8, 16, 9, 12, 5			// array to be inserted

start:
	ldi ZL, low(2*original)
	ldi ZH, high(2*original)
	ldi XL, low(list)
	ldi XH, high(list)
	ldi r18, 0      //is turning found
	ldi r19, 0		// stored array size
	ldi r20, array_size
	ldi r23, insert_size
	ldi r24, 0	// turning size
	ldi r25, 0   //ascend or decend
	ldi r28, low(RAMEND)
	ldi r29, high(RAMEND)
	out SPH, r29
	out SPL, r28

initial_loop:
	lpm r16, Z+
	lpm r17, Z
	st X+, r16
	cpi r25, 0
	breq direct
	cpi r18, 0
	breq turning
	dec r20
	inc r19
	cpi r20, 0
	breq pre_insertion
	rjmp initial_loop

direct:  //check direction 
	inc r24
	cp r16, r17
	breq initial_loop

	cp r17, r16
	brlo decending

ascending:
	inc r25					//decending r25 = 1
decending:
	inc r25					//ascending r25 = 2
	dec r20
	inc r19
	cpi r20, 0
	breq pre_insertion
	rjmp initial_loop

turning: //find when elevator turns
	inc r24
	cpi r25, 2
	breq less

more:    
	cp r17, r16
	brlo save
	dec r20
	inc r19
	cpi r20, 0
	breq pre_insertion
	rjmp initial_loop
less:
	cp r17, r16
	brlo save
	dec r20
	inc r19
	cpi r20, 0
	breq pre_insertion
	rjmp initial_loop
save:
	inc r18
	dec r20
	inc r19
	cpi r20, 0
	breq pre_insertion
	rjmp initial_loop


pre_insertion:
	ldi ZL, low(2*insert)
	ldi ZH, high(2*insert)
//	ldi r28, low(RAMEND)
//	ldi r29, high(RAMEND)
//	out SPH, r29
//	out SPL, r28
//	inc r23
next:
	mov r20, r19
	lpm r16, Z+
	cpi r16, 0
	breq halt
//	ld r25, X
//	st X, r16
//	st X, r25
equal_floor:

	ld r17, -X
	cp r16, r17
	breq double
	cpi r20, 0
	breq insertion
	dec r20
	rjmp equal_floor

double:  
	ld r17, X+
	cp r20, r19
	breq next
	inc r20
	rjmp double
	

insertion:
	inc r19
	mov r20, r19
	ld r17, X+
	ld r17, X
	cp r16, r17
	brlo below

above:
	cpi r25, 2
	breq ascending_above

	
decending_above:
	rcall past_turning
	rjmp up

ascending_above:
	mov r21, r24
	rjmp up

below:
	cpi r25, 2
	breq ascending_below


decending_below:
	mov r21, r24
	rjmp down

ascending_below:
	rcall past_turning
	rjmp down

past_turning:
	mov r21, r24

move:
	ld r22, X+
	dec r20
	dec r21
	cpi r21, 0
	breq quick_return
	rjmp move

up:
	ld r18, X
	dec r20
	cpi r20, 0
	breq end
	cp r16, r18
	brlo end
	ld r18, X+
	rjmp up

down:
	ld r18, X
	dec r20
	cpi r20, 0
	breq end
	cp r18, r16
	brlo end
	ld r18, X+
	rjmp down

end:
	ld r23, X
	st X+, r16
	mov r16, r23
	cpi r20, 0
	breq next
	dec r20
	rjmp end
	

	

quick_return:
	ret
	//rjmp insertion


halt:
	rjmp halt
	

