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
	nodegraph.cpp
        nodemodel.cpp
        scenewidget.cpp
        propertylistwidget.cpp
        graphtest.cpp)

set(HEADERS
	mainwindow.h
        properties.h
	nodegraph.h
        nodemodel.h
        scenewidget.h
        propertylistwidget.h
        propertywidgets/floatpropertywidget.h
        graphtest.h)

set(FORMS
        mainwindow.ui
        propertylistwidget.ui)

set(LIBS IrisGL Qt5::Widgets)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_cpp(HEADERS_moc ${HEADERS})
qt5_wrap_ui(UI_moc ${FORMS})

add_executable(MaterialNodeGraph ${HEADERS} ${CPP})
include_directories(MaterialNodeGraph "irisgl/include")
target_link_libraries(MaterialNodeGraph ${LIBS})
