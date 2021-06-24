#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void sigttouhandler(int signum)
{
	// 首先通过handler接收, 看收到的信号是哪个, 然后在里面设置父进程为前台进程组
	int ppid = getpid();
	printf("pid: %d SIGtou received %d.\n", ppid, signum);
	tcsetpgrp(0,ppid);
	sleep(1);
	return;
}

int main()
{
	int cpid; /* 保存子进程的id号 */
	int ppid; /* 保存父进程的id号 */
	char buf[256];

	char *prog1_argv[2];
	prog1_argv[0] = "/usr/bin/vi"; /* 命令ls的参数表 */
	prog1_argv[1] = NULL;

	ppid = getpid(); //得到进程号
	cpid = fork();

	if (cpid < 0)
		exit(-1);

	if (!cpid)
	{
		fprintf(stdout, "ID(child)=%d\n", getpid());

		/* 使子进程所在的进程组成为前台进程组，然后执行vi */
		setpgid(0, 0);
		tcsetpgrp(0, getpid());
		int returnvalue = 0;
		returnvalue = execvp(prog1_argv[0], prog1_argv);
		perror("exec vi");
		exit(-1);
	}
	// signal(SIGTTOU, SIG_IGN);
	fprintf(stdout, "ID(parent)=%d\n", ppid);
	setpgid(cpid, cpid);	/* 设置进程组 */
	tcsetpgrp(0, cpid);		/* 设置控制终端为子进程拥有 */
	waitpid(cpid, NULL, 0); /* 父进程等待子进程执行完毕，所在进程组成为前台进程组 */
	signal(SIGTTOU, sigttouhandler);
	tcsetpgrp(0, ppid);


	//父进程等待终端输入，然后回显
	while (1)
	{
		memset(buf, 0, 256);
		fgets(buf, 256, stdin);
		puts("ECHO: ");
		puts(buf);
	}

	return 0;
}