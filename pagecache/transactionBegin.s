	.text
	.file	"transactionBegin.c"
	.globl	transactionBegin
	.align	16, 0x90
	.type	transactionBegin,@function
transactionBegin:                       # @transactionBegin
	movq    (%rsp), %r10
    movq	$0, (%rsp)
.LBBb_0:
	cmpq	$10, (%rsp)
	jge	.LBBb_2
	xbegin	.LBBb_1
    movq    %r10, (%rsp)
    ret
.LBBb_1:
	addq	$1, (%rsp)
    leaq	.L.str(%rip), %rdi
	movq	(%rsp), %rsi
	movb	$0, %al
	callq	printf@PLT
	jmp	.LBBb_0
.LBBb_2:
    movq    %r10, (%rsp)
	movl	$4294967295, %edi       # imm = 0xFFFFFFFF
	callq	exit@PLT
.Lfunc_endt:
	.size	transactionBegin, .Lfunc_endt-transactionBegin

	.globl	transactionEndBegin
	.align	16, 0x90
	.type	transactionEndBegin,@function
transactionEndBegin:                       # @transactionEndBegin
	xend
    movq    (%rsp), %r10
    movq	$0, (%rsp)
.LBBe_0:
	cmpq	$10, (%rsp)
	jge	.LBBe_2
	xbegin	.LBBe_1
    movq    %r10, (%rsp)
    ret
.LBBe_1:
	addq	$1, (%rsp)
    leaq	.L.str(%rip), %rdi
	movq	(%rsp), %rsi
	movb	$0, %al
	callq	printf@PLT
	jmp	.LBBe_0
.LBBe_2:
    movq    %r10, (%rsp)
	movl	$4294967295, %edi       # imm = 0xFFFFFFFF
	callq	exit@PLT
.Lfunc_ende:
	.size	transactionBegin, .Lfunc_ende-transactionEndBegin

	.type	.L.str,@object        # @.str
.L.str:
	.asciz	"abort%d\n"
	.size	.L.str, 9