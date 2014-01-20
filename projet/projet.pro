QT          += opengl xml widgets

SOURCES     = \
    main.cpp \
    physics/debuggingwidget.cpp \
    physics/debugtimer.cpp

#   UTILS   ###############
HEADERS     += \
    /home/batman/Programmation/QtDee/utils/utils.h \
    /home/batman/Programmation/QtDee/utils/triplet.h \
    /home/batman/Programmation/QtDee/utils/extendedtriplet.h \
    /home/batman/Programmation/QtDee/utils/geometry.h \
    /home/batman/Programmation/QtDee/utils/maths.h \
    /home/batman/Programmation/QtDee/utils/csvparser.h \
    /home/batman/Programmation/QtDee/utils/typedefinitions.h \
    /home/batman/Programmation/QtDee/utils/curve.h \
    physics/debuggingwidget.h \
    physics/debugtimer.h
SOURCES     += \
    /home/batman/Programmation/QtDee/utils/maths.cpp \
    /home/batman/Programmation/QtDee/utils/csvparser.cpp \
    /home/batman/Programmation/QtDee/utils/curve.cpp
############################

#   3D         #############
HEADERS += \
    /home/batman/Programmation/QtDee/3d/3d.h \
    /home/batman/Programmation/QtDee/3d/graphicobject3d.h \
    /home/batman/Programmation/QtDee/3d/vertex.h \
    /home/batman/Programmation/QtDee/3d/transform.h \
    /home/batman/Programmation/QtDee/3d/movementpath.h \
    /home/batman/Programmation/QtDee/3d/mesh.h
SOURCES     += \
    /home/batman/Programmation/QtDee/3d/graphicobject3d.cpp \
    /home/batman/Programmation/QtDee/3d/vertex.cpp \
    /home/batman/Programmation/QtDee/3d/transform.cpp \
    /home/batman/Programmation/QtDee/3d/movementpath.cpp \
    /home/batman/Programmation/QtDee/3d/mesh.cpp
############################

#   CORE     ##############
HEADERS     += \
    /home/batman/Programmation/DeeCore/core/globalconfig.h \
    /home/batman/Programmation/DeeCore/core/inputmanager.h \
    /home/batman/Programmation/DeeCore/core/uistate.h \
    /home/batman/Programmation/DeeCore/core/debugger.h \
    /home/batman/Programmation/DeeCore/core/timing.h \
    /home/batman/Programmation/DeeCore/core/viewer.h \
    /home/batman/Programmation/DeeCore/core/camera.h \
    /home/batman/Programmation/DeeCore/core/core.h
SOURCES     += \
    /home/batman/Programmation/DeeCore/core/globalconfig.cpp \
    /home/batman/Programmation/DeeCore/core/inputmanager.cpp \
    /home/batman/Programmation/DeeCore/core/uistate.cpp \
    /home/batman/Programmation/DeeCore/core/debugger.cpp \
    /home/batman/Programmation/DeeCore/core/viewer.cpp \
    /home/batman/Programmation/DeeCore/core/camera.cpp \
    /home/batman/Programmation/DeeCore/core/timing.cpp
############################

#   PHYSICS        #########
HEADERS += \
    physics/interactiveobject.h \
    physics/simulationparameters.h \
    physics/simulation.h \
    physics/animationdata.h \
    physics/bodyinfo.h \
    physics/humanbody.h \
    physics/debugginginterface.h
SOURCES     += \
    physics/interactiveobject.cpp \
    physics/simulationparameters.cpp \
    physics/simulation.cpp \
    physics/animationdata.cpp \
    physics/bodyinfo.cpp \
    physics/humanbody.cpp \
    physics/debugginginterface.cpp
############################


#   PROGRAM  ###############
HEADERS += \
    testscene.h
SOURCES     += \
    testscene.cpp
############################



INSTALLS += target sources

unix:!macx:!symbian: LIBS += -L$usr/lib/x86_64-linux-gnu/
#INCLUDEPATH += $/usr/local/include/bullet/LinearMath
#INCLUDEPATH += $/usr/lib/x86_64-linux-gnu

INCLUDEPATH += /usr/local/include/bullet
DEPENDPATH += /usr/local/include/bullet
INCLUDEPATH += /home/batman/Programmation/QtDee
DEPENDPATH += /home/batman/Programmation/QtDee
INCLUDEPATH += /home/batman/Programmation/DeeCore
DEPENDPATH += /home/batman/Programmation/DeeCore
#INCLUDEPATH += /usr/local/include/bullet/LinearMath
# Linux
#INCLUDEPATH *= $/usr/include/QGLViewer
LIBS *= -L/usr/lib -L/usr/local/lib -lQGLViewer -lGLU -lglut  -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody
#FORMS += \
#dialog.ui

OTHER_FILES += \
    ../assets/shaders/fshader.glsl \
    ../assets/shaders/vshader.glsl \
    ../assets/shaders/gshader.glsl \
    ../../../.config/Windel/ETS.conf

CONFIG(debug,debug|release){
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

FORMS += \
    physics/Debugging.ui

RESOURCES += \
    ../assets/ressources.qrc
