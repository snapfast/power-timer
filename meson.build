project('timed-shutdown', 'c',
  version : '1.1.1',
  default_options : ['warning_level=2']
)

gtk4_dep = dependency('gtk4')
adwaita_dep = dependency('libadwaita-1')

executable('timed-shutdown',
  ['src/main.c', 'src/ui.c', 'src/timer.c', 'src/sound.c', 'src/power.c'],
  dependencies : [gtk4_dep, adwaita_dep],
  install : true
)

install_data('data/in.rahulbali.TimedShutdown.desktop',
  install_dir : join_paths(get_option('datadir'), 'applications')
)

install_data('data/timed-shutdown.svg',
  install_dir : join_paths(get_option('datadir'), 'icons', 'hicolor', 'scalable', 'apps')
)

