
PROGS   = foo
OBJECTS = foo.o bar.o main.o

.PHONY: all clean

OBJDIR=../obj
#VPATH=$(OBJDIR)

OBJPROG=$(addprefix $(OBJDIR)/, $(PROGS))

all: $(OBJPROG)


$(OBJPROG): $(addprefix $(OBJDIR)/, $(OBJECTS))
	@echo "Linking $@"
	@$(LINK.o) $^ $(LDLIBS) -o $@

$(OBJDIR)/%.o : %.c
	@echo "Building $@"
	@$(COMPILE.c) $< -o $@

$(OBJDIR)/main.o : main.c
	@echo "Building MAIN $@"
	@$(COMPILE.c) $< -o $@

clean:
	rm -f $(OBJDIR)/*

