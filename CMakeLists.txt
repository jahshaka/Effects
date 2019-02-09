cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(ShaderGraph)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EFFECT_BUILD_AS_LIB false CACHE BOOL "Builds the lib as a static library to be embedded in Jahshaka")

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Sql)

if (NOT EFFECT_BUILD_AS_LIB)
	add_subdirectory(irisgl)
endif (NOT EFFECT_BUILD_AS_LIB)

set_target_properties(IrisGL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CPP
	#main.cpp
	mainwindow.cpp
	#nodegraph.cpp
    #nodemodel.cpp
    scenewidget.cpp
    propertylistwidget.cpp
	materialsettingswidget.cpp
    propertywidgets/vectorpropertywidget.cpp
    propertywidgets/intpropertywidget.cpp
    propertywidgets/floatpropertywidget.cpp
    propertywidgets/texturepropertywidget.cpp
    propertywidgets/basepropertywidget.cpp
    propertywidgets/propertywidgetbase.cpp
    properties.cpp
    materialwriter.cpp
    texturemanager.cpp
	socketconnection.cpp
	socket.cpp
	graphnode.cpp
	graphnodescene.cpp
    #graphtest.cpp
	# graph
	graph/nodegraph.cpp
	graph/nodemodel.cpp
	graph/connectionmodel.cpp
	graph/socketmodel.cpp
	graph/sockethelper.cpp
	graph/library.cpp
	graph/graphicsview.cpp

	# generator
	generator/shadergenerator.cpp
	generator/shadercontext.cpp
	listwidget.cpp
	shaderlistwidget.cpp
	# nodes
	nodes/test.cpp
	nodes/libraryv1.cpp
	nodes/math.cpp
	nodes/utils.cpp
	nodes/inputs.cpp
	nodes/object.cpp
	nodes/vector.cpp
	nodes/texture.cpp

	dialogs/createnewdialog.cpp
	dialogs/searchdialog.cpp
	core/project.cpp
	core/materialhelper.cpp
	core/undoredo.cpp
	misc/QtAwesome.cpp
	misc/QtAwesomeAnim.cpp
	widgets/treewidget.cpp

	)

set(HEADERS
	mainwindow.h
        properties.h
	#nodegraph.h
        scenewidget.h
        propertylistwidget.h
        materialsettingswidget.h
        propertywidgets/floatpropertywidget.h
        propertywidgets/vectorpropertywidget.h
        propertywidgets/intpropertywidget.h
        propertywidgets/texturepropertywidget.h
        propertywidgets/basepropertywidget.h
        propertywidgets/propertywidgetbase.h
        materialwriter.h
		texturemanager.h
		socketconnection.h
		socket.h
		graphnode.h
		graphnodescene.h
        #graphtest.h
		#graph
		graph/nodegraph.h
		graph/nodemodel.h
		graph/connectionmodel.h
		graph/socketmodel.h
		graph/sockethelper.h
		graph/sockets.h
		graph/library.h
		graph/graphicsview.h
		# generator
		generator/shadergenerator.h
		generator/shadercontext.h
		listwidget.h
		shaderlistwidget.h
		# nodes
		nodes/test.h
		nodes/libraryv1.h
		nodes/math.h
		nodes/utils.h
		nodes/inputs.h
		nodes/object.h
		nodes/vector.h
		nodes/texture.h

		dialogs/createnewdialog.h
		dialogs/searchdialog.h
		core/project.h
		core/undoredo.h
		core/materialhelper.h
		misc/QtAwesome.h
		misc/QtAwesomeAnim.h
		widgets/treewidget.h
	)

set(FORMS
        mainwindow.ui
		materialsettingswidget.ui
        propertylistwidget.ui)

Qt5_add_resources(QRCS
		images.qrc
		icons.qrc
		fonts.qrc)


set(LIBS IrisGL Qt5::Widgets)


foreach(p CMP0071) # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    if(POLICY ${p})
        cmake_policy(SET ${p} NEW)
    endif()
endforeach()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#add_library(shadergraph STATIC ${HEADERS} ${CPP} ${FORMS} ${QRCS})
#target_link_libraries(shadergraph Qt5::Widgets Qt5::Sql)

if (EFFECT_BUILD_AS_LIB)

	add_definitions(-DBUILD_AS_LIB)

	add_library(shadergraph STATIC ${HEADERS} ${CPP} ${FORMS} ${QRCS})
	target_link_libraries(shadergraph Qt5::Widgets)
else(EFFECT_BUILD_AS_LIB)

	#qt5_wrap_cpp(HEADERS_moc ${HEADERS})
	#qt5_wrap_ui(UI_moc ${FORMS})

	set(CPP main.cpp ${CPP})
	add_executable(shadergraph ${HEADERS} ${CPP} ${FORMS} ${QRCS})
	include_directories(shadergraph "irisgl/include")
	target_link_libraries(shadergraph ${LIBS})

	if (WIN32)
	    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
	    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
	    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")


	    add_custom_command(TARGET shadergraph POST_BUILD
	        COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:shadergraph>)
	endif(WIN32)

	# Copy Jahshaka data folders after a successful build
	set(DataDirs assets)
	foreach(dir ${DataDirs})
		if (APPLE)
			add_custom_command(
				TARGET shadergraph POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
						${PROJECT_SOURCE_DIR}/${dir}
						${PROJECT_BINARY_DIR}/${APP_OUTPUT_NAME}.app/Contents/MacOS/${dir})
		else()
			add_custom_command(
				TARGET shadergraph POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
						${PROJECT_SOURCE_DIR}/${dir}
						${PROJECT_BINARY_DIR}/${dir})
		endif()
	endforeach()

endif(EFFECT_BUILD_AS_LIB)