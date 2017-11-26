# Generic Makefile for compiling a simple executable.
#
#

OS := $(shell uname)

CC := clang++
SRCDIR := src
BUILDDIR := build
CFLAGS := -g -Wall `sdl-config --cflags`

ifeq ($(OS),Linux)
   LFLAGS := `sdl-config --libs` `pkg-config --libs gl`
else ifeq ($(OS),Darwin)
   LFLAGS := -framework OpenGL `sdl-config --libs`
endif

TARGET := bin/program

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)

$(TARGET): $(OBJECTS)
	@echo " Linking..."; $(CC) $(LFLAGS) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<

clean:
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(TARGET)

-include $(DEPS)

.PHONY: clean
