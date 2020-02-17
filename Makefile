CXX = g++
RM = rm -f $1
MKDIR = mkdir -p $1

SRC_DIR := src
OBJ_DIR := obj
BUILD_DIR := bin

DSP_BIN := dsp

CXXFLAGS := -mconsole -std=c++17 -g -Wall -I$(SRC_DIR)
LDFLAGS := -mconsole -lOpenAL32 -lliquid -lfftw3f
#-lliquid -lSoapySDR -lboost_thread -lboost_chrono -lboost_system

DSP_SRC = $(wildcard src/*.cpp src/dsp/*.cpp)

DSP_OBJ = $(patsubst %.cpp,%.o,$(DSP_SRC:$(SRC_DIR)/%=$(OBJ_DIR)/%))

ALLOBJ := $(sort $(DSP_OBJ))
OBJDIRS := $(sort $(dir $(ALLOBJ)))

default: dsp

.PHONY: clean build_dirs build_dsp $(OBJDIRS)

dsp: build_dirs build_dsp

run:
	$(BUILD_DIR)/$(DSP_BIN)

-include $(ALLOBJ:.o=.d)

build_dsp: $(DSP_OBJ)
	$(CXX) -o $(BUILD_DIR)/$(DSP_BIN) $^ $(LDFLAGS)

build_dirs: $(OBJDIRS)
	@$(call MKDIR, $(BUILD_DIR))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@$(CXX) $(CXXFLAGS) -MM $< > $(@:.o=.d)
	@-mv -f $(@:.o=.d) $(@:.o=.d.tmp)
	@-sed -e 's|.*:|$@:|' < $(@:.o=.d.tmp) > $(@:.o=.d)
	@-sed -e 's/.*://' -e 's/\\$$//' < $(@:.o=.d.tmp) | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(@:.o=.d)
	@-rm -f $(@:.o=.d.tmp)

$(OBJDIRS):
	@$(call MKDIR, $(@D))

.PHONY: clean
clean:
	$(call RM, $(BUILD_DIR)/$(DSP_BIN))
	$(call RM, $(DSP_OBJ))
