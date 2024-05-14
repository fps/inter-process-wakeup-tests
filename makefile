LDFLAGS ?= -pthread
CFLAGS ?= -Wall -D_GNU_SOURCE -O3 -march=native

all: i_pipe o_pipe i_cond_shm o_cond_shm o_named_sem i_named_sem i_unnamed_sem o_unnamed_sem i_bare_int o_bare_int
