	.file	"math_ops.c"
	.text
	.globl	add                             # -- Begin function add
	.p2align	4
	.type	add,@function
add:                                    # @add
	.cfi_startproc
# %bb.0:
                                        # kill: def $esi killed $esi def $rsi
                                        # kill: def $edi killed $edi def $rdi
	leal	(%rdi,%rsi), %eax
	retq
.Lfunc_end0:
	.size	add, .Lfunc_end0-add
	.cfi_endproc
                                        # -- End function
	.globl	multiply                        # -- Begin function multiply
	.p2align	4
	.type	multiply,@function
multiply:                               # @multiply
	.cfi_startproc
# %bb.0:
	movl	%edi, %eax
	imull	%esi, %eax
	retq
.Lfunc_end1:
	.size	multiply, .Lfunc_end1-multiply
	.cfi_endproc
                                        # -- End function
	.ident	"Debian clang version 21.1.8 (7+b1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
