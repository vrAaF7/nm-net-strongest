project('nm-conn-strongest', 'c',
     default_options: [
          'buildtype=debugoptimized',
          'c_std=gnu89'])

glib_dep = dependency('glib-2.0')
nm_dep = dependency('NetworkManager')
libnm_dep = dependency('libnm')
libgio_dep = dependency('gio-2.0')

executable('nm-conn-strongest', 'main.c',
           dependencies: [glib_dep,
                          libnm_dep,
                          nm_dep,
                          libgio_dep])
