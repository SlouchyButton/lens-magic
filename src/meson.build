lens_magic_sources = [
  'main.c',
  'lens-magic-application.c',
  'lens-magic-window.c',
  'color-utils.c',
  'image-renderer.c',
  'hw-renderer.c',
]

subdir('shaders')

lens_magic_deps = [
  dependency('gtk4'),
  dependency('GL'),
  dependency('epoxy'),
  dependency('libadwaita-1', version: '>= 1.2'),
  cc.find_library('m', required : false),
  shaders_source_dep
]

lens_magic_sources += gnome.compile_resources('lens-magic-resources',
  'lens-magic.gresource.xml',
  c_name: 'lens_magic'
)

executable('lens-magic', lens_magic_sources,
  dependencies: lens_magic_deps,
       install: true,
)
