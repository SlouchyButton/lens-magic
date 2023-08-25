lens_magic_sources = [
  'main.c',
  'lens-magic-application.c',
  'lens-magic-window.c',
  'color-utils.c',
  'image-renderer.c',
  'hw-renderer.c',
]

prog_python = find_program('python3')
kernel_source = custom_target(
    'kernel-source.h',
    input : ['prepare-kernel.py', 'kernel-source.cl'],
    output : 'kernel-source.h',
    command : [prog_python, '@INPUT0@', '@INPUT1@', '@OUTPUT@'],
	build_by_default: true
)
lib_kernel_source = static_library('kernel-source', [kernel_source])
kernel_source_dep = declare_dependency(link_with : lib_kernel_source)

lens_magic_deps = [
  dependency('gtk4'),
  dependency('OpenCL'),
  dependency('OpenCL-Headers'),
  dependency('libadwaita-1', version: '>= 1.2'),
  cc.find_library('m', required : false),
  kernel_source_dep
]

lens_magic_sources += gnome.compile_resources('lens-magic-resources',
  'lens-magic.gresource.xml',
  c_name: 'lens_magic'
)

executable('lens-magic', lens_magic_sources,
  dependencies: lens_magic_deps,
       install: true,
)
