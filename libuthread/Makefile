# Target library
lib := libuthread.a
objs := queue.o
CC   := gcc
CFLAGS  := -Wall -Werror 

all: $(lib)

##deps := $(patsubst %.o,%.d,$(objs))
#include $(deps)
#DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a: $(objs)
	##@echo "CC $@"
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	##@echo "CC $@"
	$(CC) $(CFLAGS) -c -o $@ $< # $(DEPFLAGS)

clean:
	##@echo "clean"
	rm -f $(targets) $(objs) # $(deps)




## TODO: Phase 1.1
