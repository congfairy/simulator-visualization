SIMGRID_DIR=/home/congfairy/SimGrid-3.12
CC = gcc -g
CFLAGS = -I$(SIMGRID_DIR)/include
LIBS = -L$(SIMGRID_DIR)/lib/ -lm -lsimgrid

PROG = extracttrace

OBJS = extracttrace.o

all: ${PROG}

%.o: ../common/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LIBS)

clean:
	-rm -f core $(OBJS) ${PROG} *~ 
