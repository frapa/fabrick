# c99 ex.c -o ex -I../drm -I../drm/include/drm ../drm/builddir/libdrm.a -lm -static
# c99 test.c -o test -I../drm -I../drm/include/drm ../drm/builddir/libdrm.a -lm -static

c99 -o dmgr display_manager/*.c ./libs/drm/bin/libdrm.a -I./libs/drm/include/drm \
    -I./libs -I./include -lm -lpthread -static
c99 dmclient.c -o dmclient -static -I./include -I./libs/uuid/include