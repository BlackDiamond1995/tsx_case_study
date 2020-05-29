	.text
	.file	"transactionBegin.c"
	.globl	transactionBegin
	.align	16, 0x90
	.type	transactionBegin,@function
transactionBegin:                       # @transactionBegin
	movq    (%rsp), %r10
    movq	$0, (%rsp)
.LBBt_0:
	cmpq	$10, (%rsp)
	jge	.LBBt_2
	xbegin	.LBBt_1
    movq    %r10, (%rsp)
    ret
.LBBt_1:
	addq	$1, (%rsp)
    leaq	.L.str(%rip), %rdi
	movq	(%rsp), %rsi
	movb	$0, %al
	callq	printf@PLT
	jmp	.LBBt_0
.LBBt_2:
	movq    %r10, (%rsp)
    movl	$4294967295, %edi       # imm = 0xFFFFFFFF
	callq	exit@PLT
.Lfunc_endt:
	.size	transactionBegin, .Lfunc_endt-transactionBegin

    .type	.L.str,@object        # @.str
.L.str:
	.asciz	"abort%d\n"
	.size	.L.str, 9