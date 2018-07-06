# Copyright (c) 2018 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 2.8.7)
project(umi CXX)
include(webOS/webOS)
webos_modules_init(1 0 0 QUALIFIER RC7)
webos_component(1 0 0)

webos_add_compiler_flags(ALL -std=c++11 -Wall -Wextra -fno-permissive)
#disable specific warnings
webos_add_compiler_flags(ALL -Wno-unused-parameter -Wno-type-limits -Wno-comment)
#promote specific warnings to errors
webos_add_compiler_flags(ALL -Werror=return-type  -Werror=reorder -Werror=uninitialized)
# Require that all undefined symbols are satisfied by the libraries from target_link_libraries()
webos_add_linker_options(ALL --no-undefined)
webos_add_linker_options(ALL --hash-style=gnu --as-needed --as-needed)

include(FindPkgConfig)

include_directories(include/public)

pkg_check_modules(PMLOG REQUIRED PmLogLib)
include_directories(${PMLOG_INCLUDE_DIRS})
webos_add_compiler_flags(ALL ${PMLOG_CFLAGS_OTHER})

pkg_check_modules(PBNJSON_CPP REQUIRED pbnjson_cpp)
include_directories(${PBNJSON_CPP_INCLUDE_DIRS})
webos_add_compiler_flags(ALL ${PBNJSON_CPP_CFLAGS_OTHER})

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})
webos_add_compiler_flags(ALL ${GLIB2_CFLAGS})

pkg_check_modules(ALSA_LIB REQUIRED alsa)
include_directories(${ALSA_LIB_INCLUDE_DIRS})
webos_add_compiler_flags(ALL ${ALSA_LIB_CFLAGS})

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/umi)
file(GLOB SOURCES src/*.cpp src/umi/*.cpp)

file(MAKE_DIRECTORY "include/public")

#add local include directories, accessible only by double quotes include.
webos_add_compiler_flags(ALL -iquote ${CMAKE_CURRENT_BINARY_DIR}/Configured/files/conf)
webos_add_compiler_flags(ALL -iquote ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(umi SHARED ${SOURCES})
target_link_libraries(umi
        ${PMLOG_LDFLAGS}
        ${PBNJSON_CPP_LDFLAGS}
        ${GLIB2_LDFLAGS}
        ${ALSA_LIB_LDFLAGS}
        pthread
        )

webos_build_library(TARGET umi)

install(TARGETS umi DESTINATION ${WEBOS_INSTALL_LIBDIR})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/files/conf/device-cap.json
        ${CMAKE_CURRENT_SOURCE_DIR}/files/conf/umi-config.json
        DESTINATION ${WEBOS_INSTALL_WEBOS_SYSCONFDIR}/umi
        )

webos_configure_source_files(SOURCES files/conf/config.h)

webos_build_pkgconfig(files/pkgconfig/umi)

webos_config_build_doxygen(doc Doxyfile)

#WEBOS_SOC is updted fromm bb/bbappend
add_definitions(-DWEBOS_SOC_TYPE="${WEBOS_SOC}")

