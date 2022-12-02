CC := gcc
CEXTRAFLAGS := \
	-Wall \
	-Wextra \
	-Wfloat-equal \
	-Wundef \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wstrict-overflow=5 \
	-Wwrite-strings \
	-Wcast-qual \
	-Wswitch-default \
	-Wswitch-enum \
	-Wconversion \
	-Wunreachable-code
#	-Waggregate-return \

CFLAGS := -O0 -std=c11 $(CEXTRAFLAGS)
DEBUGFLAGS := -g -D_DEBUG
CLIBFLAG := -c

OUTDIR := build

INCLUDE := .
SRC	:= .

EXECUTABLE := $(OUTDIR)/main.exe
EXEC_SOURCE := $(wildcard *.c)
SOURCES := $(wildcard $(SRC)/*.c)
INCLUDES := $(wildcard $(INCLUDE)/*.h)

OBJECTS := $(patsubst $(SRC)/%.c, $(OUTDIR)/%.o, $(SOURCES))


.PHONY: build-debug clean all run run-debug

all: build-debug

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o: $(SRC)/%.c
	$(CC) $(CLIBFLAG) -I$(INCLUDE) $(CFLAGS) -o $@ $<

build-debug: CFLAGS += $(DEBUGFLAGS)
build-debug: $(OUTDIR) $(EXECUTABLE)
	@echo EXE: $(EXECUTABLE)
	@echo SOURCES: $(SOURCES)
	@echo INCLUDES: $(INCLUDES)
	@echo OBJECTS: $(OBJECTS)


$(OUTDIR):
	mkdir $(OUTDIR)

clean:
	del $(OUTDIR)

run:
	./$(EXECUTABLE)

run-debug:
	gdb $(EXECUTABLE)