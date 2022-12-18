# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := summermall

BUILD_DIR := ./build
SRC_DIRS := ./src

T_DIR := ./test
T_SRC_DIR := $(T_DIR)/src
T_BUILD_DIR := $(T_DIR)/build

LIB_SDL := -lSDL2 -lSDL2_image

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS += $(shell find $(T_SRC_DIR) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
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

### TEST ###

TEST_EXEC = tests.out

# source files in ./src except for main.cpp
T_SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s' | grep -v "main")
# + source files in ./test/src
T_SRCS += $(shell find $(T_SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')

T_OBJS := $(T_SRCS:%=$(T_BUILD_DIR)/%.o)

T_LIBS := -lgtest -lgtest_main -lpthread -L/usr/lib
T_LIBS += $(LIB_SDL)

test: $(T_BUILD_DIR)/$(TEST_EXEC)
	$(T_BUILD_DIR)/$(TEST_EXEC)

$(T_BUILD_DIR)/$(TEST_EXEC): $(T_OBJS)
	ln -sf $(shell pwd)/assets $(T_BUILD_DIR)
	$(CXX) $(T_OBJS) -o $@ $(LDFLAGS) $(LIB_SDL)

$(T_BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(T_BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean: 
	$(RM) -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
DEPS += $(T_OBJS:.o=.d)
-include $(DEPS)
