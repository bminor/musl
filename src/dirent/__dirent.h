struct __DIR_s
{
	int lock;
	int fd;
	off_t tell;
	int buf_pos;
	int buf_end;
	char buf[2048];
};
