cmake_minimum_required(VERSION 3.0.0)

set(SOURCES
  ../win32/registry.cpp
  ../win32/thread.cpp
  ../win32/synch.cpp
  ../win32/msgwnd.cpp
  ../win32/uuid.cpp
  ../win32/error.cpp
  ../log/log.cpp
  main.cpp
  mgr.def
)

add_headers(HEADERS ${SOURCES})

add_library(xfsmgr SHARED ${SOURCES} ${HEADERS})

target_link_libraries(xfsmgr rpcrt4)
