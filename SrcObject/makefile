
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

.PHONY: all clean

all:myTest

-include $(DEPS)

myTest:$(OBJS)
	@echo "-----link success------"
	gcc $^ -o $@

%.o:%.c
	gcc -c -g -Wall $< -o $@ -MD -MF $*.d -MP


clean:
	-rm -f *.o *.d myTest 
