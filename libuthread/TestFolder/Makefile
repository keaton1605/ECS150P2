# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o 
CC   := gcc
CC2  := ar rcs
CFLAGS  := -g -Wall#-Werror 

all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a: $(objs)
	#@echo "CC $@"
	$(CC2) -o $@ $^

%.o: %.c
	#@echo "CC $@"
	$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	#@echo "clean"
	rm -f $(targets) $(objs) $(deps)




