sdl2_dep = dependency('sdl2', required: get_option('sdl2-integration'))
glfw_dep = dependency('glfw', required: get_option('glfw-integration'))

wayland_client_dep = dependency('wayland-client')

cppevents_lib_sources = [
    'filesystem-linux.cpp',
    'network-linux.cpp',
    'os-events-linux.cpp',
    'event_queue-linux.cpp',
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
        'sdl2-integration.cpp'
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

if glfw_dep.found()
    cppevents_glfw_sources = [
        'glfw-integration.cpp'
    ]

    glfw_integration = static_library(
        'cppevents-glfw',
        cppevents_glfw_sources,
        dependencies: [
            glfw_dep,
            wayland_client_dep,
        ],
        include_directories: cppevents_include_path,
    )

    cppevents_glfw_dep = declare_dependency(
        link_with: glfw_integration,
        include_directories: cppevents_include_path,
    )
endif
