# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src/googletest"
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src/googletest-build"
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix"
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/tmp"
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src/googletest-stamp"
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src"
  "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/user/Desktop/prosoft-c-stack/build/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
