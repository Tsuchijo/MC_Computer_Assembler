# Fetch Catch2 from GitHub
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0  # Use the latest release
)
FetchContent_MakeAvailable(Catch2)

# Create test executable
add_executable(tests 
    test_assembler.cpp
    ../src/assembler.cpp  # Include your source files
)

# Include directories
target_include_directories(tests PRIVATE ../src)

# Link Catch2
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Register tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(tests)