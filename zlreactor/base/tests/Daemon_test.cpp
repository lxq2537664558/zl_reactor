﻿#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>   // for exit
#include <time.h>
#include <fcntl.h>    // for O_CREAT
#include <string.h>
#include <error.h>
#include "libTitCppKits/base/Daemonize.h"
#include "libTitCppKits/base/Logger.h"
#include "libTitCppKits/base/LogFile.h"

void test_daemon_print_log(int nochdir = 1, int noclose = 0, const char *pidfile = 0)
{
    LOG_INFO("test_daemon_print_log[%d] starting...", ::getpid());

    int pid = zl::base::createDaemonize(nochdir, noclose, pidfile);
    if (pid < 0)
    {
        fprintf(stderr, "create daemon error. exit......\n");
        exit(1);
    }
    else
    {
        printf("create daemon success, pid = [%d]\n", pid);
        assert(pid == ::getpid());
    }


    LOG_INFO("test_daemon_print_log[%d] started...", ::getpid());
	
    // 日志名，日志目录，日志回滚大小，线程安全，flush间隔0s， flush日志条数0条（也即是每次输出都flush）
    zl::base::LogFile* g_logFile = new zl::base::LogFile("Daemon_test", "./", 4000, true, 0, 0); 
    assert(g_logFile);
    LOG_INFO("init log");

    time_t t;
    int fd;
    while (1)
    {
        // fd = open("daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
        // if (fd == -1)
        // {
        //     perror("open daemon.log error");
        //     exit(EXIT_FAILURE);
        // }
        t = time(0);
        char *buf = asctime(localtime(&t));
        //write(fd, buf, strlen(buf));     //向daemon.log文件中写入当前时间
        //printf("[%d] time: %s\n", pid, buf);
        LOG_INFO("[%d][%d] time: %s", pid, ::getpid(), buf);
        //close(fd);
        sleep(3);    // 每隔3s写入一次
    }

    zl::base::exitDaemonize(pidfile);
}

int main()
{
    LOG_INFO("main[%d] starting...", ::getpid());

    //test_daemon_print_log(1, 1, "Daemon_test.pid");  // 不改变当前目录，不重定向输入输出，禁止进程重复启动
    //test_daemon_print_log(1, 0, "Daemon_test.pid");  // 不改变当前目录，重定向输入输出(/dev/null)，禁止进程重复启动
    test_daemon_print_log(1, 0, "Daemon_test.pid");                 // 不改变当前目录，不重定向输入输出(/dev/null)，允许进程重复启动
}
