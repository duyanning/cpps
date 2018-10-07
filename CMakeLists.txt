cmake_minimum_required (VERSION 2.8.4)

project (cpps)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

set (CPPS_SRCS Action.cpp H2GchAction.cpp Obj2ExeAction.cpp Cpp2ObjAction.cpp helpers.cpp PhonyEntity.cpp DependencyGraphEntity.cpp Loggers.cpp ShebangMagic.cpp FileEntity.cpp main.cpp UpdateDependencyGraphAction.cpp GchMagic.cpp MiniLogger.cpp VulnerableFileEntity.cpp samples.cpp)


add_executable (cpps ${CPPS_SRCS})

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

target_link_libraries(cpps ${Boost_LIBRARIES})

include (CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(regex HAVE_REGEX)

if (NOT HAVE_REGEX)
  find_package(Boost REQUIRED COMPONENTS regex)
endif ()

target_link_libraries(cpps ${Boost_LIBRARIES})

configure_file (
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
  )

include_directories (${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

install (TARGETS cpps DESTINATION bin)

enable_testing()
add_test (cppsruns cpps)

add_test (cppsusage cpps)
set_tests_properties (cppsusage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage")

