# Install script for directory: D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/AdolcForward"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/AlignedVector3"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/ArpackSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/AutoDiff"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/BVH"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/EulerAngles"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/FFT"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/IterativeSolvers"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/KroneckerProduct"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/LevenbergMarquardt"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/MatrixFunctions"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/MoreVectorization"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/MPRealSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/NonLinearOptimization"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/NumericalDiff"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/OpenGLSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/Polynomials"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/Skyline"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/SparseExtra"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/SpecialFunctions"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/out/build/x64-Debug/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

