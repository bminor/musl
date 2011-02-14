#define BUFSIZ 1024

#define FILENAME_MAX 4095
#define FOPEN_MAX 1000
#define TMP_MAX 10000

#define L_cuserid 20
#define L_tmpnam 20

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
#define L_ctermid 20
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
#define P_tmpdir "/tmp"
#endif
