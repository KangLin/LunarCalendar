project(LunarCalendarApp)

set(HEAD_FILES
    MainWindow.h 
    )
set(SOURCE_FILES
    main.cpp 
    MainWindow.cpp 
    )
set(SOURCE_UI_FILES
    MainWindow.ui
    )

#翻译
include(${CMAKE_SOURCE_DIR}/cmake/Qt5CorePatches.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)

set(RESOURCE_FILES ${RESOURCE_FILES}
    Resource/Resource.qrc
    "${TRANSLATIONS_RESOURCE_FILES}")

add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")

if(ANDROID)
    add_library(${PROJECT_NAME} ${SOURCE_FILES}
        ${HEAD_FILES} ${SOURCE_UI_FILES} ${RESOURCE_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES}
        ${HEAD_FILES} ${SOURCE_UI_FILES} ${RESOURCE_FILES} AppIcon.rc)
endif()
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
add_dependencies(${TRANSLATIONS_NAME} translations_${TRANSLATIONS_NAME})
if(MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-mwindows")
elseif(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE
        "/SUBSYSTEM:WINDOWS\",5.01\" /ENTRY:mainCRTStartup")
endif()
SET(APP_LIBS LunarCalendar ${QT_LIBRARIES})
if(TARGET RabbitCommon)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DRABBITCOMMON)
    target_include_directories(${PROJECT_NAME}
        PRIVATE "${RabbitCommon_DIR}/Src")
    set(APP_LIBS ${APP_LIBS} RabbitCommon)
endif()
target_link_libraries(${PROJECT_NAME} ${APP_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/Src" "${CMAKE_BINARY_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    VERSION ${BUILD_VERSION})
if(UNIX AND NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${PROJECT_NAME})
endif()

# Install target
if(ANDROID)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
elseif(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
else()
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
endif()
if(ANDROID)
    # Install other files
    INSTALL(FILES ${OTHER_FILES} DESTINATION "assets" COMPONENT Runtime)    
else()
    # Install other files
    INSTALL(FILES ${OTHER_FILES} DESTINATION "." COMPONENT Runtime)
endif()

if(UNIX)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/LunarCalendar.desktop
        DESTINATION "share/applications" COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/LunarCalendar.png
        DESTINATION "share/pixmaps" COMPONENT Runtime)
endif()

IF("Release" STREQUAL CMAKE_BUILD_TYPE AND WIN32)
    SET(RUNTIME_FILE "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
    IF(CMAKE_BUILD_TYPE MATCHES Release AND MINGW)
        #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND strip "$<TARGET_FILE:${PROJECT_NAME}>"
            )
    ENDIF()

    #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中  
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
        --compiler-runtime
        --verbose 7
        "$<TARGET_FILE:${PROJECT_NAME}>"
        )
    INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Runtime)
ENDIF()

IF(ANDROID)
    Set(JSON_FILE ${CMAKE_CURRENT_BINARY_DIR}/android-libLunarCalendarApp.so-deployment-settings.json)
    GENERATED_DEPLOYMENT_SETTINGS(NAME ${JSON_FILE}
        APPLACTION "${CMAKE_BINARY_DIR}/bin/lib${PROJECT_NAME}.so")

    add_custom_target(APK #TARGET ${PROJECT_NAME} POST_BUILD
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" ${CMAKE_INSTALL_PREFIX}/libs/${ANDROID_ABI}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/libs/${ANDROID_ABI}
        #COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:${PROJECT_NAME}>/*" ${CMAKE_INSTALL_PREFIX}/libs/${ANDROID_ABI}
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
        --output ${CMAKE_INSTALL_PREFIX}
        --input ${JSON_FILE}
        --verbose
        --gradle
        )

ENDIF(ANDROID)
