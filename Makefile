C = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g

INCLUDES = -I includes -I libft/

NAME = lemipc

SRCS = src/main.c src/ctx.c src/error.c src/semaphore.c src/message.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

LIBFT = libft/libft.a

all: $(LIBFT) $(NAME) visualizer

visualizer:
	@make -C visualizer/ all
	@mv visualizer/lemipc-visualizer .

re: fclean all

$(LIBFT):
	@make -C libft all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(C) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBFT)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(C) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make -C libft clean
	@make -C visualizer clean
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@make -C libft fclean
	@make -C visualizer fclean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)
	@rm -rf lemipc-visualizer

run: $(NAME)
	./$(NAME) avm/sample.avm

.PHONY: all clean fclean run re visualizer

-include $(DEPS)
