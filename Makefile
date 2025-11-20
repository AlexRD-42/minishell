# Configuration ------------------------------- #
NAME = main
BUILD_PATH = build
INC_PATH = includes
OBJ_PATH = $(BUILD_PATH)/obj
BIN = $(BUILD_PATH)/$(NAME)
VPATH = sources sources/utils sources/env sources/commands sources/core sources/exec sources/history \
sources/read_input sources/history

# Files --------------------------------------- #
LIBS =
SRCS = mem_read.c mem_write.c str_read.c str_write.c str_cmp.c io_utils.c syscalls.c \
test2.c
# main.c history.c cursor.c handle_arrows.c process_key.c interrupts.c
# exec.c heredoc.c main.c pipe.c
# msh_cd.c msh_dispatch.c msh_echo.c msh_env.c msh_exit.c msh_export.c msh_pwd.c msh_unset.c

OBJS = $(addprefix $(OBJ_PATH)/, $(SRCS:.c=.o))

# Flags --------------------------------------- #
CC = clang
CFLAGS = -Wall -Wextra $(addprefix -I,$(INC_PATH)) -flto -fstrict-aliasing
LFLAGS =
DEBUG = -g -Wpedantic -Wcast-qual -Wfloat-equal -Wswitch-default -Wduplicated-branches -Wduplicated-cond -Wsign-conversion
SANITIZERS = -fsanitize=address,undefined,leak -fno-omit-frame-pointer
FAST = -march=native -O3 -ffast-math

# Pattern Rule -------------------------------- #
$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

# Linking Rule -------------------------------- #
$(BIN): $(OBJS) | $(BUILD_PATH)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS) $(LFLAGS)

# Directory Rule ------------------------------ #
$(OBJ_PATH):
	@mkdir -p $@
$(BUILD_PATH):
	@mkdir -p $@

# Phonies ------------------------------------- #
all: $(BIN)

debug: CFLAGS += $(DEBUG) $(SANITIZERS)
debug: clean $(BIN)

fast: CFLAGS += $(FAST)
fast: clean $(BIN)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(BIN)

re: fclean all

.PHONY: all clean fclean re fast debug
