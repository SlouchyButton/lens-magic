lens_magic_sources = [
  'main.c',
  'lens-magic-application.c',
  'lens-magic-windows/lens-magic-window.c',
  'lens-magic-windows/signals/lens-magic-window-signals.c',
  'rendering/image-renderer.c',
  'rendering/image-processing.c',
]

subdir('shaders')

lens_magic_deps = [
  dependency('gtk4'),
  dependency('pango'),
  dependency('GL'),
  dependency('epoxy'),
  dependency('libraw'),
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
