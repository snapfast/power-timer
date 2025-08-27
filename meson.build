project('timed-shutdown', 'c',
  version : '1.1.0',
  default_options : ['warning_level=2']
)

gtk4_dep = dependency('gtk4')
adwaita_dep = dependency('libadwaita-1')

executable('timed-shutdown',
  'src/main.c',
  dependencies : [gtk4_dep, adwaita_dep],
  install : true
)

install_data('data/in.rahulbali.TimedShutdown.desktop',
  install_dir : join_paths(get_option('datadir'), 'applications')
)

install_data('data/timed-shutdown.svg',
  install_dir : join_paths(get_option('datadir'), 'icons', 'hicolor', 'scalable', 'apps')
)

install_data('data/mixkit-forest-birds-singing-1212.wav',
  install_dir : join_paths(get_option('datadir'), 'timed-shutdown')
)