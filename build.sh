# c99 ex.c -o ex -I../drm -I../drm/include/drm ../drm/builddir/libdrm.a -lm -static
# c99 test.c -o test -I../drm -I../drm/include/drm ../drm/builddir/libdrm.a -lm -static

c99 dm.c -o dm ./libs/drm/bin/libdrm.a -I./libs/drm/include -I./include -lm -static
c99 dmclient.c -o dmclient -static