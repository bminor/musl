/* copied from kernel definition, but with padding replaced
 * by the corresponding correctly-sized userspace types. */

struct stat {
	unsigned long st_dev;
	ino_t st_ino;
	nlink_t st_nlink;

	mode_t st_mode;
	uid_t st_uid;
	gid_t st_gid;
	unsigned int    __pad0;
	dev_t st_rdev;
	off_t st_size;
	blksize_t st_blksize;
	blkcnt_t st_blocks;

	time_t st_atime;
	unsigned long st_atime_nsec;
	time_t st_mtime;
	unsigned long st_mtime_nsec;
	time_t st_ctime;
	unsigned long st_ctime_nsec;
	long __unused[3];
};
