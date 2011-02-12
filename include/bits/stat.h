/* copied from kernel definition, but with padding replaced
 * by the corresponding correctly-sized userspace types. */

struct stat
{
	dev_t st_dev;
	int __st_dev_padding;
	long __st_ino_truncated;
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid;
	gid_t st_gid;
	dev_t st_rdev;
	int __st_rdev_padding;
	off_t st_size;
	blksize_t st_blksize;
	blkcnt_t st_blocks;
	time_t st_atime;
	unsigned long __st_atime_nsec;
	time_t st_mtime;
	unsigned long __st_mtime_nsec;
	time_t st_ctime;
	unsigned long __st_ctime_nsec;
	ino_t st_ino;
};
