# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "H:/esp/v5.5.1/esp-idf/components/bootloader/subproject")
  file(MAKE_DIRECTORY "H:/esp/v5.5.1/esp-idf/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader"
  "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix"
  "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix/tmp"
  "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix/src/bootloader-stamp"
  "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix/src"
  "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "H:/ESPIDF_Projects/simpleI2cDemo/i2c_basic/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
