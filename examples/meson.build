sdl2_dep = dependency('sdl2', required: false)

executable(
  'ex0',
  'ex0.cpp',
  include_directories : cppevents_include_path,
  dependencies: cppevents_dep
)

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
