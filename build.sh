CC=cc

# Sock
for src in include/sock/*.c;
do
    $CC -c $src -o ${src%.c}.o \
        -Iinclude/sock \
        -fPIC \
        -std=c99
done
ar rcs include/sock.a include/sock/*.o

# Dumbo
for src in include/dumbo/*.c;
do
    $CC -c $src -o ${src%.c}.o \
        -Ilibs \
        -Ilibs/drm/include \
        -Iinclude \
        -fPIC \
        -std=c99
done
ar rcs include/dumbo.a include/dumbo/*.o

# Protosock
for src in include/protosock/*.c;
do
    $CC -c $src -o ${src%.c}.o \
        -Iinclude \
        -Iinclude/protosock \
        -fPIC \
        -std=c99
done
ar rcs include/protosock.a include/protosock/*.o

# Display manager
for src in display_manager/*.c;
do
    $CC -c $src -o ${src%.c}.o \
        -Ilibs \
        -Ilibs/drm/include \
        -Iinclude \
        -D_XOPEN_SOURCE=500 \
        -fPIC \
        -std=c99
done
ar rcs display_manager.a display_manager/*.o

# UI Kit
for src in uik/*.c;
do
    $CC -c $src -o ${src%.c}.o \
        -Iinclude \
        -Ilibs \
        -Ilibs/cairo/include \
        libs/cairo/bin/libcairo.a \
        -fPIC \
        -std=c99
done
ar rcs uik.a uik/*.o

# Server
$CC -o dmgr compositor/*.c \
    -Idisplay_manager \
    display_manager.a \
    -Ilibs \
    -Iinclude \
    ./include/sock.a \
    ./include/dumbo.a \
    -Ilibs/drm/include \
    ./libs/drm/bin/libdrm.a \
    -D_XOPEN_SOURCE=500 \
    -lrt -lpthread -lm \
    -static \
    -fPIC \
    -std=c99

# Client
$CC -o dmclient dmclient.c \
    -Ilibs \
    -Iinclude \
    ./uik.a \
    ./include/protosock.a \
    ./include/sock.a \
    -static \
    -fPIC \
    -std=c99