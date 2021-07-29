/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permutation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 13:51:52 by helmanso          #+#    #+#             */
/*   Updated: 2021/07/27 16:51:28 by helmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pestilence.h"

// op R1, R2
// op R3, R4

void    permutate(t_instructions instructions[256], int first, int second)
{
    t_instructions tmp;

    tmp = instructions[first];
    instructions[first] = instructions[second];
    instructions[second] = tmp;
}

int    can_permutate(t_instructions first, t_instructions second)
{
    unsigned char first_src;
    unsigned char first_dst;
    unsigned char second_src;
    unsigned char second_dst;
    unsigned char index;

    if(PUSH(first.op) || POP(first.op) || PUSH(second.op) || POP(second.op))
        return (0);
    if (first.op == 0xc3 || second.op == 0xc3)
        return (0);
    if (branching_opcode(first.op) || branching_opcode(second.op))
        return (0);

    first_src = first.src;
    first_dst = first.dst;
    second_src = second.src;
    second_dst = second.dst;

    if (first.sib != 0 || second.sib != 0)
        return (0);
    if (first.op & 0b00000010)
    {
        first_src = first.dst;
        first_dst = first.src;
    }
    if (second.op & 0b00000010)
    {
        second_src = second.dst;
        second_dst = second.src;
    }

    if (first_src != second_src && first_src != second_dst && first_dst != second_src)
        return (1);
    return (0);

}

void    instruction_permuation(t_instructions instructions[256], int code_size, t_seed *seed)
{
    int i;
    int random;

    i = 0;
    while (i < 500)
    {
        ps_random(seed);
        random = seed->w % code_size;
        if (can_permutate(instructions[random], instructions[random + 1]))
            permutate(instructions, random, random + 1);
        i++;
    }
    
}
