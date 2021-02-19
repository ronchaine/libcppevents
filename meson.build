project(
  'cppevents',
  'cpp',
  default_options : [
      'cpp_std=c++20',
      'buildtype=release',
      'default_library=static'
  ],
)

compiler = meson.get_compiler('cpp').get_id()

if meson.is_subproject() == true
  add_project_arguments(['-Wall', '-Wextra', '-pedantic', '-Wno-missing-braces'], language : 'cpp')
else
  add_project_arguments(['-Wall', '-Wextra', '-pedantic', '-Werror', '-Wno-missing-braces'], language : 'cpp')
endif

cppevents_include_path = include_directories('include')

subdir ('src')

if meson.is_subproject() == false
  subdir ('tests')
  subdir ('examples')
endif
