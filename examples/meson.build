sdl2_dep = dependency('sdl2', required: false)

if sdl2_dep.found()
  executable(
    'sdl-example',
    'sdl-example.cpp',
    include_directories : cppevents_include_path,
    dependencies: [
      cppevents_dep,
      sdl2_dep,
      cppevents_sdl2_dep,
    ]
  )
endif

executable(
  'timer-example',
  'timer-example.cpp',
  dependencies : [
    cppevents_dep,
  ]
)
