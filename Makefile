CC            := clang
CCFLAGS       := -Wall -Wextra -Wtype-limits -pedantic -std=c17 -g -lm -fsanitize=address
ASSIGNMENT    := a4-csf

.DEFAULT_GOAL := default
.PHONY: default clean bin all run test help


default: help

clean: reset          ## cleans up project folder
	@printf '[\e[0;36mINFO\e[0m] Cleaning up folder...\n'
	rm -f $(ASSIGNMENT)
	rm -f testreport.html
	rm -rf valgrind_logs

bin:                  ## compiles project to executable binary
	@printf '[\e[0;36mINFO\e[0m] Compiling binary...\n'
	$(CC) $(CCFLAGS) -o $(ASSIGNMENT) *.c
	chmod +x $(ASSIGNMENT)
	chmod +x testrunner

reset:                ## resets the reports folder
	@printf "[\e[0;36mINFO\e[0m] Resetting config files...\n"
	rm -f ./images/*.bmp
	cp ./images_ref/* ./images/

all: clean bin        ## all of the above

run: all              ## runs the project
	@printf '[\e[0;36mINFO\e[0m] Executing binary...\n'
	./$(ASSIGNMENT) 20 10

test: all             ## runs public testcases on the project
	@printf '[\e[0;36mINFO\e[0m] Executing testrunner...\n'
	./testrunner

help:                 ## prints the help text
	@printf "Usage: make \e[0;36m<TARGET>\e[0m\n"
	@printf "Available targets:\n"
	@awk -F':.*?##' '/^[a-zA-Z_-]+:.*?##.*$$/{printf "  \033[36m%-10s\033[0m%s\n", $$1, $$2}' $(MAKEFILE_LIST)
