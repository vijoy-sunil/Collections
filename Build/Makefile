# Module paths
BUFFER_SRC 	:= ../Common/Buffer/sample
LIBTEST_SRC := ../Common/LibTest/sample
LIST_SRC 	:= ../Common/List/sample
LOG_SRC 	:= ../Common/Log/sample

# set module src to build
SRC_DIRS = $(BUFFER_SRC)

TARGET = exec_img
CC = clang++

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

SAVE_DIR := ./LogSaves ./LibTestSaves
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++20

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)
	$(RM) $(OBJS) $(DEPS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)
	$(RM) -r $(SAVE_DIR)

-include $(DEPS)