include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/xup
TARGET	= XUP
QT	*= xml
RESOURCES	*= src/resources/XUPManager.qrc src/qmakemanager/resources/qmakemanager.qrc
ALL_INCLUDES	= $$INCLUDEPATH
INCLUDEPATH	= . src src/xupmanager src/xupmanager/ui src/qmakemanager $$ALL_INCLUDES

FORMS	*= src/xupmanager/ui/UIXUPManager.ui \
	src/xupmanager/ui/UIXUPProjectEditor.ui \
	src/xupmanager/ui/UIAddVariable.ui

HEADERS	*= src/xupmanager/ProjectItemModel.h \
	src/xupmanager/XUPItem.h \
	src/xupmanager/XUPManager.h \
	src/xupmanager/ui/UIXUPManager.h \
	src/qmakemanager/QMakeXUPItem.h \
	src/qmakemanager/QMake2XUP.h \
	src/xupmanager/FilteredProjectItemModel.h \
	src/xupmanager/ui/AddFilesDialog.h \
	src/xupmanager/ScopedProjectItemModel.h \
	src/xupmanager/XUPPlugin.h \
	src/XUP.h \
	src/xupmanager/ui/UIXUPProjectEditor.h \
	src/xupmanager/ProjectEditorModel.h \
	src/xupmanager/ui/UIAddVariable.h

SOURCES	*= src/xupmanager/ProjectItemModel.cpp \
	src/xupmanager/XUPItem.cpp \
	src/xupmanager/XUPManager.cpp \
	src/xupmanager/ui/UIXUPManager.cpp \
	src/qmakemanager/QMakeXUPItem.cpp \
	src/qmakemanager/QMake2XUP.cpp \
	src/xupmanager/FilteredProjectItemModel.cpp \
	src/xupmanager/ui/AddFilesDialog.cpp \
	src/xupmanager/ScopedProjectItemModel.cpp \
	src/XUP.cpp \
	src/xupmanager/ui/UIXUPProjectEditor.cpp \
	src/xupmanager/ProjectEditorModel.cpp \
	src/xupmanager/ui/UIAddVariable.cpp