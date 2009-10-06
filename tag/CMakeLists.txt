cmake_minimum_required(VERSION 2.4)
project (tag)

include_directories (../libc/include)
add_definitions(-DNEW_TRADE)
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
  include_directories (../libc/win32)
  add_subdirectory (../libc/win32 libc)
elseif (UNIX)
  add_definitions(-DLINUX)
  include_directories(../libc/glibc)
  add_subdirectory (../libc/glibc libc)
  # hack
#  set(CMAKE_C_FLAGS "-g -Wall")
  set(CMAKE_C_FLAGS "-g")
else (WIN32)
  include_directories(../libc/default)
  add_subdirectory (../libc/default libc)
endif (WIN32)
include_directories (../libc/default)

set (SERVER_SRCS
 src/add.c src/adv.c src/alchem.c src/art.c src/basic.c src/beast.c src/build.c src/buy.c
 src/check.c src/c1.c src/c2.c src/cloud.c src/code.c src/combat.c src/day.c src/dir.c
 src/display.c src/eat.c src/faery.c src/garr.c src/gate.c src/glob.c src/gm.c src/hades.c
 src/immed.c src/input.c src/io.c src/loc.c src/lore.c src/main.c src/make.c src/move.c
 src/necro.c src/npc.c src/order.c src/perm.c src/produce.c src/quest.c src/relig.c
 src/report.c src/savage.c src/scry.c src/seed.c src/sout.c src/stack.c src/stealth.c
 src/storm.c src/summary.c src/swear.c src/u.c src/use.c src/effect.c src/trading.c
 src/shipcraft.c src/mining.c src/win.c src/kill.c src/ranger.c src/times.c
 src/artifacts.c src/map.c src/hero.c src/z.c
)

set (SERVER_HDRS
 src/loop.h src/code.h src/dir.h src/display.h src/etc.h src/garr.h src/loc.h src/order.h 
 src/sout.h src/stack.h src/swear.h src/u.h src/use.h src/effect.h src/relig.h src/combat.h
 src/quest.h src/build.h src/kill.h src/artifacts.h src/eat.h
)

add_executable (olympia ${SERVER_SRCS} ${SERVER_HDRS})
if (WIN32)
  target_link_libraries (olympia oly-libc)
elseif (UNIX)
  target_link_libraries (olympia m)
endif (WIN32)

set (MAPGEN_SRCS mapgen/mapgen.c mapgen/rnd.c mapgen/z.c)
set (MAPGEN_HDRS mapgen/z.h )
add_executable (mapgen ${MAPGEN_SRCS} ${MAPGEN_HDRS})

set (ACCT_SRCS cmd/newacct.c)
add_executable (acct ${ACCT_SRCS})

set (REP_SRCS cmd/rep.c)
add_executable (rep ${REP_SRCS})
