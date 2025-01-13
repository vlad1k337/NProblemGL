CC     := gcc
TARGET := main
SRCDIR := src/
OBJDIR := build/

SRC := $(wildcard $(SRCDIR)*.c)
OBJ := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))

CFLAGS   := -Wall -Wextra -O2 -std=c99 $(shell pkgconf --cflags sdl2 cglm)
CPPFLAGS := -Iinclude/ -Iinclude/glad/
LDFLAGS  := $(shell pkgconf --libs sdl2 cglm)

UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
	CFLAGS  += $(shell pkgconf --cflags opengl)
	LDFLAGS += -lm $(shell pkgconf --libs opengl)
endif
ifeq ($(UNAME), Darwin)
	CFLAGS  += -march=armv8.6-a+simd
	LDFLAGS += -framework OpenGL
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p -v $(OBJDIR)

clean:
	rm -r build

debug: CFLAGS  += -fsanitize=address -ggdb
debug: LDFLAGS += -static-libasan ; ASAN_OPTIONS=detect_leaks=1
debug: $(TARGET)

.PHONY: clean
