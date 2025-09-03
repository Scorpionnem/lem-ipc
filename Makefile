C = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP

INCLUDES = -I includes

NAME = lemipc

SRCS = src/main.c src/ctx.c src/game.c src/game_checks.c src/render.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

all: $(NAME)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(C) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(C) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	./$(NAME) avm/sample.avm

.PHONY: all clean fclean run re

-include $(DEPS)
