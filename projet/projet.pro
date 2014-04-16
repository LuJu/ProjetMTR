QT          += opengl xml widgets

SOURCES     = \
    main.cpp

# library QtDee
#   UTILS   ###############
HEADERS     += \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/utils.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/triplet.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/extendedtriplet.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/geometry.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/maths.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/csvparser.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/typedefinitions.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/curve.h
SOURCES     += \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/maths.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/csvparser.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/utils/curve.cpp
############################


# library QtDee
#   3D         #############
HEADERS += \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/3d.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/graphicobject3d.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/vertex.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/transform.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/material.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/mesh.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/meshutils.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/movementpath.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/bezierpath.h \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/objloader.h

SOURCES     += \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/graphicobject3d.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/vertex.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/transform.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/material.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/mesh.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/meshutils.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/movementpath.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/bezierpath.cpp \
    /home/paulinamonroy/Documents/ETS/Qt-Dee/3d/objloader.cpp
############################

# library WTree
#   WTREE    ##############
HEADERS += \
    /home/paulinamonroy/Documents/ETS/DeeTree/wgraph/wnode.h \
    /home/paulinamonroy/Documents/ETS/DeeTree/wgraph/wtree.h

# Core program
#   CORE     ##############
HEADERS     += \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/globalconfig.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/inputmanager.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/uistate.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/debugger.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/timing.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/viewer.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/camera.h \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/core.h
SOURCES     += \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/globalconfig.cpp \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/inputmanager.cpp \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/uistate.cpp \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/debugger.cpp \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/viewer.cpp \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/camera.cpp \
    /home/paulinamonroy/Documents/ETS/DeeCore/core/timing.cpp
############################

#   PHYSICS        #########
HEADERS += \
    physics/interactiveobject.h \
    physics/simulationparameters.h \
    physics/simulation.h \
    physics/bodyinfo.h \
    physics/humanbody.h \
    physics/debugginginterface.h \
    physics/debuggingwidget.h \
    physics/joint.h\
    physics/shape.h \
    physics/utils.h \
    physics/animatedobject.h \
    physics/simulatedobject.h
SOURCES     += \
    physics/interactiveobject.cpp \
    physics/simulation.cpp \
    physics/bodyinfo.cpp \
    physics/humanbody.cpp \
    physics/debugginginterface.cpp \
    physics/joint.cpp\
    physics/shape.cpp \
    physics/utils.cpp \
    physics/animatedobject.cpp
############################


#   PROGRAM  ###############
HEADERS += \
    testscene.h \
    stats.h
SOURCES     += \
    testscene.cpp \
    stats.cpp
############################

# FORMS
FORMS += \
    physics/Debugging.ui


INSTALLS += target sources

unix:!macx:!symbian: LIBS += -L$usr/lib/x86_64-linux-gnu/

#Library dependencies
INCLUDEPATH += /usr/local/include/bullet
DEPENDPATH += /usr/local/include/bullet
INCLUDEPATH += /home/paulinamonroy/Documents/ETS/Qt-Dee
DEPENDPATH += /home/paulinamonroy/Documents/ETS/Qt-Dee
INCLUDEPATH += /home/paulinamonroy/Documents/ETS/DeeCore
DEPENDPATH += /home/paulinamonroy/Documents/ETS/DeeCore
INCLUDEPATH += /home/paulinamonroy/Documents/ETS/DeeTree
DEPENDPATH += /home/paulinamonroy/Documents/ETS/DeeTree

# Linux
# libs for bullet and opengl
LIBS *= -L/usr/lib -L/usr/local/lib -lGLU -lglut  -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody

OTHER_FILES += \
    ../assets/shaders/fshader.glsl \
    ../assets/shaders/vshader.glsl \
    ../assets/shaders/gshader.glsl \
    /User/batman/.config/Windel/ETS.conf

CONFIG(debug,debug|release){
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}


#ressource file
RESOURCES += \
    ../assets/ressources.qrc
