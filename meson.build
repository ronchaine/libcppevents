project(
  'cppevents',
  'cpp',
  default_options : [
      'cpp_std=c++2a',
      'buildtype=release',
      'default_library=static'
  ],
)

compiler = meson.get_compiler('cpp').get_id()

if compiler == 'clang'
  add_project_arguments(['-Xclang', '-fconcepts-ts'], language : 'cpp')
endif

add_project_arguments(['-Wall', '-Wextra', '-Wno-missing-braces'], language : 'cpp')

cppevents_include_path = include_directories('include')

subdir ('src')

if meson.is_subproject() == false
  subdir ('tests')
  subdir ('examples')
endif
