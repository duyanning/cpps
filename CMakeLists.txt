cmake_minimum_required (VERSION 2.6.2)

project (cpps)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

set (CPPS_SRCS main.cpp Action.cpp DependencyGraphEntity.cpp Obj2ExeAction.cpp ShebangMagic.cpp Cpp2ObjAction.cpp FileEntity.cpp MiniLogger.cpp PhonyEntity.cpp UpdateDependencyGraphAction.cpp)

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
