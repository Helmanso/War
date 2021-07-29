/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disassembler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 11:04:06 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/28 17:00:39 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"




int    mixed_rax(unsigned char opcode)
{
    if (opcode == 0x04 || opcode == 0x05 || opcode == 0x2D || opcode == 0x2C || opcode == 0x35
        || opcode == 0x34 || opcode == 0x3D || opcode == 0x3C)
            return (1);
    return (0);
}

int mixed_opcode(unsigned char opcode)
{
    if (PUSH(opcode) || POP(opcode) || MOV_IMM64(opcode) || opcode == 0xc3)
        return (1);
   
    return (0);

}

int branching_opcode(unsigned char opcode)
{
    if (opcode == JMP64 || opcode == CALL)
        return (1);
    if (opcode == JMP8 || JE_FAMILY8(opcode))
        return (2);
    return (0);
}

int disass(unsigned char *mapped, int i, t_instructions instructions[256])
{
    unsigned char opcode;
    int             inst_length;


    instructions[i].rex = 0;
    instructions[i].op = 0;
    instructions[i].mod = 0;
    instructions[i].src = 0;
    instructions[i].dst = 0;
    instructions[i].sib = 0;
    instructions[i].imm[0] = 0;
    instructions[i].imm[1] = 0;
    instructions[i].imm[2] = 0;
    instructions[i].imm[3] = 0;
    inst_length = 0;
    label:
        opcode = *mapped++;
       
        inst_length++;

        if (opcode == 0x26 || opcode == 0x2e
	        ||  opcode == 0x36 || opcode == 0x3e
	        ||  opcode == 0x64 || opcode == 0x65 || opcode == 0x66 || opcode == 0x67
	        ||  opcode == 0xf0)
            {goto label;}
        else if (opcode >= 0x40 && opcode <= 0x4f)
            {instructions[i].rex=opcode;goto label;}


        if (mixed_rax(opcode))
            {instructions[i].op = opcode;ft_memcpy(instructions[i].imm, mapped, 4);inst_length +=4;}
        else if (mixed_opcode(opcode))
        {
            instructions[i].op = opcode;
            if (MOV_IMM64(opcode))
                {ft_memcpy(instructions[i].imm, mapped, 4);inst_length+=4;}
        }
        else if (branching_opcode(opcode))
        {
            instructions[i].op = opcode;
            if (branching_opcode(opcode) == 1)
                {ft_memcpy(instructions[i].imm, mapped, 4);inst_length+=4;}
            else
                {ft_memcpy(instructions[i].imm, mapped, 1);inst_length++;}
        }
        else
        {
            instructions[i].op = opcode;
            instructions[i].mod = (*mapped & 0xC0) >> 6;
            instructions[i].src = (*mapped & 0x38) >> 3;
            instructions[i].dst = (*mapped & 0x07);
            opcode = *(++mapped);
            inst_length++;
        
            if (instructions[i].mod == 0b0000)
            {
                if (instructions[i].dst == 0b0101 || instructions[i].dst == 0b0100)
                    {instructions[i].sib = opcode;inst_length++;}

            }
            else if (instructions[i].mod == 0b0001)
                {ft_memcpy(instructions[i].dis, mapped, 1);inst_length++;}
            else if (instructions[i].mod == 0b0010)
                {ft_memcpy(instructions[i].dis, mapped, 4);inst_length +=4;}
            else if (instructions[i].mod == 0b0011)
            {
                if (instructions[i].op == OP_IMM8)
                    {ft_memcpy(instructions[i].imm, mapped, 1);inst_length++;}
                else if (instructions[i].op == OP_IMM64)
                    {ft_memcpy(instructions[i].imm, mapped, 4);inst_length+=4;}
                    
            }
        }
        
    return (inst_length);
    
}

void print_arr(unsigned char imm[4], int type, int fd)
{
    int i;
    
    i = -1;
    while (++i < type)
        write_wrapper(fd, &imm[i], 1);
}

int op_c(void *mapped, int size, int fd, t_seed *seed)
{
    int i;
    int inst_length;
    int code_size = size;
    t_instructions instructions[256];
    unsigned char modrm;
    unsigned char *code = mapped;
    int random;
    
    i = 0;
    inst_length = 0;
    while (code_size)
    {
        inst_length = disass(code, i, instructions);
        code += inst_length;
        code_size -= inst_length;
        i++;
    }
    
    code_size = i;
    //instruction_permuation(instructions, code_size, seed);
    register_swap(instructions, code_size, seed);
    i = 0;
    while (i < code_size)
    {
        ps_random(seed);
        random = seed->w;
        if (random % 80 == 0)
            do_nothing(fd);
        else
        {
            if (instructions[i].rex != 0)
                write_wrapper(fd, &instructions[i].rex, 1);
            if (instructions[i].op != 0)
                write_wrapper(fd, &instructions[i].op, 1);
            modrm = (instructions[i].mod << 6) | (instructions[i].src << 3) | instructions[i].dst;
            if (modrm != 0)
                write_wrapper(fd, &modrm, 1);
            if (instructions[i].sib != 0)
                write_wrapper(fd, &instructions[i].sib, 1);
            if (MOV_IMM64(instructions[i].op) || instructions[i].op == OP_IMM64 || branching_opcode(instructions[i].op) == 1 || mixed_rax(instructions[i].op))
                print_arr(instructions[i].imm, 4, fd);
            if (branching_opcode(instructions[i].op) == 2 ||  instructions[i].op ==  OP_IMM8)
                print_arr(instructions[i].imm, 1, fd);
            if (instructions[i].mod == 0b001)
                print_arr(instructions[i].dis, 1, fd);
            if (instructions[i].mod == 0b010)
                print_arr(instructions[i].dis, 4, fd);
            i++;
        }
    }
    
}