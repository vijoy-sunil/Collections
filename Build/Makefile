# How To?
# make MODULE=X, where 'X' is one of the available modules listed below
# make clean, removes all generated outputs

# add path to all available modules here
BUFFER 		:= ../Common/Buffer
LIBTEST 	:= ../Common/LibTest
TREE		:= ../Common/Tree
LIST 		:= ../Core/List
LOG 		:= ../Core/Log
# save dirs
SAVEDIR 	:= ./LogSaves ./LibTestSaves

TARGET     	= exec_img
CXX        	= clang++
CXXFLAGS   	= -std=c++20 -Wall -Wextra -O0

LD         	= clang++ -o
LDFLAGS    	= -Wall -pedantic

SRCDIR     	= $($(MODULE))/sample
INCDIR		= $($(MODULE))/inc
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
	@$(RM) $(OBJDIR)/* $(BINDIR)/$(TARGET)
	@$(RMDIR) $(SAVEDIR)
	@echo "cleanup complete!"