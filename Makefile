# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: helmanso <helmanso@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/17 17:51:29 by helmanso          #+#    #+#              #
#    Updated: 2021/07/29 12:50:41 by helmanso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Deathe
OBJ_DIR = obj/
SRC_DIR = src/
FILE = encrypt.c main.c syscall_wrapper.c tools.c do_nothing.c permutation.c inject_self.c register_swap.c disassembler.c increase_segment_sections.c protection.c
OBJ_FILE = $(FILE:.c=.o)
SRC := $(addprefix $(SRC_DIR), $(FILE))
OBJ := $(addprefix $(OBJ_DIR), $(OBJ_FILE))

CC = gcc
CFLAGS = -fno-stack-protector   -fno-builtin -nostdlib -fpic 

all : $(NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC)  $(CFLAGS) -o $@  -c $<
	nasm -f elf64 loader.s -o obj/loader.o

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ obj/loader.o $^ 
	gcc -o /tmp/test/target test.c
	

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Clean done"

fclean: clean
	@rm -rf $(NAME)
	@echo "Fclean done"

re: fclean $(NAME)
