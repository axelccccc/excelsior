.section .init
	/* gcc's crtend.o's .init section */
	popl %ebp
	ret

.section .fini
	/* gcc's crtend.o's .fini section */
	popl %ebp
	ret