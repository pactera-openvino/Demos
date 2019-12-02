#-------------------------------------------------
#
# Project created by QtCreator 2019-10-28T16:02:20
#
#-------------------------------------------------

QT       += core gui charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OneVino
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        addmeeting.cpp \
        align_transform.cpp \
        cnn.cpp \
        database.cpp \
        detectors.cpp \
        face.cpp \
        industry.cpp \
        industryresult.cpp \
        licplate.cpp \
        main.cpp \
        mainwindow.cpp \
        meetingscene.cpp \
        playback.cpp \
        reg_face.cpp \
        reid_gallery.cpp \
        tracker.cpp \
        visitor.cpp \
        visitorscene.cpp \
        visualizer.cpp

HEADERS += \
        addmeeting.h \
        cnn.hpp \
        common.hpp \
        database.hpp \
        detectors.hpp \
        face.hpp \
        face_reid.hpp \
        grid_mat.hpp \
        industry.h \
        industryresult.h \
        input_wrappers.hpp \
        interactive_face_detection.hpp \
        licplate.h \
        mainwindow.h \
        meetingscene.h \
        net_wrappers.hpp \
        playback.h \
        reg_face.h \
        tracker.hpp \
        visitor.h \
        visitorscene.h \
        visualizer.hpp

INCLUDEPATH += /opt/intel/openvino/opencv/include/ \
            += /opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/include/ \
            += /opt/intel/openvino/deployment_tools/open_model_zoo/demos/common/ \
            += /root/omz_demos_build/thirdparty/gflags/include/ \
            += /opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/src/extension/ \
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libclDNN64.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libclDNNPlugin.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libcpu_extension_avx2.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libcpu_extension_avx512.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libcpu_extension_sse4.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libGNAPlugin.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libHDDLPlugin.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libHeteroPlugin.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libinference_engine.so
#LIBS +=/root/OpenVino-Driver-Behaviour/build/ie_cpu_extension/libcpu_extension.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libMKLDNNPlugin.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libMultiDevicePlugin.so
LIBS +=/opt/intel/openvino_2019.2.275/deployment_tools/inference_engine/lib/intel64/libmyriadPlugin.so
LIBS +=/root/omz_demos_build/intel64/Release/lib/libgflags_nothreads.a
LIBS +=/opt/intel/openvino/opencv/lib/libopencv*
LIBS += -ldl

FORMS += \
        addmeeting.ui \
        industry.ui \
        industryresult.ui \
        licplate.ui \
        mainwindow.ui \
        meetingscene.ui \
        playback.ui \
        reg_face.ui \
        visitor.ui \
        visitorscene.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
