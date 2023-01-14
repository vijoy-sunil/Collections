# How To?
# (1) build a single module
# make <module name>
#
# (2) build all modules
# make all
#
# (3) remove all generated objects and files
# make clean
#
# (4) run a module
# make run module=<module name>
#
# (5) run all modules
# make run_all

# add module paths here
ALLPATHS		= ./Common/Buffer		\
				  ./Common/LibTest		\
				  ./Common/Tree			\
				  ./Core/BTree			\
				  ./Core/List			\
				  ./Core/Log

# do not edit below here
BINFMT			= _exe
BINDIR     		= ./Build/Bin
SAVEDIR			= ./Build/Save

SRCDIR     		:= $(MODULEPATH)/sample
INCDIR			:= $(MODULEPATH)/inc
OBJDIR     		:= $(BINDIR)/Objs
ALLMODULES		:= $(foreach path,$(ALLPATHS),$(notdir $(path)))
ALLBINS			:= $(foreach module,$(ALLMODULES),$(addsuffix $(BINFMT),$(module)))
SOURCES   		:= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES  		:= $(wildcard $(INCDIR)/*.h)
OBJECTS   		:= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CXX        		= clang++
CXXFLAGS   		= -std=c++20 -Wall -Wextra -O0
LD         		= clang++ -o
LDFLAGS    		= -Wall -pedantic
RM         		= rm -f
RMDIR			= rm -r -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LD) $@ $(LDFLAGS) $(OBJECTS)
	@echo "[OK] link"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[OK] compile"

# targets
$(ALLMODULES):
	make MODULEPATH=$(wildcard ./*/$@) TARGET=$(addsuffix $(BINFMT),$@)

all:
	for module in $(ALLMODULES) ; do	\
		make $$module ;					\
	done

.PHONY: clean
clean:
	@$(RM) $(OBJDIR)/* $(BINDIR)/*_exe
	@$(RMDIR) $(SAVEDIR)/*
	@echo "[OK] clean"

run:
	$(BINDIR)/$(addsuffix $(BINFMT),$(module))

run_all:
	for bin in $(ALLBINS) ; do			\
		$(BINDIR)/$$bin ;				\
	done