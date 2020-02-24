# Software to use
CXX	:= g++
AR	:= ar

# Commands
RM		= rm -f $1
MKDIR	= mkdir -p $1
CP		= cp $1 $2

# Functions
ifneq ($(TERM),xterm)
ECHO = echo $2
else
ECHO = tput setaf $1; echo $2; tput sgr0
endif

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
INC_DIR := include
PREFIX	:= /usr/local

# Projects
APPLICATION_PROJECTS	:= main
TEST_PROJECTS			:=

# Library Projects
BASE_LIBRARY		:= plasma

# Debug Projects
APPLICATION_PROJECTS_D	= $(addsuffix _d,$(APPLICATION_PROJECTS))
TEST_PROJECTS_D			= $(addsuffix _d,$(TEST_PROJECTS))

# Debug Libraries
BASE_LIBRARY_D			= $(addsuffix _d,$(BASE_LIBRARY))

# Source Files
APPLICATION_SRC	= $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRC		= $(wildcard $(SRC_DIR)/test/*.cpp)
BASE_SRC		= $(wildcard $(SRC_DIR)/dsp/*.cpp)

# Object Files
APPLICATION_OBJ	= $(patsubst %.cpp,%.o,$(APPLICATION_SRC:$(SRC_DIR)/%=$(OBJ_DIR)/%))
TEST_OBJ		= $(patsubst %.cpp,%.o,$(TEST_SRC:$(SRC_DIR)/%=$(OBJ_DIR)/%))
BASE_OBJ		= $(patsubst %.cpp,%.o,$(BASE_SRC:$(SRC_DIR)/%=$(OBJ_DIR)/%))

# Debug Object Files
APPLICATION_OBJ_D	= $(patsubst %.o,%_d.o,$(APPLICATION_OBJ))
TEST_OBJ_D			= $(patsubst %.o,%_d.o,$(TEST_OBJ))
BASE_OBJ_D			= $(patsubst %.o,%_d.o,$(BASE_OBJ))

# Helpers
ALL_LIBRARIES	= $(BASE_LIBRARY)
ALL_PROJECTS	= $(APPLICATION_PROJECTS)

# Debug Helpers
ALL_LIBRARIES_D	= $(addsuffix _d,$(ALL_LIBRARIES))
ALL_PROJECTS_D	= $(addsuffix _d,$(ALL_PROJECTS))

# CXX Flags
CXXFLAGS_BASE			= -std=c++17 -Wall -Wextra -O2 -I$(INC_DIR)
CXXFLAGS_APPLICATION	= $(CXXFLAGS_BASE)
CXXFLAGS_TEST			= $(CXXFLAGS_BASE)

# CXX Debug Flags
CXXFLAGS_BASE_D			= -std=c++17 -Wall -Wextra -g -I$(INC_DIR)
CXXFLAGS_APPLICATION_D	= $(CXXFLAGS_BASE_D)
CXXFLAGS_TEST_D			= $(CXXFLAGS_BASE_D)

# LD Flags
LDFLAGS_BASE			=
LDFLAGS_APPLICATION		= -L$(LIB_DIR) -l$(BASE_LIBRARY) -lopenal
LDFLAGS_TEST			= -L$(LIB_DIR) -l$(BASE_LIBRARY) -lgtest

# LD Debug Flags
LDFLAGS_BASE_D			=
LDFLAGS_APPLICATION_D	= -L$(LIB_DIR) -l$(BASE_LIBRARY_D) -lopenal
LDFLAGS_TEST_D			= -L$(LIB_DIR) -l$(BASE_LIBRARY_D) -lgtest

# OS-Specific Options
ifeq ($(OS),Windows_NT)
PREFIX			:= /mingw64
LDFLAGS_BASE	+= -mconsole
LDFLAGS_BASE_D	+= -mconsole
endif

# Library Files
BASE_LIBRARY_BIN		= $(addprefix lib,$(addsuffix .a,$(BASE_LIBRARY)))

# Debug Library Files
BASE_LIBRARY_BIN_D			= $(addprefix lib,$(addsuffix .a,$(BASE_LIBRARY_D)))

# Useful Variables
ALL_OBJ				= $(APPLICATION_OBJ) $(TEST_OBJ) $(BASE_OBJ) $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(ALL_PROJECTS)))
ALL_OBJ_D			= $(APPLICATION_OBJ_D) $(TEST_OBJ_D) $(BASE_OBJ_D) $(addprefix $(OBJ_DIR)/,$(addsuffix d.o,$(ALL_PROJECTS)))
OBJ_DIRS			= $(sort $(dir $(ALL_OBJ) $(ALL_OBJ_D)))
ALL_LIBRARIES_BIN	= $(addprefix $(LIB_DIR)/,$(BASE_LIBRARY_BIN))
ALL_LIBRARIES_BIN_D	= $(addprefix $(LIB_DIR)/,$(BASE_LIBRARY_BIN_D))
ALL_PROJECTS_BIN	= $(addprefix $(BIN_DIR)/,$(ALL_PROJECTS))
ALL_PROJECTS_BIN_D	= $(addprefix $(BIN_DIR)/,$(ALL_PROJECTS_D))

# Need second expansion to get the project names
.SECONDEXPANSION:

# Default Targets
.PHONY: default

default: $(ALL_LIBRARIES)

.PHONY: all libs $(ALL_LIBRARIES) $(ALL_PROJECTS)
.PHONY: all_d libs_d $(ALL_LIBRARIES_D) $(ALL_PROJECTS_D)
.PHONY: $(ALL_LIBRARIES_BIN) $(ALL_LIBRARIES_BIN_D)
.PHONY: $(ALL_PROJECTS_BIN) $(ALL_PROJECTS_BIN_D)
.PHONY: _build_dirs _run_tests _run_tests_d
.PHONY: variables
.PHONY: install

variables:
	@echo ALL_PROJECTS: $(ALL_PROJECTS)
	@echo ALL_PROJECTS_D: $(ALL_PROJECTS_D)
	@echo ALL_LIBRARIES: $(ALL_LIBRARIES)
	@echo ALL_LIBRARIES_D: $(ALL_LIBRARIES_D)
	@echo ALL_PROJECTS_BIN: $(ALL_PROJECTS_BIN)
	@echo ALL_PROJECTS_BIN_D: $(ALL_PROJECTS_BIN_D)
	@echo ALL_LIBRARIES_BIN: $(ALL_LIBRARIES_BIN)
	@echo ALL_LIBRARIES_BIN_D: $(ALL_LIBRARIES_BIN_D)
	@echo SRC_DIR: $(SRC_DIR)
	@echo BIN_DIR: $(BIN_DIR)
	@echo LIB_DIR: $(LIB_DIR)
	@echo OBJ_DIR: $(OBJ_DIR)
	@echo OBJ_DIRS: $(OBJ_DIRS)

all: $(ALL_PROJECTS)

all_d: $(ALL_PROJECTS_D)

libs: $(ALL_LIBRARIES)

libs_d: $(ALL_LIBRARIES_D)

# Install script
install: $(LIB_DIR)/$(BASE_LIBRARY_BIN)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(LIB_DIR)/$(BASE_LIBRARY_BIN) $(DESTDIR)$(PREFIX)/lib/
	cd $(INC_DIR) ; find . -type d -exec install -d "$(DESTDIR)$(PREFIX)/include/{}" \;
	cd $(INC_DIR) ; find . -type f -exec install -m 644 "{}" "$(DESTDIR)$(PREFIX)/include/{}" \;

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(BASE_LIBRARY_BIN)
	rm -rf $(DESTDIR)$(PREFIX)/include/plasma-dsp/

# Static Libraries
$(BASE_LIBRARY): CXXFLAGS = $(CXXFLAGS_BASE)
$(BASE_LIBRARY): _build_dirs _start_$$@ $(LIB_DIR)/$(BASE_LIBRARY_BIN) _finish_$$@
$(LIB_DIR)/$(BASE_LIBRARY_BIN): $(BASE_OBJ)
	@$(call $(ECHO), 5, "Building static library $@")
	$(AR) rcs $@ $^


# Debug Static Libraries
$(BASE_LIBRARY_D): CXXFLAGS = $(CXXFLAGS_BASE_D)
$(BASE_LIBRARY_D): _build_dirs _start_$$@ $(LIB_DIR)/$(BASE_LIBRARY_BIN_D) _finish_$$@
$(LIB_DIR)/$(BASE_LIBRARY_BIN_D): $(BASE_OBJ_D)
	@$(call ECHO, 5, "Building static library $@")
	$(AR) rcs $@ $^


# Project Targets
$(APPLICATION_PROJECTS): LDFLAGS = $(LDFLAGS_APPLICATION)
$(APPLICATION_PROJECTS): CXXFLAGS = $(CXXFLAGS_APPLICATION)
$(APPLICATION_PROJECTS): _build_dirs $(BASE_LIBRARY) _start_$$@ $(BIN_DIR)/$$(@) _finish_$$@
$(addprefix $(BIN_DIR)/,$(APPLICATION_PROJECTS)): $(APPLICATION_OBJ) $(OBJ_DIR)/$$(@F).o
	@$(call ECHO, 5, "Building executable $@")
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TEST_PROJECTS): LDFLAGS = $(LDFLAGS_TEST)
$(TEST_PROJECTS): CXXFLAGS = $(CXXFLAGS_TEST)
$(TEST_PROJECTS): _build_dirs $(BASE_LIBRARY) _start_$$@ $(BIN_DIR)/$$(@) _finish_$$@ _run_tests
$(addprefix $(BIN_DIR)/,$(TEST_PROJECTS)): $(TEST_OBJ) $(OBJ_DIR)/$$(@F).o
	@$(call ECHO, 5, "Building executable $@")
	$(CXX) -o $@ $^ $(LDFLAGS)


# Run Tests
_run_tests:
	@$(foreach _test,$(TEST_PROJECTS),$(BIN_DIR)/$(_test))


# Debug Project Targets
$(APPLICATION_PROJECTS_D): LDFLAGS = $(LDFLAGS_APPLICATION_D)
$(APPLICATION_PROJECTS_D): CXXFLAGS = $(CXXFLAGS_APPLICATION_D)
$(APPLICATION_PROJECTS_D): _build_dirs $(BASE_LIBRARY_D) _start_$$@ $(BIN_DIR)/$$(@) _finish_$$@
$(addprefix $(BIN_DIR)/,$(APPLICATION_PROJECTS_D)): $(APPLICATION_OBJ_D) $(OBJ_DIR)/$$(@F).o
	@$(call ECHO, 5, "Building executable $@")
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TEST_PROJECTS_D): LDFLAGS = $(LDFLAGS_TEST_D)
$(TEST_PROJECTS_D): CXXFLAGS = $(CXXFLAGS_TEST_D)
$(TEST_PROJECTS_D): _build_dirs $(BASE_LIBRARY_D) _start_$$@ $(BIN_DIR)/$$(@) _finish_$$@ _run_tests_d
$(addprefix $(BIN_DIR)/,$(TEST_PROJECTS_D)): $(TEST_OBJ_D) $(OBJ_DIR)/$$(@F).o
	@$(call ECHO, 5, "Building executable $@")
	$(CXX) -o $@ $^ $(LDFLAGS)


# Run Debug Tests
_run_tests_d:
	@$(foreach _test,$(TEST_PROJECTS_D),$(BIN_DIR)/$(_test))


# Dependencies
-include $(ALL_OBJ:.o=.d)
-include $(ALL_OBJ_D:.o=.d)


# Output Display
_start_%:
	@$(call ECHO, 6, "Building target $*")

_finish_%:
	@$(call ECHO, 2, "Target $* complete")

# Directories
_build_dirs:
	@$(call MKDIR, $(OBJ_DIRS))
	@$(call MKDIR, $(BIN_DIR))
	@$(call MKDIR, $(LIB_DIR))


# Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(call ECHO, 5, "Building CXX object $@")
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@$(CXX) $(CXXFLAGS) -MM $< > $(@:.o=.d)
	@-mv -f $(@:.o=.d) $(@:.o=.d.tmp)
	@-sed -e 's|.*:|$@:|' < $(@:.o=.d.tmp) > $(@:.o=.d)
	@-sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@-rm -f $(@:.o=.d.tmp)

$(OBJ_DIR)/%_d.o: $(SRC_DIR)/%.cpp
	@$(call ECHO, 5, "Building debug CXX object $@")
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@$(CXX) $(CXXFLAGS) -MM $< > $(@:.o=.d)
	@-mv -f $(@:.o=.d) $(@:.o=.d.tmp)
	@-sed -e 's|.*:|$@:|' < $(@:.o=.d.tmp) > $(@:.o=.d)
	@-sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@-rm -f $(@:.o=.d.tmp)


# Clean
clean:
	$(call RM, $(ALL_OBJ) $(ALL_OBJ:.o=.d))
	$(call RM, $(ALL_OBJ_D) $(ALL_OBJ_D:.o=.d))
	$(call RM, $(ALL_PROJECTS_BIN))
	$(call RM, $(ALL_PROJECTS_BIN_D))
	$(call RM, $(addprefix $(LIB_DIR)/,$(ALL_LIBRARIES_BIN)))
	$(call RM, $(addprefix $(LIB_DIR)/,$(ALL_LIBRARIES_BIN_D)))
