;
; AssemblerApplication5.asm
;
; Created: 3/13/2019 7:16:47 PM
; Author : Sean
;

;
; AssemblerApplication3.asm
;
; Created: 3/13/2019 2:17:16 PM
; Author : Sean
;



.set array_size = 5
.set insert_size = 1

.dseg
	list: .byte array_size
.cseg
	original: .db 4,5,7,8,2
  	insert: .db 3
	
	//compare first 2

	//acending
	//if less than current floor iterate through until there is a number less than inserted or end of list
	//if greater than first number iterate through until less than a number or end of list


	//decending
	//if greater than current floor iterate through until there is a number greater than inserted or end of list
	//if less than first number iterate through until greater than a number or end of list
start:
	ldi ZL, low(2*original)
	ldi ZH, high(2*original)
	ldi YL, low(list)
	ldi YH, high(list)
	ldi r20, array_size
	ldi r21, 0 // counter1
	ldi r22, 0 // counter2
	ldi r23, 0 // counts how many numbers are repeated in original array to subtract from array_size

	lpm r16, Z+	
	st Y+, r16		// store first value in program memory into data memory
	lpm r17, Z+

loop_1:				// this loop compares a value already in data memory stored in r16 (largest to smallest) with the new value from program memory
	cpi r20, 1		// when r20 = 1, move onto inserting numbers from the second list
	breq insertion

	cp r16, r17
	breq skip	// r16 = r17

	cp r16, r17 // r16 < r17
	brlt one

	cp r17, r16	// r16 > r17
	brlt two

loop_2:				// stores the value when it is the smallest number that has been processed
	ld r19, -Y		// r19 used as a buffer to increment and decrement Y as necessary
	st Y+, r17		// stores the larger value into the pointer space above the lower value it is being compared to
	rjmp loop_3

loop_3:				// takes the pointer back to the start of the file
	ld r19, Y+
	cpi r19, 0
	breq next

	rjmp loop_3

next:				// loads next number from program memory into r17, moves pointer back to start so that r16 is the largest stored value, inc/dec different counters
	lpm r17, Z+
	ld r19, -Y
	ld r19, -Y
	ld r16, Y+
	dec r20
	inc r21
	mov r22, r21
	rjmp loop_1

one:
	st Y, r17		
	rjmp loop_3

two:
	st Y, r16

	cpi r22, 0
	breq loop_2

	dec r22
	ld r19, -Y
	ld r19, -Y
	ld r16, Y+

	rjmp loop_1

skip:				// when two numbers are equal you need to get rid of the number but also move any numbers that was larger than the number back down one spot in program memory
	ld r19, Y+
	ld r19, Y
	st -Y, r19
	ld r19, Y+
	cpi r19, 0
	brne skip
	
	subi r23, -1
	rjmp next
	
insertion:
	ldi ZL, low(2*insert)	
	ldi ZH, high(2*insert)
	ld r19, -Y
	ldi r19, insert_size
	ldi r21, 0
	ldi r22, 0

	ldi r20, array_size
	cpi r23, 0
	brne dec_array

dec_array:
	dec r20
	dec r23
	cpi r23, 0
	brne dec_array

//now compare
	cp r16,r17
	brlt decending 

acending:
	cp r16,r17
	brlt below_a		//floor is below acending elevator	

above_a:
		
	rjmp above_a
below_a:
	
	rjmp below_a

decending:

	cp r17,r16
	brlt above_d		//floor is above decending elevator

below_d:

	rjmp below_d
above_d: 
	
	rjmp above_d

next_number:
	cpi r19, 0
	breq halt
	lpm r16, Z+
	dec r19
	ldi YL, low(list)
	ldi YH, high(list)

	ld r17, Y+ //pointer
	ldi r18, 0 //position start from 1

checkloop:
	cp r18, r20
	breq trigger 
	cp r16, r17
	breq equal
	cp r16, r17
	brlo trigger
	ld r17, Y+
	inc r18
	rjmp checkloop

equal:
	rjmp next_number

trigger:
	inc r20
	ld r17, -Y

insertloop:
	cp r18, r20
	breq next_number
	ld r17, Y
	st Y+, r16
	mov r16, r17
	inc r18
	rjmp insertloop

halt:
	rjmp halt

