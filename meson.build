project('timed-shutdown', 'c',
  version : '1.0.0',
  default_options : ['warning_level=2']
)

gtk4_dep = dependency('gtk4')
adwaita_dep = dependency('libadwaita-1')

executable('timed-shutdown',
  'src/main.c',
  dependencies : [gtk4_dep, adwaita_dep],
  install : true
)

install_data('data/com.example.TimedShutdown.desktop',
  install_dir : join_paths(get_option('datadir'), 'applications')
)