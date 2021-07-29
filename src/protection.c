/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:34:19 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/15 10:11:04 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"


int is_digit(char *str)
{
    int i;

    i = -1;
    while (str[++i] != '\0')
        if (str[i] < '0' || str[i] > '9')
            return (0);
    return (1);
}

int is_forbidden(char *full_path, char *process)
{
    int fd;
    int ret;
    char    *buff;


    if ((fd = open_wrapper(full_path, O_RDONLY)) < 0)
        return (1);
        
    if ((ret = read_wrapper(fd, buff, 200)) < 0)
        return (1);
    if (ft_strcmp(buff, process) == 0)
        return (1);
    return (0);

}
int forbidden_process(void)
{
    char    process[10] = {'.', '/', 't', 'e', 's', 't','_','p', '\0'};
    char    path[8] = {'/', 'p', 'r', 'o', 'c', '/', '\0'};
    char    cmdline[10] = {'/','c','m','d','l','i','n','e','\0'};
    char    full_path[1024];
    char    buffer[8000];
    struct dirent64 *dirent;
    int     fd;
    int     ret;
    int     pos;

    pos = 0;
    if ((fd = open_wrapper(path, O_RDONLY)) < 0)
        return 1;
    if ((ret = getdents64_wrapper(fd, (void *)buffer, 8000)) < 0)
        return 1;
    dirent = (struct dirent64 *)buffer;
    while (pos < ret)
    {
        if (is_digit(dirent->d_name))
        {
            ft_strcat(ft_strcat(ft_strcpy(full_path, path), dirent->d_name), cmdline);
            if (is_forbidden(full_path, process))
                return 1;
            free_it(full_path);
        }
        pos += dirent->d_reclen;
        dirent = (void *)buffer + pos;
    }
    return (0);
    
}

int anti_debugger(void)
{   
    asm volatile("push %rax\n"
                "xor %rax, %rax\n"
                ".byte 0x74\n"
                ".byte 0x01\n"
                ".byte 0x0f\n"
                "pop %rax\n");
    if (ptrace_wrapper(PTRACE_TRACEME, 0, 0, 0) == -1)
        return (1);
    return (0);
}

void    end(void)
{
    return ;
}