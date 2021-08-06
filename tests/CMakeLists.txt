enable_testing()

find_package(Catch2 REQUIRED)

include(CTest)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

add_definitions("-DCATCH_CONFIG_WCHAR")

add_executable(tests
				lexer_tests.cpp
				main "source_reader_tests.cpp")

target_link_libraries(tests PRIVATE Catch2::Catch2 PUBLIC seam)