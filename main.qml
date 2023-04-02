/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 2.15
import QtWebView 1.1
import QtQuick.Layouts 1.1

Item {
    id: root
    property bool showProgress: webView.loading
                                && Qt.platform.os !== "ios"
                                && Qt.platform.os !== "winrt"
    visible: true

    ToolBar {
        id: navigationBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
            anchors.fill: parent
            spacing: 0

            ToolButton {
                id: backButton
                icon.source: "images/left-32.png"
                onClicked: webView.goBack()
                enabled: webView.canGoBack
                Layout.preferredWidth: navigationBar.height
            }

            ToolButton {
                id: forwardButton
                icon.source: "images/right-32.png"
                onClicked: webView.goForward()
                enabled: webView.canGoForward
                Layout.preferredWidth: navigationBar.height
            }

            ToolButton {
                id: reloadButton
                icon.source: webView.loading ? "images/stop-32.png" : "images/refresh-32.png"
                onClicked: webView.loading ? webView.stop() : webView.reload()
                Layout.preferredWidth: navigationBar.height
            }

            ToolButton {
                id: refresheeButton

                text: checked ? "Updating in " + remainTime : "Start Update"
                checkable: true

                property int remainTime: 0
                property int updateInt: theWindow.updateInterval

                NumberAnimation {
                    id: remainAnimation
                    target: refresheeButton
                    property: "remainTime"
                    duration: refresheeButton.updateInt * 1000
                    from: refresheeButton.updateInt
                    to: 0
                }

                onClicked: remainAnimation.start()
            }

            Item { Layout.preferredWidth: 5 }

            TextField {
                Layout.fillWidth: true
                id: urlField
                inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhPreferLowercase
                text: webView.url

                onAccepted: webView.url = theWindow.fromUserInput(text)

                ProgressBar {
                    anchors.centerIn: parent
                    z: Qt.platform.os === "android" ? -1 : 1
                    visible: showProgress
                    from: 0
                    to: 100
                    value: webView.loadProgress == 100 ? 0 : webView.loadProgress
                }
            }

            Item { Layout.preferredWidth: 5 }

            ToolButton {
                id: goButton
                text: qsTr("Go")
                Layout.preferredWidth: navigationBar.height
                onClicked: {
                    Qt.inputMethod.commit()
                    Qt.inputMethod.hide()
                    webView.url = theWindow.fromUserInput(urlField.text)
                }
            }

            Item { Layout.preferredWidth: 10 }
        }
    }

    Item {
        id: statusBar
        visible: showProgress
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
            anchors.fill: parent
            Label { text: webView.loadProgress == 100 ? qsTr("Done") : qsTr("Loading: ") + webView.loadProgress + "%" }
        }
    }

    WebView {
        id: webView
        anchors.fill: parent
        anchors.topMargin: navigationBar.height
        anchors.bottomMargin: statusBar.height
        url: "https://metro.zakaz.md/ru/"
        onLoadingChanged: function(loadRequest) {
            if (loadRequest.errorString)
                console.error(loadRequest.errorString);
        }
    }

    ScanRectVisualizer {
        height: theWindow.scanRect.height
        width: theWindow.scanRect.width
        x: theWindow.scanRect.x
        y: theWindow.scanRect.y

        Image {
            id: diffImage
            anchors.fill: parent
            source: theWindow.diffFilename

            SequentialAnimation {
                loops: Animation.Infinite
                running: diffImage.source != ""

                PauseAnimation {
                    duration: 2000
                }

                ScriptAction {
                    script: diffImage.visible = !diffImage.visible
                }
            }
        }
    }

    Component.onCompleted: {
        theWindow.scanRect = Qt.rect(30,150, 720, 530)
    }

    function updatePage() {
        refresheeButton.checked = true
        remainAnimation.restart()
        webView.reload()
    }
}
