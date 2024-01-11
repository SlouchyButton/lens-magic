prog_python = find_program('python3')
shaders_source = custom_target(
    'shaders-source.h',
    input : ['prepare-shaders.py', 'default.vs.glsl', 'flipped.vs.glsl', 'default.fs.glsl', 
              'exposure.fs.glsl', 'brightness.fs.glsl', 'contrast.fs.glsl', 'tint.fs.glsl', 
              'saturation.fs.glsl', 'highlights.fs.glsl', 'shadows.fs.glsl', 'temperature.fs.glsl'],
    output : 'shaders-source.h',
    command : [prog_python, '@INPUT@', '@OUTPUT@'],
	build_by_default: true
)
lib_shaders_source = static_library('shaders-source', [shaders_source])
shaders_source_dep = declare_dependency(link_with : lib_shaders_source)