C = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g

INCLUDES = -I includes -I ../libft/

NAME = lemipc

SRCS = src/main.c \
		src/ctx.c \
		src/error.c \
		src/semaphore.c \
		src/message.c \
		src/game_infos.c \
		src/check_death.c \
		src/move.c \
		src/target.c \
		src/exchanges.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

LIBFT = ../libft/libft.a

all: $(LIBFT) $(NAME)

re: fclean all

$(LIBFT):
	@make -C ../libft all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(C) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBFT) -lm

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(C) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make -C ../libft clean
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@make -C ../libft fclean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	./$(NAME) avm/sample.avm

.PHONY: all clean fclean run re

-include $(DEPS)
