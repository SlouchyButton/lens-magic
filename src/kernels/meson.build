prog_python = find_program('python3')
kernels_source = custom_target(
    'kernels-source.h',
    input : ['prepare-kernels.py', 'exposure.cl'],
    output : 'kernels-source.h',
    command : [prog_python, '@INPUT@', '@OUTPUT@'],
	build_by_default: true
)
lib_kernels_source = static_library('kernels-source', [kernels_source])
kernels_source_dep = declare_dependency(link_with : lib_kernels_source)