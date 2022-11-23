# CMake generated Testfile for 
# Source directory: C:/Users/user/Desktop/prosoft-c-stack/src
# Build directory: C:/Users/user/Desktop/prosoft-c-stack/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(cstack_test "C:/Users/user/Desktop/prosoft-c-stack/build/Debug/cstack_test.exe")
  set_tests_properties(cstack_test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;27;add_test;C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(cstack_test "C:/Users/user/Desktop/prosoft-c-stack/build/Release/cstack_test.exe")
  set_tests_properties(cstack_test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;27;add_test;C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(cstack_test "C:/Users/user/Desktop/prosoft-c-stack/build/MinSizeRel/cstack_test.exe")
  set_tests_properties(cstack_test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;27;add_test;C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(cstack_test "C:/Users/user/Desktop/prosoft-c-stack/build/RelWithDebInfo/cstack_test.exe")
  set_tests_properties(cstack_test PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;27;add_test;C:/Users/user/Desktop/prosoft-c-stack/src/CMakeLists.txt;0;")
else()
  add_test(cstack_test NOT_AVAILABLE)
endif()
