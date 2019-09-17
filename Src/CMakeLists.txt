project(LunarCalendar)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

#需要的QT组件
SET(QT_COMPONENTS Core Gui Widgets Sql ${QT_COMPONENTS})
find_package(Qt5 COMPONENTS ${QT_COMPONENTS})
FOREACH(_COMPONENT ${QT_COMPONENTS})
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()

SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")

set(INSTALL_HEAD_FILES
    LunarCalendar.h
    )
set(HEAD_FILES
    ${INSTALL_HEAD_FILES}
    LunarCalendarModel.h 
    CalendarLunar.h 
    ../3th_lib/sxtwl/src/lunar.h 
    ../3th_lib/sxtwl/src/eph.h 
    ../3th_lib/sxtwl/src/JD.h 
    ../3th_lib/sxtwl/src/SSQ.h
    ../3th_lib/sxtwl/src/XL.h  
    LunarCalendarDelegate.h
    LunarTable.h
    )
set(SOURCE_FILES
    LunarCalendar.cpp
    LunarCalendarModel.cpp
    CalendarLunar.cpp
    ../3th_lib/sxtwl/src/lunar.cpp
    ../3th_lib/sxtwl/src/eph.cpp
    ../3th_lib/sxtwl/src/JD.cpp
    ../3th_lib/sxtwl/src/SSQ.cpp
    ../3th_lib/sxtwl/src/XL.cpp  
    LunarCalendarDelegate.cpp
    LunarTable.cpp
    )
set(SOURCE_UI_FILES
    )

#翻译
include(${CMAKE_SOURCE_DIR}/cmake/Qt5CorePatches.cmake) 
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)

#设置资源文件
SET(RCC_FILES
    Resource/ResourceLunarCalendar.qrc
    ${TRANSLATIONS_RESOURCE_FILES}
    )

IF(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES}
    ${SOURCE_UI_FILES} ${RCC_FILES})
add_dependencies(${TRANSLATIONS_NAME} translations_${TRANSLATIONS_NAME})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} RabbitCommon)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../3th_lib/sxtwl/src
    ${RabbitCommon_DIR}/Src)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
SET(INSTALL_HEAD_FILES ${INSTALL_HEAD_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

# Install head files
#INSTALL(FILES ${INSTALL_HEAD_FILES} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
set_property(
    TARGET
        ${PROJECT_NAME}
    PROPERTY
        PUBLIC_HEADER "${INSTALL_HEAD_FILES}"
)

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Config
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LunarCalendar
)
export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
)
# Install cmake configure files
install(
        EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")
if(NOT MSVC)
    # Install pc files
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/LunarCalendar.pc.in
        ${CMAKE_BINARY_DIR}/LunarCalendar.pc @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/LunarCalendar.pc
        DESTINATION lib/pkgconfig)
endif()
IF("Release" STREQUAL CMAKE_BUILD_TYPE AND WIN32 AND BUILD_SHARED_LIBS)
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
    #INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/" DESTINATION "${CMAKE_INSTALL_BINDIR}")
ENDIF()

#Install dababase file
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/database/db.sqlite DESTINATION "./data/db")
