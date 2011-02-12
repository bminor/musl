#ifndef _LINUX_LOOP_H
#define _LINUX_LOOP_H

#include <stdint.h>

#define LO_NAME_SIZE	64
#define LO_KEY_SIZE	32

enum {
	LO_FLAGS_READ_ONLY	= 1,
	LO_FLAGS_USE_AOPS	= 2,
	LO_FLAGS_AUTOCLEAR	= 4,
};

struct loop_info {
	int                lo_number;
	long               lo_device;
	unsigned long	   lo_inode;
	long               lo_rdevice;
	int		   lo_offset;
	int		   lo_encrypt_type;
	int		   lo_encrypt_key_size;
	int		   lo_flags;
	char		   lo_name[LO_NAME_SIZE];
	unsigned char	   lo_encrypt_key[LO_KEY_SIZE];
	unsigned long	   lo_init[2];
	char		   reserved[4];
};

struct loop_info64 {
	uint64_t	   lo_device;
	uint64_t	   lo_inode;
	uint64_t	   lo_rdevice;
	uint64_t	   lo_offset;
	uint64_t	   lo_sizelimit;
	uint32_t	   lo_number;
	uint32_t	   lo_encrypt_type;
	uint32_t	   lo_encrypt_key_size;
	uint32_t	   lo_flags;
	uint8_t		   lo_file_name[LO_NAME_SIZE];
	uint8_t		   lo_crypt_name[LO_NAME_SIZE];
	uint8_t		   lo_encrypt_key[LO_KEY_SIZE];
	uint64_t	   lo_init[2];
};


#define LO_CRYPT_NONE		0
#define LO_CRYPT_XOR		1
#define LO_CRYPT_DES		2
#define LO_CRYPT_FISH2		3
#define LO_CRYPT_BLOW		4
#define LO_CRYPT_CAST128	5
#define LO_CRYPT_IDEA		6
#define LO_CRYPT_DUMMY		9
#define LO_CRYPT_SKIPJACK	10
#define LO_CRYPT_CRYPTOAPI	18
#define MAX_LO_CRYPT		20


#define LOOP_SET_FD		0x4C00
#define LOOP_CLR_FD		0x4C01
#define LOOP_SET_STATUS		0x4C02
#define LOOP_GET_STATUS		0x4C03
#define LOOP_SET_STATUS64	0x4C04
#define LOOP_GET_STATUS64	0x4C05
#define LOOP_CHANGE_FD		0x4C06
#define LOOP_SET_CAPACITY	0x4C07

#endif
