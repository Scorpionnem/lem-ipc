all: player visualizer

visualizer:
	@make -C visualizer/ all --no-print-directory
	@cp visualizer/lemipc-visualizer .

player:
	@make -C player/ all --no-print-directory
	@cp player/lemipc .

re: fclean all

clean:
	@make -C visualizer clean --no-print-directory
	@make -C player clean --no-print-directory

fclean: clean
	@make -C visualizer fclean --no-print-directory
	@make -C player fclean --no-print-directory
	@rm -rf lemipc
	@rm -rf lemipc-visualizer

.PHONY: all clean fclean re visualizer player

-include $(DEPS)
