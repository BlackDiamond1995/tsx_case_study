	.text
	.file	"victim.c"
	.globl	test                    # -- Begin function test
	.p2align	4, 0x90
	.type	test,@function
test:                                   # @test
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rbx
	andq	$-16, %rsp
	subq	$80, %rsp
	movq	%rsp, %rbx
	movq	targetfileName@GOTPCREL(%rip), %rdi
	xorl	%esi, %esi
	movb	$0, %al
	callq	open@PLT
	movl	%eax, 68(%rbx)
	cmpl	$-1, 68(%rbx)
	jne	.LBB0_2
# %bb.1:                                # %if.then
	movq	targetfileName@GOTPCREL(%rip), %rsi
	leaq	.L.str(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	jmp	.LBB0_3
.LBB0_2:                                # %if.else
	movq	targetfileName@GOTPCREL(%rip), %rsi
	leaq	.L.str.1(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
.LBB0_3:                                # %if.end
	xorl	%eax, %eax
	movl	%eax, %esi
	movl	68(%rbx), %edi
	movl	$2, %edx
	callq	lseek@PLT
	movq	%rax, 56(%rbx)
	movq	56(%rbx), %rax
	shrq	$12, %rax
                                        # kill: def $eax killed $eax killed $rax
	movl	%eax, 52(%rbx)
	movq	56(%rbx), %rsi
	movl	52(%rbx), %edx
	leaq	.L.str.2(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	xorl	%ecx, %ecx
	movl	%ecx, %esi
	movl	52(%rbx), %ecx
	movl	%ecx, %edi
	movq	%rsp, %r8
	movq	%r8, 40(%rbx)
	movq	%rdi, 32(%rbx)
	movq	56(%rbx), %rdi
	movl	68(%rbx), %r8d
	movq	%rdi, 16(%rbx)          # 8-byte Spill
	movq	%rsi, %rdi
	movq	16(%rbx), %r9           # 8-byte Reload
	movq	%rsi, 8(%rbx)           # 8-byte Spill
	movq	%r9, %rsi
	movl	$1, %ecx
	movl	%ecx, %edx
	movq	8(%rbx), %r9            # 8-byte Reload
	movl	%eax, 4(%rbx)           # 4-byte Spill
	callq	mmap@PLT
	movq	%rax, 24(%rbx)
	movq	40(%rbx), %rax
	movq	%rax, %rsp
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
                                        # -- End function
	.globl	test2                   # -- Begin function test2
	.p2align	4, 0x90
	.type	test2,@function
test2:                                  # @test2
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rbx
	andq	$-16, %rsp
	subq	$80, %rsp
	movq	%rsp, %rbx
	movq	targetfileName@GOTPCREL(%rip), %rdi
	xorl	%esi, %esi
	movb	$0, %al
	callq	open@PLT
	movl	%eax, 68(%rbx)
	cmpl	$-1, 68(%rbx)
	jne	.LBB1_2
# %bb.1:                                # %if.then
	movq	targetfileName@GOTPCREL(%rip), %rsi
	leaq	.L.str(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	jmp	.LBB1_3
.LBB1_2:                                # %if.else
	movq	targetfileName@GOTPCREL(%rip), %rsi
	leaq	.L.str.1(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
.LBB1_3:                                # %if.end
	xorl	%eax, %eax
	movl	%eax, %esi
	movl	68(%rbx), %edi
	movl	$2, %edx
	callq	lseek@PLT
	movq	%rax, 56(%rbx)
	movq	56(%rbx), %rax
	shrq	$12, %rax
                                        # kill: def $eax killed $eax killed $rax
	movl	%eax, 52(%rbx)
	movq	56(%rbx), %rsi
	movl	52(%rbx), %edx
	leaq	.L.str.2(%rip), %rdi
	movb	$0, %al
	callq	printf@PLT
	xorl	%ecx, %ecx
	movl	%ecx, %esi
	movl	52(%rbx), %ecx
	movl	%ecx, %edi
	movq	%rsp, %r8
	movq	%r8, 40(%rbx)
	movq	%rdi, 32(%rbx)
	movq	56(%rbx), %rdi
	movl	68(%rbx), %r8d
	movq	%rdi, 16(%rbx)          # 8-byte Spill
	movq	%rsi, %rdi
	movq	16(%rbx), %r9           # 8-byte Reload
	movq	%rsi, 8(%rbx)           # 8-byte Spill
	movq	%r9, %rsi
	movl	$1, %ecx
	movl	%ecx, %edx
	movq	8(%rbx), %r9            # 8-byte Reload
	movl	%eax, 4(%rbx)           # 4-byte Spill
	callq	mmap@PLT
	movq	%rax, 24(%rbx)
	movq	40(%rbx), %rax
	movq	%rax, %rsp
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end1:
	.size	test2, .Lfunc_end1-test2
                                        # -- End function
	.globl	traverse                # -- Begin function traverse
	.p2align	4, 0x90
	.type	traverse,@function
traverse:                               # @traverse
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-8, %rsp
	subq	$80, %rsp
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	movq	16(%rsp), %rax
	cmpq	$-1, %rax
	jne	.LBB2_2
# %bb.1:                                # %if.then
	jmp	.LBB2_6
.LBB2_2:                                # %if.end
	movq	$3, (%rsp)
.LBB2_3:                                # %for.cond
                                        # =>This Inner Loop Header: Depth=1
	movq	(%rsp), %rax
	movq	8(%rsp), %rcx
	subq	$3, %rcx
	cmpq	%rcx, %rax
	jae	.LBB2_6
# %bb.4:                                # %for.body
                                        #   in Loop: Header=BB2_3 Depth=1
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	addq	$-3, %rax
	movq	%rax, 24(%rsp)
	movq	24(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	addq	$-2, %rax
	movq	%rax, 32(%rsp)
	movq	32(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	addq	$-1, %rax
	movq	%rax, 40(%rsp)
	movq	40(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	48(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	addq	$1, %rax
	movq	%rax, 56(%rsp)
	movq	56(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	addq	$2, %rax
	movq	%rax, 64(%rsp)
	movq	64(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	addq	$3, %rax
	movq	%rax, 72(%rsp)
	movq	72(%rsp), %rcx
	#APP
	movq	(%rcx), %rax

	#NO_APP
# %bb.5:                                # %for.inc
                                        #   in Loop: Header=BB2_3 Depth=1
	movq	(%rsp), %rax
	addq	$1, %rax
	movq	%rax, (%rsp)
	jmp	.LBB2_3
.LBB2_6:                                # %for.end
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end2:
	.size	traverse, .Lfunc_end2-traverse
                                        # -- End function
	.globl	get_targetfile          # -- Begin function get_targetfile
	.p2align	4, 0x90
	.type	get_targetfile,@function
get_targetfile:                         # @get_targetfile
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rbx
	andq	$-16, %rsp
	subq	$80, %rsp
	movq	%rsp, %rbx
	movq	%rdi, 64(%rbx)
	movq	%rsi, 56(%rbx)
	movq	%rdx, 48(%rbx)
	leaq	.L.str.3(%rip), %rdi
	movb	$0, %al
	callq	wrapper_by_compiler_printf@PLT
	xorl	%esi, %esi
	movq	64(%rbx), %rdi
	movl	%eax, 20(%rbx)          # 4-byte Spill
	movb	$0, %al
	callq	wrapper_by_compiler_open@PLT
	movl	%eax, 44(%rbx)
	cmpl	$-1, 44(%rbx)
	jne	.LBB3_2
# %bb.1:                                # %if.then
	movq	64(%rbx), %rsi
	leaq	.L.str(%rip), %rdi
	movb	$0, %al
	callq	wrapper_by_compiler_printf@PLT
	jmp	.LBB3_4
.LBB3_2:                                # %if.else
	movq	64(%rbx), %rsi
	leaq	.L.str.1(%rip), %rdi
	movb	$0, %al
	callq	wrapper_by_compiler_printf@PLT
# %bb.3:                                # %if.end
	xorl	%eax, %eax
	movl	%eax, %esi
	movl	44(%rbx), %edi
	movl	$2, %edx
	callq	wrapper_by_compiler_lseek@PLT
	movq	56(%rbx), %rcx
	movq	%rax, (%rcx)
	movq	56(%rbx), %rax
	movq	(%rax), %rax
	shrq	$12, %rax
                                        # kill: def $eax killed $eax killed $rax
	movl	%eax, 40(%rbx)
	movq	56(%rbx), %rsi
	movl	40(%rbx), %edx
	leaq	.L.str.2(%rip), %rdi
	movb	$0, %al
	callq	wrapper_by_compiler_printf@PLT
	xorl	%edx, %edx
	movl	%edx, %ecx
	movl	40(%rbx), %edx
	movl	%edx, %esi
	movq	%rsp, %rdi
	movq	%rdi, 32(%rbx)
	movq	%rsi, 24(%rbx)
	movq	56(%rbx), %rsi
	movq	(%rsi), %rsi
	movl	44(%rbx), %r8d
	movq	%rcx, %rdi
	movl	$1, %edx
	movl	%edx, 16(%rbx)          # 4-byte Spill
	movl	16(%rbx), %r9d          # 4-byte Reload
	movq	%rcx, 8(%rbx)           # 8-byte Spill
	movl	%r9d, %ecx
	movq	8(%rbx), %r10           # 8-byte Reload
	movq	%r10, %r9
	movl	%eax, 4(%rbx)           # 4-byte Spill
	callq	wrapper_by_compiler_mmap@PLT
	movq	48(%rbx), %rsi
	movq	%rax, (%rsi)
	movq	32(%rbx), %rax
	movq	%rax, %rsp
.LBB3_4:                                # %return
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end3:
	.size	get_targetfile, .Lfunc_end3-get_targetfile
                                        # -- End function
	.globl	load_targetfile         # -- Begin function load_targetfile
	.p2align	4, 0x90
	.type	load_targetfile,@function
load_targetfile:                        # @load_targetfile
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$32, %rsp
	movq	%rdi, 24(%rsp)
	leaq	.L.str.4(%rip), %rdi
	movb	$0, %al
	callq	wrapper_by_compiler_printf@PLT
	movq	24(%rsp), %rdi
	leaq	8(%rsp), %rsi
	leaq	16(%rsp), %rdx
	movl	%eax, 4(%rsp)           # 4-byte Spill
	callq	get_targetfile@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end4:
	.size	load_targetfile, .Lfunc_end4-load_targetfile
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$144, %rsp
	movl	$0, 140(%rsp)
	movl	%edi, 136(%rsp)
	movq	%rsi, 128(%rsp)
	leaq	.L.str.5(%rip), %rdi
	movl	$1, %esi
	movb	$0, %al
	callq	printf@PLT
	movq	preloadStr@GOTPCREL(%rip), %rdi
	movl	%eax, 12(%rsp)          # 4-byte Spill
	movb	$0, %al
	callq	preload@PLT
	cmpl	$1, 136(%rsp)
	jne	.LBB5_2
# %bb.1:                                # %if.then
	movl	$0, 140(%rsp)
	jmp	.LBB5_3
.LBB5_2:                                # %if.end
	leaq	.L.str.6(%rip), %rdi
	movb	$0, %al
	callq	wrapper_by_compiler_printf@PLT
	movq	targetfileName@GOTPCREL(%rip), %rsi
	leaq	16(%rsp), %rdi
	movq	128(%rsp), %rcx
	movq	8(%rcx), %rcx
	movsbl	(%rcx), %edx
	subl	$48, %edx
	movl	%edx, 124(%rsp)
	movl	124(%rsp), %edx
	movl	%eax, 8(%rsp)           # 4-byte Spill
	movb	$0, %al
	callq	wrapper_by_compiler_sprintf@PLT
	leaq	16(%rsp), %rdi
	movl	%eax, 4(%rsp)           # 4-byte Spill
	callq	load_targetfile@PLT
	movb	$0, %al
	callq	stop_tsx@PLT
	leaq	.L.str.6(%rip), %rdi
	movl	%eax, (%rsp)            # 4-byte Spill
	movb	$0, %al
	callq	printf@PLT
	movl	$0, 140(%rsp)
.LBB5_3:                                # %return
	movl	140(%rsp), %eax
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end5:
	.size	main, .Lfunc_end5-main
                                        # -- End function
	.globl	wrapper_by_compiler_open # -- Begin function wrapper_by_compiler_open
	.p2align	4, 0x90
	.type	wrapper_by_compiler_open,@function
wrapper_by_compiler_open:               # @wrapper_by_compiler_open
# %bb.0:                                # %entry
	pushq   %rax
    callq   transactionBegin
    xend
    popq   %rax
    pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$16, %rsp
	movq	%rdi, 8(%rsp)
	movl	%esi, 4(%rsp)
	movq	8(%rsp), %rdi
	movl	4(%rsp), %esi
	xorl	%eax, %eax
                                        # kill: def $al killed $al killed $eax
	callq	open@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end6:
	.size	wrapper_by_compiler_open, .Lfunc_end6-wrapper_by_compiler_open
                                        # -- End function
	.globl	wrapper_by_compiler_printf # -- Begin function wrapper_by_compiler_printf
	.p2align	4, 0x90
	.type	wrapper_by_compiler_printf,@function
wrapper_by_compiler_printf:             # @wrapper_by_compiler_printf
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$16, %rsp
	movq	%rdi, 8(%rsp)
	movq	8(%rsp), %rdi
	xorl	%eax, %eax
                                        # kill: def $al killed $al killed $eax
	callq	printf@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end7:
	.size	wrapper_by_compiler_printf, .Lfunc_end7-wrapper_by_compiler_printf
                                        # -- End function
	.globl	wrapper_by_compiler_lseek # -- Begin function wrapper_by_compiler_lseek
	.p2align	4, 0x90
	.type	wrapper_by_compiler_lseek,@function
wrapper_by_compiler_lseek:              # @wrapper_by_compiler_lseek
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$32, %rsp
	movl	%edi, 28(%rsp)
	movq	%rsi, 16(%rsp)
	movl	%edx, 12(%rsp)
	movl	28(%rsp), %edi
	movq	16(%rsp), %rsi
	movl	12(%rsp), %edx
	callq	lseek@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end8:
	.size	wrapper_by_compiler_lseek, .Lfunc_end8-wrapper_by_compiler_lseek
                                        # -- End function
	.globl	wrapper_by_compiler_mmap # -- Begin function wrapper_by_compiler_mmap
	.p2align	4, 0x90
	.type	wrapper_by_compiler_mmap,@function
wrapper_by_compiler_mmap:               # @wrapper_by_compiler_mmap
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$48, %rsp
	movq	%rdi, 40(%rsp)
	movq	%rsi, 32(%rsp)
	movl	%edx, 28(%rsp)
	movl	%ecx, 24(%rsp)
	movl	%r8d, 20(%rsp)
	movq	%r9, 8(%rsp)
	movq	40(%rsp), %rdi
	movq	32(%rsp), %rsi
	movl	28(%rsp), %edx
	movl	24(%rsp), %ecx
	movl	20(%rsp), %r8d
	movq	8(%rsp), %r9
	callq	mmap@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end9:
	.size	wrapper_by_compiler_mmap, .Lfunc_end9-wrapper_by_compiler_mmap
                                        # -- End function
	.globl	wrapper_by_compiler_traverse # -- Begin function wrapper_by_compiler_traverse
	.p2align	4, 0x90
	.type	wrapper_by_compiler_traverse,@function
wrapper_by_compiler_traverse:           # @wrapper_by_compiler_traverse
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$16, %rsp
	movq	%rdi, 8(%rsp)
	movq	%rsi, (%rsp)
	movq	8(%rsp), %rdi
	movq	(%rsp), %rsi
	callq	traverse@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end10:
	.size	wrapper_by_compiler_traverse, .Lfunc_end10-wrapper_by_compiler_traverse
                                        # -- End function
	.globl	wrapper_by_compiler_sprintf # -- Begin function wrapper_by_compiler_sprintf
	.p2align	4, 0x90
	.type	wrapper_by_compiler_sprintf,@function
wrapper_by_compiler_sprintf:            # @wrapper_by_compiler_sprintf
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$16, %rsp
	movq	%rdi, 8(%rsp)
	movq	%rsi, (%rsp)
	movq	8(%rsp), %rdi
	movq	(%rsp), %rsi
	xorl	%eax, %eax
                                        # kill: def $al killed $al killed $eax
	callq	sprintf@PLT
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end11:
	.size	wrapper_by_compiler_sprintf, .Lfunc_end11-wrapper_by_compiler_sprintf
                                        # -- End function
	.type	targetfileName,@object  # @targetfileName
	.data
	.globl	targetfileName
	.p2align	4
targetfileName:
	.asciz	"/home/liuweijie/Documents/pagecachefiles/targetfile%d"
	.size	targetfileName, 48

	.type	preloadStr,@object      # @preloadStr
	.globl	preloadStr
	.p2align	4
preloadStr:
	.asciz	"/home/liuweijie/Documents/pagecachefiles/targetfile1"
	.size	preloadStr, 47

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%s loading failed\n"
	.size	.L.str, 19

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"%s founded\n"
	.size	.L.str.1, 12

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"targetfile info: size %lldB pages %d\n"
	.size	.L.str.2, 38

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"in get\n"
	.size	.L.str.3, 8

	.type	.L.str.4,@object        # @.str.4
.L.str.4:
	.asciz	"in load\n"
	.size	.L.str.4, 9

	.type	.L.str.5,@object        # @.str.5
.L.str.5:
	.asciz	"HHH%d\n"
	.size	.L.str.5, 7

	.type	.L.str.6,@object        # @.str.6
.L.str.6:
	.asciz	"HHH\n"
	.size	.L.str.6, 5


	.ident	"clang version 9.0.0 (tags/RELEASE_900/final)"
	.section	".note.GNU-stack","",@progbits
