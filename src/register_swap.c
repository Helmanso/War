/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_swap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 13:06:31 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/28 18:52:10 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

#define RAX 0b00000000
#define RCX 0b00000001
#define RDX 0b00000010
#define RBX 0b00000011
#define RSI 0b00000110
#define RDI 0b00000111


void swap_gp_ext(t_instructions instructions[256], unsigned char from, unsigned char to, int size)
{

    int i;
    unsigned char pad;
    unsigned char rex;

    i = 0;

    rex = 0;
    pad = 0;
    while (i < size)
    {   
        rex = instructions[i].rex & 0b00000101;
        if (instructions[i].rex != 0 &&  rex != 0)
        {
            if (mixed_opcode(instructions[i].op))
            {
                pad = instructions[i].op & 0b11111000;
                if ((instructions[i].op & 0b00000111) == from)
                    instructions[i].op = pad | to;
                else if ((instructions[i].op & 0b00000111) == to)
                    instructions[i].op = pad | from;
            }
         
            if ((rex & 0b00000100) == 0b00000100)
            {
                if (instructions[i].src == from)
                    instructions[i].src = to;
                else if (instructions[i].src == to)
                    instructions[i].src = from;
            }
            if ((rex & 0b00000001) == 0b00000001)
            {
                if (instructions[i].dst == from)
                    instructions[i].dst = to;
                else if (instructions[i].dst == to)
                    instructions[i].dst = from;
            }
        }
        i++;
    }
}

void swap_gp(t_instructions instructions[256], unsigned char from, unsigned char to, int size)
{
    int i;
    unsigned char rex;
    unsigned char pad;
    unsigned char scale;
    unsigned char index;
    unsigned char base;

    i = 0;
    while (i < size)
    {
        rex = instructions[i].rex & 0b00000101;
        if (branching_opcode(instructions[i].op))
            {i++;continue;}
        if (mixed_opcode(instructions[i].op) && instructions[i].op != 0xc3)
        {
            pad = instructions[i].op & 0b11111000;
            if ((instructions[i].op & 0b00000111) == from)
                instructions[i].op = pad | to;
            else if ((instructions[i].op & 0b00000111) == to)
                instructions[i].op = pad | from;        
        }
        if (instructions[i].sib != 0)
        {
            scale = instructions[i].sib & 0b11000000;
            index = instructions[i].sib & 0b00111000;
            base =  instructions[i].sib & 0b00000111;
            
            if (base == from)
                base = to;
            if ((index >> 3) == from)
                index = (to << 3);
            if (instructions[i].src == from)
                instructions[i].src = to;
            instructions[i].sib = scale | index | base;
        }
        else if (((rex & 0b00000001) == 0b00000000 || (rex & 0b00000100) == 0b00000000) && !mixed_opcode(instructions[i].op) && instructions[i].op != 0xc3)
        {
            if ((rex & 0b00000100) == 0b00000000)
            {
                if (instructions[i].src == from)
                    instructions[i].src = to;
                else if (instructions[i].src == to)
                    instructions[i].src = from;
            }
            if ((rex & 0b00000001) == 0b00000000)
            {
                if (instructions[i].dst == from)
                    instructions[i].dst = to;
                else if (instructions[i].dst == to)
                    instructions[i].dst = from;
            }
        }
        i++;
    }
}

void register_swap(t_instructions instructions[256], int size, t_seed *seed)
{
    int i;
    unsigned char regs[] = {RCX, RDX, RDI, RBX, RSI};
    int from;
    int to;
    int nswap;


    i = 0;
   
    while (i < 1)
    {
        ps_random(seed);
        from = seed->w % sizeof(regs);
        ps_random(seed);
        to = seed->w % sizeof(regs);
       // from != to ? swap_gp_ext(instructions, regs[from], regs[to], size) : (void )nswap;
        from != to ? swap_gp(instructions, regs[from], regs[to], size) : (void )nswap;
        i++;
    }
}
