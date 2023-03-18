.entry LIST
.extern W
MAIN: 	add r3, LIST
LOOP: 	prn #48
		lea STR, r6
		inc r6
		mov r3, W
		sub r1, r4
		bne END
		cmp val1, #-6
		bne END
		dec K
.entry MAIN
		sub r10 ,r14
		END: stop
STR: 	.string "abcd"
LIST: 	.data 6, -9
		.data -100,22 , 33 ,44
		.entry K
K: 		.data 31
.extern val1