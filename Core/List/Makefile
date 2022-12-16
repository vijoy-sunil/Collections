TARGET = exec_img
SRC_DIRS = ./sample
CC = clang++

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)
DUMP := *.txt

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++20

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)
	$(RM) $(OBJS) $(DEPS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) $(DUMP)

-include $(DEPS)