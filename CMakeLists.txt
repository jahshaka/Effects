project(MaterialNodeGraph)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

add_subdirectory(irisgl)
set_target_properties(IrisGL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CPP
	main.cpp
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
	# nodes
	nodes/test.cpp
	nodes/libraryv1.cpp
	nodes/math.cpp
	nodes/utils.cpp
	nodes/inputs.cpp
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
		# nodes
		nodes/test.h
		nodes/libraryv1.h
		nodes/math.h
		nodes/utils.h
		nodes/inputs.h
	)

set(FORMS
        mainwindow.ui
		materialsettingswidget.ui
        propertylistwidget.ui)

Qt5_add_resources(QRCS
		images.qrc
		icons.qrc)


set(LIBS IrisGL Qt5::Widgets)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#qt5_wrap_cpp(HEADERS_moc ${HEADERS})
#qt5_wrap_ui(UI_moc ${FORMS})

add_executable(MaterialNodeGraph ${HEADERS} ${CPP} ${FORMS} ${QRCS})
include_directories(MaterialNodeGraph "irisgl/include")
target_link_libraries(MaterialNodeGraph ${LIBS})


if (WIN32)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")


    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
endif()
