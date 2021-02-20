sdl2_dep = dependency('sdl2', required: get_option('sdl2-integration'))

wayland_client_dep = dependency('wayland-client')

cppevents_lib_sources = [
    'platform-linux/filesystem.cpp',
    'platform-linux/network.cpp',
    'platform-linux/os_events.cpp',
    'platform-linux/event_queue.cpp',
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

if sdl2_dep.found()
    cppevents_sdl2_sources = [
        'integrations/sdl2-integration.cpp'
    ]

    sdl2_integration = static_library(
        'cppevents-sdl2',
        cppevents_sdl2_sources,
        dependencies: [
            sdl2_dep,
            wayland_client_dep,
        ],
        include_directories: cppevents_include_path,
    )

    cppevents_sdl2_dep = declare_dependency(
        link_with: sdl2_integration,
        include_directories: cppevents_include_path,
    )
endif
