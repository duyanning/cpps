cmake_minimum_required (VERSION 2.8.4)

project (cpps)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

set (CPPS_SRCS Action.cpp H2GchAction.cpp Obj2ExeAction.cpp Cpp2ObjAction.cpp helpers.cpp PhonyEntity.cpp DependencyGraphEntity.cpp Loggers.cpp ShebangMagic.cpp FileEntity.cpp main.cpp UpdateDependencyGraphAction.cpp GchMagic.cpp MiniLogger.cpp VulnerableFileEntity.cpp samples.cpp)



add_executable (cpps ${CPPS_SRCS})

find_library (BOOST_SYSTEM
  NAMES boost_system
  PATHS /usr/lib /usr/local/lib
  )

find_library (BOOST_FILESYSTEM
  NAMES boost_filesystem
  PATHS /usr/lib /usr/local/lib
  )

find_library (BOOST_PROGRAM_OPTIONS
  NAMES boost_program_options
  PATHS /usr/lib /usr/local/lib
  )

if (BOOST_SYSTEM)
  target_link_libraries (cpps ${BOOST_SYSTEM})
endif ()

if (BOOST_FILESYSTEM)
  target_link_libraries (cpps ${BOOST_FILESYSTEM})
endif ()

if (BOOST_PROGRAM_OPTIONS)
  target_link_libraries (cpps ${BOOST_PROGRAM_OPTIONS})
endif ()

install (TARGETS cpps DESTINATION bin)

enable_testing()
add_test (cppsruns cpps)

add_test (cppsusage cpps)
set_tests_properties (cppsusage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage")

