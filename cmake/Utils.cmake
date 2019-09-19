include (CMakeParseArguments)

#产生android平台分发设置
function(GENERATED_DEPLOYMENT_SETTINGS)
    cmake_parse_arguments(PARA "" "NAME;APPLACTION" "" ${ARGN})

    if(NOT ANDROID_NDK)
        set(ANDROID_NDK $ENV{ANDROID_NDK})
        if(NOT ANDROID_NDK)
            set(ANDROID_NDK ${ANDROID_NDK_ROOT})
            if(NOT ANDROID_NDK)
                set(ANDROID_NDK $ENV{ANDROID_NDK_ROOT})
            endif()
        endif()
    endif()

    if(NOT ANDROID_SDK)
        set(ANDROID_SDK $ENV{ANDROID_SDK})
        if(NOT ANDROID_SDK)
            set(ANDROID_SDK ${ANDROID_SDK_ROOT})
            if(NOT ANDROID_SDK)
                set(ANDROID_SDK $ENV{ANDROID_SDK_ROOT})
            endif()
        endif()
    endif()

    if(DEFINED PARA_NAME)
        set(_file_name ${PARA_NAME})
        #message("file_name:${PARA_NAME}")
    else()
        SET(_file_name "${PROJECT_BINARY_DIR}/android-lib${PROJECT_NAME}.so-deployment-settings.json")
    endif()

    FILE(WRITE ${_file_name} "{\n")
    FILE(APPEND ${_file_name} "\"description\": \"This file is generated by qmake to be read by androiddeployqt and should not be modified by hand.\",\n")
    FILE(APPEND ${_file_name} "\"qt\":\"${QT_INSTALL_DIR}\",\n")
    FILE(APPEND ${_file_name} "\"sdk\":\"${ANDROID_SDK}\",\n")
    FILE(APPEND ${_file_name} "\"ndk\":\"${ANDROID_NDK}\",\n")

    if(Qt5_VERSION VERSION_GREATER 5.13.0)
        FILE(APPEND ${_file_name} "\"stdcpp-path\":\"${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/libs/${ANDROID_ABI}/libc++_shared.so\",\n")
        FILE(APPEND ${_file_name} "\"useLLVM\":true,\n")
        FILE(APPEND ${_file_name} "\"toolchain-prefix\":\"llvm\",\n")
        FILE(APPEND ${_file_name} "\"tool-prefix\":\"llvm\",\n")
    else()
        FILE(APPEND ${_file_name} "\"toolchain-prefix\":\"${CMAKE_CXX_ANDROID_TOOLCHAIN_PREFIX}\",\n")
        FILE(APPEND ${_file_name} "\"tool-prefix\":\"${CMAKE_CXX_ANDROID_TOOLCHAIN_PREFIX}\",\n")
        FILE(APPEND ${_file_name} "\"toolchain-version\":\"${CMAKE_CXX_COMPILER_VERSION}\",\n")
    endif()

    IF(CMAKE_HOST_WIN32)
        IF(ANDROID_NDK_HOST_X64)
           FILE(APPEND ${_file_name} "\"ndk-host\":\"windows-x86_64\",\n")
	ELSE()
           FILE(APPEND ${_file_name} "\"ndk-host\":\"windows\",\n")
	ENDIF()
    ELSE()
        IF(ANDROID_NDK_HOST_X64)
            FILE(APPEND ${_file_name} "\"ndk-host\":\"linux-x86_64\",\n")
	ELSE()
	    FILE(APPEND ${_file_name} "\"ndk-host\":\"linux\",\n")
        ENDIF()
    ENDIF()
    FILE(APPEND ${_file_name} "\"target-architecture\":\"${CMAKE_ANDROID_ARCH_ABI}\",\n")
    FILE(APPEND ${_file_name} "\"android-package-source-directory\":\"${PROJECT_SOURCE_DIR}/android\",\n")
    IF(ANDROID_EXTRA_LIBS)
        FILE(APPEND ${_file_name} "\"android-extra-libs\":\"${ANDROID_EXTRA_LIBS}\",\n")
    ENDIF(ANDROID_EXTRA_LIBS)
    if(DEFINED PARA_APPLACTION)
        FILE(APPEND ${_file_name} "\"application-binary\":\"${PARA_APPLACTION}\"\n")
        #message("app_bin:${PARA_APPLACTION}")
    else()
        FILE(APPEND ${_file_name} "\"application-binary\":\"${CMAKE_BINARY_DIR}/bin/lib${PROJECT_NAME}.so\"\n")
    endif()
    FILE(APPEND ${_file_name} "}")
endfunction(GENERATED_DEPLOYMENT_SETTINGS)
