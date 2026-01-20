# Custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

# Paths and global macros
buildDir := build
objDir := $(buildDir)/obj
binDir := $(buildDir)/bin

executable := app
target := $(binDir)/$(executable)

sources := $(call rwildcard,src/,*.cpp,*.hpp)
objects := $(patsubst src/%, $(objDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

compileFlags := -std=c++17 -I include
linkFlags = -L lib/$(platform) -l raylib

# Platform detection
ifeq ($(OS), Windows_NT)
	platform := Windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		platform := Linux
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
	endif
	ifeq ($(UNAMEOS), Darwin)
		platform := macOS
		CXX ?= clang++
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif

	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif

# Phony targets
.PHONY: all setup submodules execute clean

# Default target
all: $(target) execute 

# Setup
setup: include lib

submodules:
	git submodule update --init --recursive --depth 1

include: submodules
	$(MKDIR) $(call platformpth, ./include)
	$(call COPY,vendor/raylib/src,./include,raylib.h)
	$(call COPY,vendor/raylib/src,./include,raymath.h)
	$(call COPY,vendor/raylib/src,./include,rlgl.h)
	$(call COPY,vendor/raylib-cpp/include,./include,*.hpp)

lib: submodules
	cd vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP
	$(MKDIR) $(call platformpth, lib/$(platform))
	$(call COPY,vendor/raylib/src,lib/$(platform),libraylib.a)

# Ensure required directories
$(binDir):
	$(MKDIR) $(call platformpth, $(binDir))

$(objDir):
	$(MKDIR) $(call platformpth, $(objDir))

# Linking
$(target): $(binDir) $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Dependencies
-include $(depends)

# Compilation rule
$(objDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)

# Execute
execute:
	$(target) $(ARGS)

# Clean — ONLY object files
clean:
	$(RM) $(call platformpth, $(objDir))
