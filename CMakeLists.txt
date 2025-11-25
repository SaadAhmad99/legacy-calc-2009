cmake_minimum_required(VERSION 3.14)
project(loan-calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Your library / executable
add_library(loancalc STATIC
    LoanCalculator.cpp
)
target_include_directories(loancalc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable (optional)
add_executable(loancalc_main LoanCalculatorMain.cpp)
target_link_libraries(loancalc_main PRIVATE loancalc)

# ---- GoogleTest ----
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: prevent overriding library names
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(loancalc_tests tests/test_loancalculator.cpp)
target_link_libraries(loancalc_tests PRIVATE loancalc gtest_main)

add_test(NAME LoanCalcTests COMMAND loancalc_tests)

# ---- Doxygen ----
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(doc_doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
else()
  message(STATUS "Doxygen not found. Docs target disabled.")
endif()
