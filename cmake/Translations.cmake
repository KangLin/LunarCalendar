SET(TS_FILES
    Src/Resource/Translations/Tasks_zh_CN.ts
    Src/Resource/Translations/Tasks_zh_TW.ts
    App/Resource/Translations/TasksApp_zh_CN.ts
    App/Resource/Translations/TasksApp_zh_TW.ts
    )

OPTION(OPTION_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${OPTION_TRANSLATIONS}\n")
IF(OPTION_TRANSLATIONS)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        #qt5_create_translation(QM_FILES ${SOURCES_FILES} ${SOURCE_UI_FILES} ${TS_FILES}) #生成 .ts 文件与 .qm 文件，w仅当没有TS文件的时候用。
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源 .qm 文件
        
        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})
        add_dependencies(${PROJECT_NAME} translations)
        
        if(ANDROID)
            file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc"
                "<!DOCTYPE RCC>
                <RCC version=\"1.0\">
                <qresource prefix=\"/Translations\">
                ")
            foreach(qm ${QM_FILES})
                get_filename_component(qm_name ${qm} NAME)
                file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc"
                    "    <file alias=\"${qm_name}\">${qm}</file>\n")
            endforeach(qm)
            file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc"
                "  </qresource>
                </RCC>
                ")
        else()
            install(FILES ${QM_FILES} DESTINATION "Translations")
        endif()

    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_TRANSLATIONS)
