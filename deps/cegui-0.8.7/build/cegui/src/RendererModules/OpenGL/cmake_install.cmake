# Install script for directory: D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/cegui")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/lib/CEGUIOpenGLRenderer-0_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/lib/CEGUIOpenGLRenderer-0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/lib/CEGUIOpenGLRenderer-0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/lib/CEGUIOpenGLRenderer-0.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/bin/CEGUIOpenGLRenderer-0_d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/bin/CEGUIOpenGLRenderer-0.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/bin/CEGUIOpenGLRenderer-0.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/build/bin/CEGUIOpenGLRenderer-0.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cegui-0/CEGUI/RendererModules/OpenGL" TYPE FILE FILES
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/ApplePBTextureTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GL.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GL3FBOTextureTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GL3Renderer.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GLFBOTextureTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GLGeometryBuffer.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GLRenderer.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GLXPBTextureTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/GlmPimpl.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/RenderTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/RendererBase.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/Shader.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/ShaderManager.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/StandardShaderFrag.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/StandardShaderVert.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/Texture.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/TextureTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/ViewportTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/../../../include/CEGUI/RendererModules/OpenGL/WGLPBTextureTarget.h"
    "D:/Yorick/source/repos/SchoolGame/deps/cegui-0.8.7/cegui/src/RendererModules/OpenGL/RenderTarget.inl"
    )
endif()

