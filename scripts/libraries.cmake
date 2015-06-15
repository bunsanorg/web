find_path(BOOSTER_INCLUDE_DIRECTORIES booster/config.h)
find_library(BOOSTER_LIBRARIES booster)
bunsan_add_imported_library(booster SHARED BOOSTER)

find_path(CPPCMS_INCLUDE_DIRECTORIES cppcms/config.h)
find_library(CPPCMS_LIBRARIES cppcms)
bunsan_add_imported_library(cppcms SHARED CPPCMS booster)

find_path(CPPDB_INCLUDE_DIRECTORIES cppdb/defs.h)
find_library(CPPDB_LIBRARIES cppdb)
bunsan_add_imported_library(cppdb SHARED CPPDB booster)
