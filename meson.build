project('power-timer', 'c',
  version : '2.1.1',
  default_options : ['warning_level=2']
)

gtk4_dep = dependency('gtk4')
adwaita_dep = dependency('libadwaita-1')

executable('power-timer',
  ['src/main.c', 'src/ui.c', 'src/timer.c', 'src/sound.c', 'src/power.c'],
  dependencies : [gtk4_dep, adwaita_dep],
  install : true
)

install_data('data/in.rahulbali.PowerTimer.desktop',
  install_dir : join_paths(get_option('datadir'), 'applications')
)

install_data('data/power-timer.svg',
  install_dir : join_paths(get_option('datadir'), 'icons', 'hicolor', 'scalable', 'apps')
)

