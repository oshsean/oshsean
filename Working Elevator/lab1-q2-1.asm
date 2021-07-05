.include "m2560def.inc" 

start:
	ldi r16, low(277)	// numerator
	ldi r17, high(277)
	ldi r18, low(1)	// denominator
	ldi r19, high(1)
	ldi r20, 0			// LOW quotient
	ldi r21, 0			// HIGH QUOTIENT
	ldi r22, 0			// remainder
	ldi r23, 0			// MAY NOT NEED - HIGH OF REMAINDER
	ldi r24, 1			// counter

	// check to see if numerator is zero
	add r20, r16
	adc r20, r17
	cpi r20, 0
	breq halt			// if zero, program ends, result is 0 r 0

	// check to see if denominator is zero
	add r21, r18
	adc r21, r19
	cpi r21, 0
	breq halt			// if zero, to avoid program error, program will end, result is 0 r 0

	ldi r20, 0			// reset parameters
	ldi r21, 0
	
	// check if numerator < denominator
	sub r16, r18
	sbc r17, r19
	brcs remainder			// if numerator < denominator result will be zero with some remainder (carry flag, C = 1)
	add r20, r24 
	
	loop:

		 // NEED SOMETHING HERE IN CASE r20 OVERFLOWS TO CONTINUE TO ADD 1 TO r21???
		sub r16, r18
		sbc r17, r19
		add r20, r24 
		brvs else

		cp r16, r0
		cpc r17, r0
		breq halt		// if numerator = denominator, there will be no remainder, end the program with quptient at current position
	
		cp r16, r18
		cpc r17, r19
		brlt remainder	// if r16/17 < r18/19 then go to remainder section to determine remainder


		rjmp loop		// restart loop here
	

	else:
		sub r16, r18
		sbc r17, r19
		add r21, r24 

		cp r16, r0
		cpc r17, r0
		breq halt		// if numerator = denominator, there will be no remainder, end the program with quptient at current position
	
		cp r16, r18
		cpc r17, r19
		brlt remainder	// if r16/17 < r18/19 then go to remainder section to determine remainder

		rjmp else		// restart loop here
	
	
	remainder:
	// subtract 1 to find remainder
	// as denominator is 8 bit signed integer, the remainder will have to be an 8 bit signed integer. therefore only concerned with lower register (???)
		sub r16, r24
		add r22, r24
		cpi r16, 0
		BREQ halt
		rjmp remainder

	// restart sub loop


halt: 
	rjmp halt ;