# Module paths
BUFFER 		:= ../Common/Buffer
LIBTEST 	:= ../Common/LibTest
LIST 		:= ../Core/List
LOG 		:= ../Core/Log
# save dirs
SAVEDIR 	:= ./LogSaves ./LibTestSaves

TARGET     	= exec_img
# set module to build
MODULE    	= $(LOG)

CXX        	= clang++
CXXFLAGS   	= -std=c++20 -Wall -Wextra -O0

LD         	= clang++ -o
LDFLAGS    	= -Wall -pedantic

SRCDIR     	= $(MODULE)/sample
INCDIR		= $(MODULE)/inc
OBJDIR     	= ./objs
BINDIR     	= ./

SOURCES   	:= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES  	:= $(wildcard $(INCDIR)/*.h)
OBJECTS   	:= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

RM         	= rm -f
RMDIR		= rm -r -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LD) $@ $(LDFLAGS) $(OBJECTS)
	@echo "linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(RM) $(OBJECTS) $(BINDIR)/$(TARGET)
	@$(RMDIR) $(SAVEDIR)
	@echo "cleanup complete!"