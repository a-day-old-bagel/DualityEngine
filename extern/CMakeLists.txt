
project( duality_extern LANGUAGES CXX C )

# helpful message
message( "To remove a submodule for real, see https://stackoverflow.com/a/36593218" )

# Find the libraries that should be installed on the user's system
# In Windows, this will require downloading the SDL2 Development Libraries and installing the Vulkan SDK,
# and making sure your environment variables are set up to point to them correctly

# ASSIMP
option( BUILD_SHARED_LIBS "Build package with shared libraries." OFF )
option( BUILD_FRAMEWORK "Build package as Mac OS X Framework bundle." OFF )
option( ASSIMP_DOUBLE_PRECISION "Set to ON to enable double precision processing" OFF )
option( ASSIMP_OPT_BUILD_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF )
option( ASSIMP_ANDROID_JNIIOSYSTEM "Android JNI IOSystem support is active" OFF )
option( ASSIMP_NO_EXPORT "Disable Assimp's export functionality." ON )
option( ASSIMP_BUILD_ZLIB "Build your own zlib" OFF )
option( ASSIMP_BUILD_ASSIMP_TOOLS "If the supplementary tools for Assimp are built in addition to the library." OFF )
option( ASSIMP_BUILD_SAMPLES "If the official samples are built as well (needs Glut)." OFF )
option( ASSIMP_BUILD_TESTS "If the test suite for Assimp is built in addition to the library." OFF )
option( ASSIMP_COVERALLS "Enable this to measure test coverage." OFF )
option( ASSIMP_WERROR "Treat warnings as errors." OFF )
option( ASSIMP_ASAN "Enable AddressSanitizer." OFF )
option( ASSIMP_UBSAN "Enable Undefined Behavior sanitizer." OFF )
option( SYSTEM_IRRXML "Use system installed Irrlicht/IrrXML library." OFF )
option( BUILD_DOCS "Build documentation using Doxygen." OFF )
option( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "default value of all ASSIMP_BUILD_XXX_IMPORTER value" FALSE )
option( ASSIMP_BUILD_OBJ_IMPORTER "build obj importer" TRUE )
option( ASSIMP_BUILD_COLLADA_IMPORTER "build collada (dae) importer" TRUE )
add_subdirectory( assimp )
list( APPEND duality_extern_includes
  ${CMAKE_CURRENT_BINARY_DIR}/assimp/include
  )
list( APPEND duality_extern_libs
  assimp
  )

# Assemble the external dependencies together into a linkable interface that also automatically includes all
# the necessary directories
add_library( duality_extern INTERFACE )
target_include_directories( duality_extern INTERFACE SYSTEM
  ${duality_extern_includes}
  )
target_link_libraries( duality_extern INTERFACE
  ${duality_extern_libs}
  )
target_compile_definitions( duality_extern INTERFACE
  ${duality_compile_definitions}
  )
