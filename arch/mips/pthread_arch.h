static inline struct pthread *__pthread_self()
{
#ifdef __clang__
	char *tp;
	__asm__ __volatile__ (".word 0x7c03e83b ; move %0, $3" : "=r" (tp) : : "$3" );
#else
	register char *tp __asm__("$3");
	/* rdhwr $3,$29 */
	__asm__ __volatile__ (".word 0x7c03e83b" : "=r" (tp) );
#endif
	return (pthread_t)(tp - 0x7000 - sizeof(struct pthread));
}

#define TLS_ABOVE_TP
#define TP_ADJ(p) ((char *)(p) + sizeof(struct pthread) + 0x7000)

#define DTP_OFFSET 0x8000

#define MC_PC pc
