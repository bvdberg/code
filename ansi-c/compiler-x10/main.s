	.file	"main.cpp"
	.text
	.align 2
.globl _Z7times10i
	.type	_Z7times10i, @function
_Z7times10i:
.LFB5:
	pushl	%ebp
.LCFI0:
	movl	%esp, %ebp
.LCFI1:
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	%eax
	leave
	ret
.LFE5:
	.size	_Z7times10i, .-_Z7times10i
	.align 2
.globl _Z8times100i
	.type	_Z8times100i, @function
_Z8times100i:
.LFB6:
	pushl	%ebp
.LCFI2:
	movl	%esp, %ebp
.LCFI3:
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	addl	%edx, %eax
	sall	$2, %eax
	leave
	ret
.LFE6:
	.size	_Z8times100i, .-_Z8times100i
	.section	.rodata
.LC0:
	.string	"result = %d\n"
	.text
	.align 2
.globl main
	.type	main, @function
main:
.LFB7:
	pushl	%ebp
.LCFI4:
	movl	%esp, %ebp
.LCFI5:
	subl	$8, %esp
.LCFI6:
	andl	$-16, %esp
	movl	$0, %eax
	addl	$15, %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	subl	%eax, %esp
	movl	12(%ebp), %eax
	addl	$4, %eax
	subl	$12, %esp
	pushl	(%eax)
.LCFI7:
	call	atoi
	addl	$16, %esp
	pushl	%eax
.LCFI8:
	call	_Z7times10i
	addl	$4, %esp
	movl	%eax, -4(%ebp)
	subl	$8, %esp
	pushl	-4(%ebp)
	pushl	$.LC0
.LCFI9:
	call	printf
	addl	$16, %esp
	movl	$0, %eax
	leave
	ret
.LFE7:
	.size	main, .-main
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zP"
	.uleb128 0x1
	.sleb128 -4
	.byte	0x8
	.uleb128 0x5
	.byte	0x0
	.long	__gxx_personality_v0
	.byte	0xc
	.uleb128 0x4
	.uleb128 0x4
	.byte	0x88
	.uleb128 0x1
	.align 4
.LECIE1:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB7
	.long	.LFE7-.LFB7
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB7
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI7-.LCFI5
	.byte	0x2e
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0x2e
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0x2e
	.uleb128 0x10
	.align 4
.LEFDE5:
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.2 20041017 (Red Hat 3.4.2-6.fc3)"
