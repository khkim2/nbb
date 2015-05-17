cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0020 NEW)
if (POLICY CMP0043)
	cmake_policy(SET CMP0043 NEW)
endif (POLICY CMP0043)

project(nbb)

# Conditional build with external dependencies
set(USE_OPENCV ON CACHE BOOL "Use OpenCV")
set(USE_CUDA ON CACHE BOOL "Use CUDA")
set(BUILD_TEST ON CACHE BOOL "Build unit tests")
set(BUILD_EXAMPLE OFF CACHE BOOL "Build examples")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

# Project header directory
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")

# Compiler option for gcc
if (${CMAKE_COMPILER_IS_GNUCXX})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
endif (${CMAKE_COMPILER_IS_GNUCXX})

# Compiler options for VC++
if (${MSVC})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS /wd4819")
endif (${MSVC})

# # numcpp library
# add_library(numcpp
# 	numcpp.cpp

# 	numcpp/allocator.h
# 	numcpp/base_array.h
# 	numcpp/array.h
# 	numcpp/array_functions.h
# 	numcpp/gpu_array.h
# 	numcpp/opencv.h)

# External library: OpenCV
if (USE_OPENCV)
    set(CMAKE_PREFIX_PATH "$ENV{HOME}/opencv")
	set(OpenCV_STATIC OFF)

	find_package(OpenCV REQUIRED)
	# target_link_libraries(numcpp ${OpenCV_LIBS})
endif (USE_OPENCV)

# External library: NVIDIA CUDA
if (USE_CUDA)
	find_package(CUDA REQUIRED)
	include_directories(${CUDA_INCLUDE_DIRS})
	# target_link_libraries(numcpp ${CUDA_LIBRARIES})

	if (${UNIX})
		# Use C++03 compiler, not C++11
		set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-Xcompiler -std=c++03)
	endif (${UNIX})
endif (USE_CUDA)

if (BUILD_TEST)
	# Use google test
	set(gtest_force_shared_crt ON CACHE BOOL "Use shared crt")
	add_subdirectory(deps/google-test)
	include_directories(deps/google-test/include)

	if (USE_CUDA)
		cuda_compile(TEST_CUDA_KERNEL_O numcpp/test_cuda_kernel.cu)
	endif (USE_CUDA)

	# test executable
	add_executable(numcpp.test 
		test.cpp

		numcpp/base_array.test.inl
		numcpp/array.test.inl
		numcpp/gpu_array.test.inl
		${TEST_CUDA_KERNEL_O}
		numcpp/opencv.test.inl
	)

	target_link_libraries(numcpp.test numcpp gtest)

	# Copy files for test
	file(COPY images/Lena.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endif (BUILD_TEST)

if (BUILD_EXAMPLE)
	# Example: circularize
	if (USE_OPENCV)
		add_executable(circularize example/circularize.h example/circularize.cpp)
		target_link_libraries(circularize numcpp)
	endif (USE_OPENCV)
endif (BUILD_EXAMPLE)

# ----------

# Qt
set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
find_package(Qt5Widgets REQUIRED)

# jsoncpp
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "jsoncpp with tests")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "jsoncpp with post build unittest")
include_directories(deps/jsoncpp/include)
add_subdirectory(deps/jsoncpp)

# Library: nbb
qt5_wrap_ui(UI_FILES nbb/ContextWindow.ui)
qt5_add_resources(QRC_FILES nbb/ContextWindow.qrc)

add_library(nbb
	nbb/signal.h
	nbb/property.h
	nbb/Object.h
	nbb/Object.cpp
	nbb/Context.h
	nbb/Context.cpp
	nbb/QuickDialog.h
	nbb/QuickDialog.cpp
	nbb/QuickJSON.h
	nbb/QuickJSON.cpp

	nbb/ContextWindow.h
	nbb/ContextWindow.cpp
	${UI_FILES}
	${QRC_FILES}
)

target_link_libraries(nbb jsoncpp_lib)
qt5_use_modules(nbb Core Widgets)

# Auto moc
set_target_properties(nbb PROPERTIES AUTOMOC TRUE)

if (BUILD_TEST)
	# test executable
	add_executable(nbb.test nbb/test.cpp)

	target_link_libraries(nbb.test nbb)
endif (BUILD_TEST)
