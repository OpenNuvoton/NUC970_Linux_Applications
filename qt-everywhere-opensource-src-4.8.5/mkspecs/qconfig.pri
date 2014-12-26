#configuration
CONFIG +=  cross_compile static def_files_disabled no_mocdepend release qt_no_framework
QT_ARCH = arm
QT_EDITION = OpenSource
QT_CONFIG +=  no-pkg-config static embedded reduce_exports ipv6 clock-gettime clock-monotonic mremap getaddrinfo ipv6ifname inotify jpeg no-mng png no-gif no-tiff freetype zlib alsa concurrent release

#versioning
QT_VERSION = 4.8.5
QT_MAJOR_VERSION = 4
QT_MINOR_VERSION = 8
QT_PATCH_VERSION = 5

#namespaces
QT_LIBINFIX = 
QT_NAMESPACE = 
QT_NAMESPACE_MAC_CRC = 

QMAKE_RPATHDIR += "/usr/local/Trolltech/QtEmbedded-4.8.5/lib"
QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 3
QT_GCC_PATCH_VERSION = 4
