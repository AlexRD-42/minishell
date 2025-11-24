# Configuration ------------------------------- #
NAME = minishell
BUILD_PATH = build
INC_PATH = includes
OBJ_PATH = $(BUILD_PATH)/obj
BIN = $(NAME)

# Automatically find all source directories
SRC_DIRS = $(shell find sources -type d)
VPATH = $(SRC_DIRS)

# Files --------------------------------------- #
LIBS =
# Automatically find all .c files in sources directory
SRCS = \
	msh_cd.c msh_echo.c msh_env.c msh_exit.c msh_export.c msh_pwd.c msh_unset.c \
	env_add.c env_core.c env_utils.c history.c \
	dispatch.c exec_cmd.c exec_line.c exec_pipe.c exec_utils.c redirects.c \
	expand_env.c expand_glob.c expand_token.c \
	heredoc.c parsing.c syntax_validation.c tokenizer.c \
	cursor.c handle_arrows.c process_key.c read_input.c utils_read_input.c \
	io_utils.c mem_read.c mem_write.c putstr.c str_cmp.c str_read.c str_write.c syscalls.c main.c \
	init.c

OBJS = $(addprefix $(OBJ_PATH)/, $(SRCS:.c=.o))

# Flags --------------------------------------- #
CC = cc
CFLAGS = -Wall -Wextra $(addprefix -I,$(INC_PATH)) -Werror
LFLAGS =
DEBUG = -g -Wpedantic -Wcast-qual -Wfloat-equal -Wswitch-default -Wduplicated-branches -Wduplicated-cond -Wsign-conversion
SANITIZERS = -fsanitize=address,undefined,leak -fno-omit-frame-pointer
FAST = -march=native -O3 -ffast-math

# Pattern Rule -------------------------------- #
$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

# Linking Rule -------------------------------- #
$(NAME): $(OBJS) | $(BUILD_PATH)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

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
