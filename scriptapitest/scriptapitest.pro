include( ../global.pri )

TARGET   = scriptapitest
TEMPLATE = app
CONFIG   += qt warn_on

QT += core printsupport script serialport sql webchannel websockets \
      widgets xml xmlpatterns

lessThan (QT_MINOR_VERSION, 6) : isEqual(QT_MAJOR_VERSION, 5) {
  QT += webkit webkitwidgets
} else {
  QT += webengine webenginewidgets
}

INCLUDEPATH += ../scriptapi \
               ../common \
               ../../xtuple-build-desktop/scriptapi \
               ../../xtuple-build-desktop/common

DEPENDPATH  += $${INCLUDEPATH} ../lib

win32-msvc* {
  PRE_TARGETDEPS += ../lib/xtuplescriptapi.lib          \
                    ../lib/xtuplecommon.$${XTLIBEXT}    \
                    $${OPENRPT_LIBDIR}/qzint.$${OPENRPTLIBEXT}    \
                    $${OPENRPT_LIBDIR}/MetaSQL.$${OPENRPTLIBEXT}  \
                    $${OPENRPT_LIBDIR}/renderer.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/wrtembed.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/openrptcommon.$${OPENRPTLIBEXT}
} else {
  PRE_TARGETDEPS += ../lib/libxtuplecommon.$${XTLIBEXT} \
                    ../lib/libxtuplescriptapi.a         \
                    $${OPENRPT_LIBDIR}/libqzint.$${OPENRPTLIBEXT}    \
                    $${OPENRPT_LIBDIR}/libMetaSQL.$${OPENRPTLIBEXT}  \
                    $${OPENRPT_LIBDIR}/librenderer.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/libwrtembed.$${OPENRPTLIBEXT} \
                    $${OPENRPT_LIBDIR}/libopenrptcommon.$${OPENRPTLIBEXT}
}

QMAKE_LIBDIR = ../lib $${OPENRPT_LIBDIR} $$QMAKE_LIBDIR
LIBS        += -lqzint $${DMTXLIB} -lxtuplescriptapi -lxtuplecommon -lopenrptcommon
LIBS        += -lrenderer -lMetaSQL

OBJECTS_DIR = tmp
win32 {
  win32-msvc*:LIBS += -lshell32
}

DESTDIR     = .
MOC_DIR     = moc
UI_DIR      = ui

FORMS = \
    scriptapitestresults.ui

HEADERS = \
    scriptapitestresults.h

SOURCES = scriptapitest.cpp \
    scriptapitestresults.cpp

DISTFILES += \
    jstests/_setup.js \
    jstests/char.js \
    jstests/contactClusterSetup.js \
    jstests/engineEvaluate.js \
    jstests/include.js \
    jstests/jsconsole.js \
    jstests/metaSqlHighLighter.js \
    jstests/orReportProto.js \
    jstests/parameterEditProto.js \
    jstests/parameterListSetup.js \
    jstests/qaAbstractSocket.js \
    jstests/qActionProto.js \
    jstests/qApplicationProto.js \
    jstests/qBoxLayout.js \
    jstests/qBuffer.js \
    jstests/qButtonGroup.js \
    jstests/qByteArray.js \
    jstests/qCoreApplication.js \
    jstests/qCryptoGraphicHash.js \
    jstests/qDataWidgetMapper.js \
    jstests/qdate.js \
    jstests/qDialogButtonBox.js \
    jstests/qDialogSetup.js \
    jstests/qDir.js \
    jstests/qDnsDomainNameRecord.js \
    jstests/qDnsHostAddressRecord.js \
    jstests/qDnsLookup.js \
    jstests/qDnsMailExchangeRecord.js \
    jstests/qDnsServiceRecord.js \
    jstests/qDnsTextRecord.js \
    jstests/qDockWidget.js \
    jstests/qDomAttr.js \
    jstests/qDomCDataSection.js \
    jstests/qDomCharacterData.js \
    jstests/qEventLoop.js \
    jstests/qWebEnginePage.js \
    jstests/qWebEngineView.js \
    jstests/_setup.js \
    jstests/char.js \
    jstests/contactClusterSetup.js \
    jstests/engineEvaluate.js \
    jstests/include.js \
    jstests/jsconsole.js \
    jstests/metaSqlHighLighter.js \
    jstests/orReportProto.js \
    jstests/parameterEditProto.js \
    jstests/parameterListSetup.js \
    jstests/qaAbstractSocket.js \
    jstests/qActionProto.js \
    jstests/qApplicationProto.js \
    jstests/qBoxLayout.js \
    jstests/qBuffer.js \
    jstests/qButtonGroup.js \
    jstests/qByteArray.js \
    jstests/qCoreApplication.js \
    jstests/qCryptoGraphicHash.js \
    jstests/qDataWidgetMapper.js \
    jstests/qdate.js \
    jstests/qDialogButtonBox.js \
    jstests/qDialogSetup.js \
    jstests/qDir.js \
    jstests/qDnsDomainNameRecord.js \
    jstests/qDnsHostAddressRecord.js \
    jstests/qDnsLookup.js \
    jstests/qDnsMailExchangeRecord.js \
    jstests/qDnsServiceRecord.js \
    jstests/qDnsTextRecord.js \
    jstests/qDockWidget.js \
    jstests/qDomAttr.js \
    jstests/qDomCDataSection.js \
    jstests/qDomCharacterData.js \
    jstests/qEventLoop.js \
    jstests/qWebEnginePage.js \
    jstests/qWebEngineView.js

SUBDIRS += \
    scriptapitest.pro
