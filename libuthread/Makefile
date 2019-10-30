# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o preempt.o
CC   := gcc
CC2  := ar rcs
CFLAGS  := -Wall -Werror

ifneq ($(V),1)
Q = @
endif

all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a: $(objs)
	@echo "CC $@"
	$(Q)$(CC2) -o $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	@echo "clean"
	$(Q)rm -f $(targets) $(objs) $(deps)




