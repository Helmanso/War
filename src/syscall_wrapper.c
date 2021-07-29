/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 17:15:13 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/17 11:36:13 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"


# define syscall_wrapper(x) syscall_wraper(x);
# define syscall_wraper(x) asm volatile ("mov %rcx, %r10\n\t" \
                                        "mov $" #x ", %rax\n\t" \
                                        "syscall\n\t" \
                                        "leave\n\t" \
                                        "ret");


void    exit_wrapper(int status)
{
    syscall_wrapper(SYS_EXIT);
    __builtin_unreachable();
}

void   write_wrapper(int fd, const void *buf, size_t size)
{
    syscall_wrapper(SYS_WRITE)
    __builtin_unreachable();

}

int	open_wrapper(const char *pathname, int flags, ...)
{
	syscall_wrapper(SYS_OPEN);
	__builtin_unreachable();
}


ssize_t read_wrapper(int fd,void *buf, size_t count)
{
	syscall_wrapper(SYS_READ);
	__builtin_unreachable();
}

int	getdents64_wrapper(unsigned int fd, struct dirent64 *dirp, unsigned int count)
{
	syscall_wrapper(SYS_GETDENTS64);
	__builtin_unreachable();
}

void	*mmap_wrapper(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    syscall_wrapper(SYS_MMAP);
	__builtin_unreachable();
}

int	fstat_wrapper(int fd, struct stat *statbuf)
{
	syscall_wrapper(SYS_FSTAT);
	__builtin_unreachable();
}

int lseek_wrapper(unsigned int fd, off_t offset, unsigned int whence)
{
    syscall_wrapper(SYS_LSEEK);
	__builtin_unreachable();
}

long sys_rt_sigaction (int sig, struct sigaction * act, struct sigaction  * oact)
{
    syscall_wrapper(13)
	__builtin_unreachable();

}

long ptrace_wrapper(enum __ptrace_request request, pid_t pid, void *addr, void *data)
{
    syscall_wrapper(SYS_PTRACE);
    __builtin_unreachable();
}