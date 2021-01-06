QT += core gui widgets multimedia qml

CONFIG += c++17

SOURCES += \
    Clipboard.cpp \
    ClipboardItem.cpp \
    LibraryDelegate.cpp \
    LibraryItem.cpp \
    LibraryModel.cpp \
    LibraryWidget.cpp \
    MainWidget.cpp \
    PatternItem.cpp \
    PatternPlayer.cpp \
    PatternScene.cpp \
    PatternSelection.cpp \
    PatternSizeEditWidget.cpp \
    PatternView.cpp \
    PatternViewBackgroundPainter.cpp \
    PatternViewForegroundPainter.cpp \
    PatternViewPasteHandler.cpp \
    PatternViewSelectionHandler.cpp \
    PatternWidget.cpp \
    PianoProperties.cpp \
    ProjectFile.cpp \
    QtDarkStyle/DarkStyle.cpp \
    QtTools.cpp \
    ScriptEditWidget.cpp \
    ScriptEngine.cpp \
    ScriptSyntaxHighlighter.cpp \
    SignalerGraphicsView/CreateHandler.cpp \
    SignalerGraphicsView/DeleteHandler.cpp \
    SignalerGraphicsView/ResizeHandler.cpp \
    SignalerGraphicsView/SignalerGraphicsView.cpp \
    SignalerGraphicsView/ZoomHandler.cpp \
    SoundExporter.cpp \
    SoundStream/SoundStream.cpp \
    Synth.cpp \
    SynthFactory.cpp \
    SynthMode.cpp \
    SynthSaw.cpp \
    SynthSin.cpp \
    SynthSquare.cpp \
    SynthTriangle.cpp \
    main.cpp \

HEADERS += \
    ValueSlider/ValueSlider.h \
    Clipboard.h \
    ClipboardItem.h \
    CoreProperties.h \
    EnumClass/enum.h \
    LibraryDelegate.h \
    LibraryItem.h \
    LibraryModel.h \
    LibraryWidget.h \
    MainWidget.h \
    PatternItem.h \
    PatternPlayer.h \
    PatternProperties.h \
    PatternScene.h \
    PatternSelection.h \
    PatternSizeEditWidget.h \
    PatternStyle.h \
    PatternView.h \
    PatternViewBackgroundPainter.h \
    PatternViewForegroundPainter.h \
    PatternViewPasteHandler.h \
    PatternViewSelectionHandler.h \
    PatternWidget.h \
    PianoProperties.h \
    PianoRollProperties.h \
    ProjectFile.h \
    QtDarkStyle/DarkStyle.h \
    QtTools.h \
    ScriptColorScheme.h \
    ScriptEditWidget.h \
    ScriptEngine.h \
    ScriptKeyword.h \
    ScriptSyntaxHighlighter.h \
    SignalerGraphicsView/CreateHandler.h \
    SignalerGraphicsView/DeleteHandler.h \
    SignalerGraphicsView/ResizeHandler.h \
    SignalerGraphicsView/ResizeMode.h \
    SignalerGraphicsView/SignalerGraphicsView.h \
    SignalerGraphicsView/ZoomHandler.h \
    SoundExporter.h \
    SoundProperties.h \
    SoundStream/SoundStream.h \
    Synth.h \
    SynthFactory.h \
    SynthMode.h \
    SynthSaw.h \
    SynthSin.h \
    SynthSquare.h \
    SynthTriangle.h \
    SynthType.h \
    core_types.h \
    properties.h

FORMS += \
    LibraryWidget.ui \
    MainWidget.ui \
    PatternWidget.ui \
    ScriptEditWidget.ui

RESOURCES += \
    qjsynth.qrc

DISTFILES += \
    SignalerGraphicsView/README.md
