sdl2_dep = dependency('sdl2', required: get_option('sdl2-integration'))
wayland_client_dep = dependency('wayland-client')

if sdl2_dep.found()
    cppevents_sdl2_sources = [
        'sdl2/integration.cpp'
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
