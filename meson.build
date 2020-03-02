project('shmem', 'cpp',
  default_options: [
    'buildtype=debug',
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17',
  ]
)

add_project_arguments(
  '-Wno-unused-parameter',
  '-Wno-terminate',
  '-Wno-pointer-arith',
  '-fpermissive',
  language: 'cpp'
)

test('shmem_test', executable('shmem_test', 'shmem_test.cc', link_args: ['-lrt']))
