option(CPP_STARTER_USE_QT "Enable compilation of QT sample" OFF)
option(CPP_STARTER_USE_FLTK "Enable compilation of FLTK sample" OFF)
option(CPP_STARTER_USE_GTKMM "Enable compilation of GTKMM sample" OFF)
option(CPP_STARTER_USE_IMGUI "Enable compilation of ImGui sample" OFF)
option(CPP_STARTER_USE_NANA "Enable compilation of Nana GUI sample" OFF)
option(CPP_STARTER_USE_SDL "Enable compilation of SDL sample" OFF)
option(USE_BOOST "Enable compilation of boost" ON)

# boost
if(USE_BOOST)
  ## boost include locations when using different boost
  #SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ~/boost_1_76_0/)
  #INCLUDE_DIRECTORIES(SYSTEM ~/boost_1_76_0)

  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  find_package(OpenSSL 1.1.1 REQUIRED)
  INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIRS})
  LINK_DIRECTORIES(${OPENSSL_LIBRARIES})

  find_package(Boost 1.71.0 REQUIRED )
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
endif()  
# sdl
if(CPP_STARTER_USE_SDL)
  message("Using SDL2")
  add_subdirectory(sdl)
endif()

# qt
if(CPP_STARTER_USE_QT)
  message("Using Qt")
  add_subdirectory(qt)
endif()

# fltk test
if(CPP_STARTER_USE_FLTK)
  message("Using FLTK")
  add_subdirectory(fltk)
endif()

# gtkmm test
if(CPP_STARTER_USE_GTKMM)
  message("Using gtkmm")
  add_subdirectory(gtkmm)
endif()

# imgui example
if(CPP_STARTER_USE_IMGUI)
  message("Using Dear ImGui")
  add_subdirectory(imgui)
endif()

# Nana
if(CPP_STARTER_USE_NANA)
  message("Using Nana")
  add_subdirectory(nana)
endif()

# Generic test that uses conan libs
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/")
FILE(GLOB_RECURSE ROOTHEADERS ${PROJECT_SOURCE_DIR}/src/*.h)
FILE(GLOB_RECURSE ROOTSOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(beastserver main.cpp ${ROOTSOURCES} )
target_link_libraries(
  beastserver
  PRIVATE project_options
  project_warnings
  Threads::Threads
#          CONAN_PKG::docopt.cpp
#          CONAN_PKG::fmt
#          CONAN_PKG::spdlog)
)
