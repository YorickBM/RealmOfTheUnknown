# Install script for directory: C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/12107/Source/Repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/out/install/x64-Debug")
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
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Cholesky"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/CholmodSupport"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Core"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Dense"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Eigen"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Eigenvalues"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Geometry"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Householder"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/IterativeLinearSolvers"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Jacobi"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/LU"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/MetisSupport"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/OrderingMethods"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/PaStiXSupport"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/PardisoSupport"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/QR"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/QtAlignedMalloc"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SPQRSupport"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SVD"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/Sparse"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SparseCholesky"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SparseCore"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SparseLU"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SparseQR"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/StdDeque"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/StdList"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/StdVector"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/SuperLUSupport"
    "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "C:/Users/12107/source/repos/YorickBM/RealmOfTheUnknown/deps/include/ext/eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

