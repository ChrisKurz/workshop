# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Nordic/ws_labs/lab3_device_pm")
  file(MAKE_DIRECTORY "C:/Nordic/ws_labs/lab3_device_pm")
endif()
file(MAKE_DIRECTORY
  "C:/Nordic/ws_labs/lab3_device_pm/build/lab3_device_pm"
  "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix"
  "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix/tmp"
  "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix/src/lab3_device_pm-stamp"
  "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix/src"
  "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix/src/lab3_device_pm-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix/src/lab3_device_pm-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Nordic/ws_labs/lab3_device_pm/build/_sysbuild/sysbuild/images/lab3_device_pm-prefix/src/lab3_device_pm-stamp${cfgdir}") # cfgdir has leading slash
endif()
