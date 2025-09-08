C = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g

INCLUDES = -I includes/ -I ../libft/ -I minilibx-linux/

NAME = lemipc-visualizer

SRCS = src/main.c src/ctx.c src/error.c src/semaphore.c src/render.c src/game_infos.c src/strings.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

LIBFT = ../libft/libft.a
MLX = minilibx-linux/libmlx.a

all: $(LIBFT) $(MLX) $(NAME)

re: fclean all

$(LIBFT):
	@make -C ../libft all

$(MLX):
	@make -C minilibx-linux all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(C) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBFT) $(MLX) -lXext -lX11 -lm -lz

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(C) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make -C ../libft clean
	@make -C minilibx-linux clean
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
