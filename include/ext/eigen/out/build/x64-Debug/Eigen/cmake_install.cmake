# Install script for directory: D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Cholesky"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/CholmodSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Core"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Dense"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Eigen"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Eigenvalues"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Geometry"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Householder"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/IterativeLinearSolvers"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Jacobi"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/LU"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/MetisSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/OrderingMethods"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/PaStiXSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/PardisoSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/QR"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/QtAlignedMalloc"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SPQRSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SVD"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/Sparse"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SparseCholesky"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SparseCore"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SparseLU"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SparseQR"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/StdDeque"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/StdList"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/StdVector"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/SuperLUSupport"
    "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "D:/Yorick/source/repos/Test NanoGUI/include/ext/eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

