# Target library
lib 	:= libuthread.a
targets	:= $(lib)
objs	:= queue.o uthread.o context.o preempt.o

CC	:= gcc
CFLAGS	:= -Wall -Werror 
CFLAGS  += -g

LIBBUILD := ar rcs

all: $(targets)

deps	:= $(patsubt %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

$(lib): $(objs)
	$(LIBBUILD) $@ $(objs)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	rm -f $(targets) $(objs) $(deps) *.d

