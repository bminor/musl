struct semid_ds {
	struct ipc_perm sem_perm;
	time_t sem_otime;
	time_t sem_ctime;
	time_t sem_nsems;
	time_t __unused3;
	time_t __unused4;
};
