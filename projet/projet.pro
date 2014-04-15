QT          += opengl xml widgets

SOURCES     = \
    main.cpp

# library QtDee
#   UTILS   ###############
HEADERS     += \
    /home/batman/Programmation/QtDee/utils/utils.h \
    /home/batman/Programmation/QtDee/utils/triplet.h \
    /home/batman/Programmation/QtDee/utils/extendedtriplet.h \
    /home/batman/Programmation/QtDee/utils/geometry.h \
    /home/batman/Programmation/QtDee/utils/maths.h \
    /home/batman/Programmation/QtDee/utils/csvparser.h \
    /home/batman/Programmation/QtDee/utils/typedefinitions.h \
    /home/batman/Programmation/QtDee/utils/curve.h
SOURCES     += \
    /home/batman/Programmation/QtDee/utils/maths.cpp \
    /home/batman/Programmation/QtDee/utils/csvparser.cpp \
    /home/batman/Programmation/QtDee/utils/curve.cpp
############################


# library QtDee
#   3D         #############
HEADERS += \
    /home/batman/Programmation/QtDee/3d/3d.h \
    /home/batman/Programmation/QtDee/3d/graphicobject3d.h \
    /home/batman/Programmation/QtDee/3d/vertex.h \
    /home/batman/Programmation/QtDee/3d/transform.h \
    /home/batman/Programmation/QtDee/3d/material.h \
    /home/batman/Programmation/QtDee/3d/mesh.h \
    /home/batman/Programmation/QtDee/3d/meshutils.h \
    /home/batman/Programmation/QtDee/3d/movementpath.h \
    /home/batman/Programmation/QtDee/3d/bezierpath.h \
    /home/batman/Programmation/QtDee/3d/objloader.h

SOURCES     += \
    /home/batman/Programmation/QtDee/3d/graphicobject3d.cpp \
    /home/batman/Programmation/QtDee/3d/vertex.cpp \
    /home/batman/Programmation/QtDee/3d/transform.cpp \
    /home/batman/Programmation/QtDee/3d/material.cpp \
    /home/batman/Programmation/QtDee/3d/mesh.cpp \
    /home/batman/Programmation/QtDee/3d/meshutils.cpp \
    /home/batman/Programmation/QtDee/3d/movementpath.cpp \
    /home/batman/Programmation/QtDee/3d/bezierpath.cpp \
    /home/batman/Programmation/QtDee/3d/objloader.cpp
############################

# library WTree
#   WTREE    ##############
HEADERS += \
    /home/batman/Programmation/DeeTree/wgraph/wnode.h \
    /home/batman/Programmation/DeeTree/wgraph/wtree.h

# Core program
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
INCLUDEPATH += /home/batman/Programmation/QtDee
DEPENDPATH += /home/batman/Programmation/QtDee
INCLUDEPATH += /home/batman/Programmation/DeeCore
DEPENDPATH += /home/batman/Programmation/DeeCore
INCLUDEPATH += /home/batman/Programmation/DeeTree
DEPENDPATH += /home/batman/Programmation/DeeTree

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
