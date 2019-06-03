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

if(ANDROID)
    set(RESOURCE_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/ResourceAbout.qrc")
    
    file(WRITE "${RESOURCE_FILE_NAME}"
        "<!DOCTYPE RCC>
        <RCC version=\"1.0\">
        <qresource prefix=\"/file\">
        ")
    foreach(f ${OTHER_FILES})
        get_filename_component(file_name ${f} NAME)
        file(APPEND "${RESOURCE_FILE_NAME}"
            "    <file alias=\"${file_name}\">${f}</file>\n")
    endforeach(f)
    file(APPEND "${RESOURCE_FILE_NAME}"
        "  </qresource>
        </RCC>
        ")
    set(RESOURCE_FILES "${RESOURCE_FILE_NAME}")
    
else()
    INSTALL(FILES ${OTHER_FILES} DESTINATION ".")
endif()

#翻译
include(${CMAKE_SOURCE_DIR}/cmake/Qt5CorePatches.cmake) 
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)

set(RESOURCE_FILES ${RESOURCE_FILES} 
    Resource/Resource.qrc
    "${TRANSLATIONS_RESOURCE_FILES}")

add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()
add_executable(${PROJECT_NAME} ${SOURCE_FILES}
    ${HEAD_FILES} ${SOURCE_UI_FILES} ${RESOURCE_FILES} AppIcon.rc)
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
# Install target
INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
    
# Install other files
INSTALL(FILES ${OTHER_FILES} DESTINATION ".")

if(UNIX)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/LunarCalendar.desktop DESTINATION "share/applications")
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/LunarCalendar.png DESTINATION "share/pixmaps")
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
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
ENDIF()

IF(ANDROID)
    include(${CMAKE_SOURCE_DIR}/cmake/Android.cmake)
    GENERATED_DEPLOYMENT_SETTINGS()
    find_program(ANT NAMES ant) # PATHS  "/usr/bin")
    MESSAGE("ant:${ANT}\n")
    find_program(GRADLE NAMES gradle)
    MESSAGE("GRADLE:${GRADLE}\n")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
            --input "${PROJECT_BINARY_DIR}/android-${PROJECT_NAME}-deployment-settings.json"
            --output ${CMAKE_BINARY_DIR}/android-build
            --verbose
            --gradle
            #--ant /usr/bin/ant
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)
