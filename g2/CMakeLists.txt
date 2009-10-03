cmake_minimum_required(VERSION 2.4)
project (g2)

include_directories (../libc/include)
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
  include_directories (../libc/win32)
  add_subdirectory (../libc/win32 libc)
elseif (UNIX)
  include_directories(../libc/glibc)
  add_subdirectory (../libc/glibc libc)
else (WIN32)
  include_directories(../libc/default)
  add_subdirectory (../libc/default libc)
endif (WIN32)
include_directories (../libc/default)

set (SERVER_SRCS
 src/add.c src/adv.c src/alchem.c src/art.c src/basic.c src/beast.c src/build.c src/buy.c
 src/check.c src/c1.c src/c2.c src/cloud.c src/code.c src/combat.c src/day.c src/dir.c src/display.c
 src/eat.c src/faery.c src/garr.c src/gate.c src/glob.c src/gm.c src/hades.c src/immed.c src/input.c src/io.c
 src/loc.c src/lore.c src/main.c src/make.c src/move.c src/necro.c src/npc.c src/order.c src/perm.c src/produce.c
 src/quest.c src/relig.c src/report.c src/rnd.c
 src/savage.c src/scry.c src/seed.c src/sout.c src/stack.c src/stealth.c src/storm.c src/summary.c src/swear.c
 src/tunnel.c src/u.c src/use.c src/z.c
)

set (SERVER_HDRS
 src/code.h src/dir.h src/display.h src/etc.h src/garr.h src/loc.h src/loop.h src/oly.h src/order.h
 src/sout.h src/stack.h src/swear.h src/use.h src/u.h src/z.h
)

add_executable (olympia ${SERVER_SRCS} ${SERVER_HDRS})
if (WIN32)
  target_link_libraries (olympia oly-libc)
endif (WIN32)

set (MAPGEN_SRCS
 mapgen/mapgen.c
 mapgen/rnd.c
 mapgen/z.c
)

set (MAPGEN_HDRS
 mapgen/z.h
)

add_executable (mapgen ${MAPGEN_SRCS} ${MAPGEN_HDRS})
