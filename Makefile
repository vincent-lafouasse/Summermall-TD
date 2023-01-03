# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := summermall

BUILD_DIR := ./build
SRC_DIRS := ./src

T_DIR := ./test
T_SRC_DIR := $(T_DIR)/src
T_BUILD_DIR := $(T_DIR)/build

LIB_SDL := -lSDL2 -lSDL2_image

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS += $(shell find $(T_SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP -Wall -Wextra -pedantic -std=c++17 -g

# Linking
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	ln -sf $(shell pwd)/assets $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIB_SDL)

# Compile C 
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Compile C++
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run: $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "\nrunning $(TARGET_EXEC) !\n"
	$(BUILD_DIR)/$(TARGET_EXEC)

###############################################################################
########## TESTS ##########

TEST_EXEC = tests.out

# source files in ./src except for main.cpp + source files in ./test/src
T_SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s' | grep -v "main")
T_SRCS += $(shell find $(T_SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')

T_OBJS := $(T_SRCS:%=$(T_BUILD_DIR)/%.o)

T_LIBS := -lgtest -lgtest_main -lpthread -L/usr/lib
T_LIBS += $(LIB_SDL)

GTESTFLAGS = --gtest_color=yes --gtest_print_time=0

.PHONY: test
test: $(T_BUILD_DIR)/$(TEST_EXEC)
	$(T_BUILD_DIR)/$(TEST_EXEC) $(GTESTFLAGS)

$(T_BUILD_DIR)/$(TEST_EXEC): $(T_OBJS)
	ln -sf $(shell pwd)/assets $(T_BUILD_DIR)
	$(CXX) $(T_OBJS) -o $@ $(LDFLAGS) $(T_LIBS)

$(T_BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(T_BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean: 
	$(RM) -r $(BUILD_DIR) $(T_BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
DEPS += $(T_OBJS:.o=.d)
-include $(DEPS)
