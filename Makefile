all: player visualizer

visualizer:
	@make -C visualizer/ all
	@mv visualizer/lemipc-visualizer .

player:
	@make -C player/ all
	@mv player/lemipc .

re: fclean all

clean:
	@make -C visualizer clean
	@make -C player clean

fclean: clean
	@make -C visualizer fclean
	@make -C player fclean
	@rm -rf lemipc
	@rm -rf lemipc-visualizer

.PHONY: all clean fclean re visualizer player

-include $(DEPS)
