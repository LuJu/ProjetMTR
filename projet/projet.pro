QT          += opengl xml widgets

#   UTILS   ###############
HEADERS     = \
    utils/triplet.h \
    utils/extendedtriplet.h \
    utils/geometry.h \
    utils/maths.h \
    utils/csvparser.h \
    utils/typedefinitions.h \
    physics/bodyinfo.h \
    physics/humanbody.h
SOURCES     = \
    main.cpp \
    utils/maths.cpp \
    utils/csvparser.cpp \
    physics/bodyinfo.cpp \
    physics/humanbody.cpp
############################

#   CORE     ##############
HEADERS     += \
    core/globalconfig.h \
    core/inputmanager.h \
    core/uistate.h \
    core/debugger.h \
    core/timing.h \
    core/core.h
SOURCES     += \
    core/globalconfig.cpp \
    core/inputmanager.cpp \
    core/uistate.cpp \
    core/debugger.cpp \
    core/timing.cpp
############################

#   3D         #############
HEADERS += \
    3d/graphicobject3d.h \
    3d/vertex.h \
    3d/transform.h \
    3d/mesh.h
SOURCES     += \
    3d/graphicobject3d.cpp \
    3d/vertex.cpp \
    3d/transform.cpp \
    3d/mesh.cpp
############################

#   PHYSICS        #########
HEADERS += \
    physics/interactiveobject.h \
    physics/simulationparameters.h \
    physics/simulation.h \
    physics/animationdata.h \
    physics/curve.h
SOURCES     += \
    physics/interactiveobject.cpp \
    physics/simulationparameters.cpp \
    physics/simulation.cpp \
    physics/animationdata.cpp \
    physics/curve.cpp
############################


#   PROGRAM  ###############
HEADERS += \
    viewer.h \
    testscene.h
SOURCES     += \
    viewer.cpp \
    testscene.cpp
############################



INSTALLS += target sources

unix:!macx:!symbian: LIBS += -L$usr/lib/x86_64-linux-gnu/
#INCLUDEPATH += $/usr/local/include/bullet/LinearMath
#INCLUDEPATH += $/usr/lib/x86_64-linux-gnu

INCLUDEPATH += /usr/local/include/bullet
DEPENDPATH += /usr/local/include/bullet
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
}
