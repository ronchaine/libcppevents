cppevents_lib_sources = [
   'filesystem.cpp',
   'network.cpp',
   'os_events.cpp',
   'event_queue-epoll.cpp',
]

cppevents_lib = library(
    'cppevents',
    cppevents_lib_sources,
    include_directories: cppevents_include_path,
)

cppevents_dep = declare_dependency(
    link_with: cppevents_lib,
    include_directories: cppevents_include_path,
)
