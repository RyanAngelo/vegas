cmake_minimum_required(VERSION 3.16)
project(VegasGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find GTest
find_package(GTest REQUIRED)

# Source files for main executable
set(MAIN_SOURCES
    Dice_Main.cc
    Die.cc
    Board.cc
    Player.cc
)

# Source files for tests (excluding main)
set(LIB_SOURCES
    Die.cc
    Board.cc
    Player.cc
)

# Header files
set(HEADERS
    Die.h
    Board.h
    Player.h
)

# Create main executable
add_executable(vegas ${MAIN_SOURCES} ${HEADERS})

# Test source files
set(TEST_SOURCES
    tests/test_die.cpp
    tests/test_board.cpp
    tests/test_player.cpp
    tests/test_integration.cpp
)

# Create test executable
add_executable(vegas_tests ${TEST_SOURCES} ${LIB_SOURCES} ${HEADERS})

# Link GTest
target_link_libraries(vegas_tests GTest::gtest GTest::gtest_main)

# Enable testing
enable_testing()
add_test(NAME VegasTests COMMAND vegas_tests)

# Set compiler flags
if(MSVC)
    target_compile_options(vegas PRIVATE /W4)
    target_compile_options(vegas_tests PRIVATE /W4)
else()
    target_compile_options(vegas PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vegas_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install targets
install(TARGETS vegas
    RUNTIME DESTINATION bin
)

# Install headers
install(FILES ${HEADERS}
    DESTINATION include/vegas
)
