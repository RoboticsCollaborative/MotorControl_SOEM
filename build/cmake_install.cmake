# Install script for directory: /opt/MotorControl_SOEM

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/opt/MotorControl_SOEM/install")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/opt/MotorControl_SOEM/build/libsoem.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soem" TYPE FILE FILES
    "/opt/MotorControl_SOEM/soem/ethercatcoe.h"
    "/opt/MotorControl_SOEM/soem/ethercatsoe.h"
    "/opt/MotorControl_SOEM/soem/ethercatfoe.h"
    "/opt/MotorControl_SOEM/soem/ethercat.h"
    "/opt/MotorControl_SOEM/soem/ethercatconfiglist.h"
    "/opt/MotorControl_SOEM/soem/ethercatdc.h"
    "/opt/MotorControl_SOEM/soem/ethercattype.h"
    "/opt/MotorControl_SOEM/soem/ethercatprint.h"
    "/opt/MotorControl_SOEM/soem/ethercatbase.h"
    "/opt/MotorControl_SOEM/soem/ethercatconfig.h"
    "/opt/MotorControl_SOEM/soem/ethercatmain.h"
    "/opt/MotorControl_SOEM/osal/osal.h"
    "/opt/MotorControl_SOEM/osal/linux/osal_defs.h"
    "/opt/MotorControl_SOEM/oshw/linux/nicdrv.h"
    "/opt/MotorControl_SOEM/oshw/linux/oshw.h"
    "/opt/MotorControl_SOEM/prj/haptic/config_map.h"
    "/opt/MotorControl_SOEM/prj/haptic/config_SDOparam.h"
    "/opt/MotorControl_SOEM/prj/haptic/haptic_config.h"
    "/opt/MotorControl_SOEM/prj/haptic/controller.h"
    "/opt/MotorControl_SOEM/prj/haptic/ecattype.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/opt/MotorControl_SOEM/build/test/linux/slaveinfo/cmake_install.cmake")
  INCLUDE("/opt/MotorControl_SOEM/build/test/linux/eepromtool/cmake_install.cmake")
  INCLUDE("/opt/MotorControl_SOEM/build/test/linux/simple_test/cmake_install.cmake")
  INCLUDE("/opt/MotorControl_SOEM/build/prj/haptic/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/opt/MotorControl_SOEM/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/opt/MotorControl_SOEM/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
