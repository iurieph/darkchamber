cmake_minimum_required(VERSION 3.16)
project(darkchamber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(Qt6Core_DIR "../../Qt/6.4.2/gcc_64/lib/cmake/Q65Core")
#set(Qt6Gui_DIR "../../Qt/6.4.2/gcc_64/lib/cmake/Qt6Gui")
#set(Qt6Widgets_DIR "../../Qt/6.4.2/gcc_64/lib/cmake/Qt6Widgets")
#set(Qt6_DIR "../../Qt/6.4.2/gcc_64/lib/cmake/Qt6")
#set(QT_QMAKE_EXECUTABLE "../../Qt/6.4.2/gcc_64/bin/qmake")

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(darkchamber
    DarkChamber.h
    RawImageInfo.h
    RawImageInfo.cpp
    ImageDecoder.h
    ImageDecoder.cpp
    DefaultImageDecoder.h
    DefaultImageDecoder.cpp
    LibRawImageDecoder.h
    LibRawImageDecoder.cpp
    ImageData.h
    ImageData.cpp
    PhotoStateFile.h
    PhotoStateFile.cpp
    PhotoItem.h
    PhotoItem.cpp
    PhotoModel.h
    PhotoModel.cpp
    LocationsView.h
    LocationsView.cpp
    EffectGroupBox.h
    EffectGroupBox.cpp
    ExposureWidget.h
    ExposureWidget.cpp
    DehazeWidget.h
    DehazeWidget.cpp
    ShadowsHighlightsWidget.h
    ShadowsHighlightsWidget.cpp
    VignetteWidget.h
    VignetteWidget.cpp
    NoiseReductionWidget.h
    NoiseReductionWidget.cpp
    DefringeWidget.h
    DefringeWidget.cpp
    WhiteBalanceWidget.h
    WhiteBalanceWidget.cpp
    EffectsWidget.h
    EffectsWidget.cpp
    Thumbnail.h
    Thumbnail.cpp
    PathWorker.h
    PathWorker.cpp
    PhotoWorker.h
    PhotoWorker.cpp
    PathLoader.h
    PathLoader.cpp
    PhotoBrowserModel.h
    PhotoBrowserModel.cpp
    PhotoBrowser.cpp
    PhotoBrowser.h
    PhotoBrowser.cpp
    PhotoViewer.h
    PhotoViewer.cpp
    PhotoEditorToolBar.h
    PhotoEditorToolBar.cpp
    PhotoEditor.h
    PhotoEditor.cpp
    MainWindow.cpp
    MainWindow.h
    Application.h
    Application.cpp
    main.cpp
    )

  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
  add_definitions(-fPIC)

target_link_libraries(darkchamber PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    raw
)
