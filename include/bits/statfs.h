struct statvfs {
	unsigned long f_type;
	unsigned long f_bsize;
	fsblkcnt_t f_blocks;
	fsblkcnt_t f_bfree;
	fsblkcnt_t f_bavail;
	fsfilcnt_t f_files;
	fsfilcnt_t f_ffree;
	unsigned long f_fsid;
	unsigned long __unused;
	unsigned long f_namemax;
	unsigned long f_frsize;
	fsfilcnt_t f_favail;
	unsigned long f_flag;
	unsigned long __reserved[2];
};
