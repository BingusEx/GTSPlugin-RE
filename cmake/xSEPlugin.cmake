add_compile_definitions(SKYRIM)
set(CommonLibPath "extern/CommonLibNG")

# =================================================================================================
# # Target Architecture Configuration
# =================================================================================================
# This option enables the AVX instruction set for release builds.
# If OFF, the project defaults to SSE4.2 for compatibility with potato PCs.

option(GTS_ENABLE_AVX "Build with AVX instruction set for release builds" OFF)

# Set the MSVC arch flag based on the option
if(GTS_ENABLE_AVX)
    set(ARCH_FLAG "/arch:AVX")
    message(STATUS "Release builds will target architecture: AVX")
else()
    set(ARCH_FLAG "/arch:SSE4.2")
    message(STATUS "Release builds will target architecture: SSE4.2 (Default)")
endif()

add_library("${PROJECT_NAME}" SHARED)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
	cxx_std_23
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(AddCXXFiles)
add_cxx_files("${PROJECT_NAME}")

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
	src/PCH.hpp
)

# Build DLL RC
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
    @ONLY
)
target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/version.rc")

# Build Version.hpp from project info.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/Version.hpp"
    @ONLY
)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

add_compile_definitions(NOMINMAX)
add_compile_definitions(_UNICODE)

# --- Release-specific compiler options ---
set(SC_RELEASE_OPTS
    ${ARCH_FLAG}                 # Set based on ENABLE_AVX option
    "/Zi"                        # Generate full debug information (.pdb file) for release builds
    "/fp:fast"                   # Use fast, less precise floating-point model
    "/GL"                        # Enable Whole Program Optimization (LTCG)
    "/Gw"                        # Optimize global data
    "/O2"                        # Optimize for speed
    "/Ob2"                       # Aggressive inlining
    "/Oi"                        # Generate intrinsic functions
    "/Ot"                        # Favor fast code
    "/Oy"                        # Omit frame pointers
    "/fp:except-"                # Disable floating-point exceptions for performance
    "/Gy"                        # Enable function-level linking
    "/Gm-"                       # Disable minimal rebuild
    "/sdl-"                      # Disable Security Development Lifecycle checks
    "/GS-"                       # Disable buffer security check (stack cookies)
    "/guard:cf-"                 # Disable Control Flow Guard
)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# --- Common compiler options for all configurations ---
target_compile_options(
    "${PROJECT_NAME}"
    PRIVATE
    /MP
    $<$<BOOL:${GTS_STRICT_COMPILE}>:/W4;/WX>
    $<$<NOT:$<BOOL:${GTS_STRICT_COMPILE}>>:/W1>
    /permissive-
    /Zc:alignedNew
    /Zc:auto
    /Zc:__cplusplus
    /Zc:externC
    /Zc:externConstexpr
    /Zc:forScope
    /Zc:hiddenFriend
    /Zc:implicitNoexcept
    /Zc:lambda
    /Zc:noexceptTypes
    /Zc:preprocessor
    /Zc:referenceBinding
    /Zc:rvalueCast
    /Zc:sizedDealloc
    /Zc:strictStrings
    /Zc:ternary
    /Zc:threadSafeInit
    /Zc:trigraphs
    /Zc:wchar_t
    /wd4200 # nonstandard extension used : zero-sized array in struct/union
    /wd4100 # 'identifier' : unreferenced formal parameter
    /wd4101 # 'identifier': unreferenced local variable
    /wd4458 # declaration of 'identifier' hides class member
    /wd4459 # declaration of 'identifier' hides global declaration
    /wd4456 # declaration of 'identifier' hides previous local declaration
    /wd4457 # declaration of 'identifier' hides function parameter
    /wd4189 # 'identifier' : local variable is initialized but not referenced
)

# --- Debug-specific compiler options ---
target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<CONFIG:DEBUG>:/Zi>
    $<$<CONFIG:DEBUG>:/Od>
    $<$<CONFIG:DEBUG>:/RTC1>
    $<$<CONFIG:DEBUG>:/EHsc>
    $<$<CONFIG:DEBUG>:/fp:strict>
)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${SC_RELEASE_OPTS}>")

# --- Linker Options ---
target_link_options(
	${PROJECT_NAME}
	PRIVATE
	/WX
	"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
	"$<$<CONFIG:RELEASE>:/LTCG;/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
)

add_subdirectory(${CommonLibPath} ${CommonLibName} EXCLUDE_FROM_ALL)

if(CMAKE_GENERATOR MATCHES "Visual Studio" AND TARGET CommonLibSSE)
    set_target_properties(CommonLibSSE PROPERTIES
        FOLDER "ExternalDependencies"
    )
endif()

find_package(spdlog CONFIG REQUIRED)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/cmake
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	CommonLibSSE::CommonLibSSE
)
