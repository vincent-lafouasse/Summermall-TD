TEST_FILE = test_main.cpp
CTESTFLAGS = --output-on-failure


test: $(TEST_FILE)
	cmake -S . -B build
	cmake --build build
	@echo
	@echo "\033[92mBuild completed\033[0m"
	@echo
	@echo "\033[92mTesting:\033[0m"
	@echo
	cd build && ctest $(CTESTFLAGS)
