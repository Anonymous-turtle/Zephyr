import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import Backend 1.0

Item {
    property alias pvButton:pvButton
    RowLayout{
        anchors.fill: parent

        CustomTargetValButton {
            id: pvButton
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        CustomTargetValButton {
            id: rrButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            index: AppCore.RR
        }
        CustomTargetValButton {
            id: peepButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            index: AppCore.PEEP
        }
        CustomTargetValButton {
            id: o2Button
            Layout.fillHeight: true
            Layout.fillWidth: true
            index: AppCore.F_iO2
        }
        // TODO: make use of custom component
        DelayButton {
            id: ieButton

            property var index:AppCore.IE_ratio

            Layout.fillHeight: true
            Layout.fillWidth: true
            ButtonGroup.group: parameterButtons
            checkable: true          
            contentItem: ColumnLayout {

                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                    font.bold: true
                    font.pointSize: 14
                    text: appCore.valueModel.get(AppCore.IE_ratio).name
                    color: parent.checked? "white": "black"
                }
                Label{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 12
                    text: "1:" + appCore.valueModel.get(AppCore.IE_ratio).target
                    color: parent.checked? "white": "black"
                }
            }
            Rectangle{
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 3
                Label {
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 2
                    anchors.leftMargin: 2
                    font.pointSize: 8
                    text: appCore.valueModel.get(AppCore.IE_ratio).lowerLimit
                }
                Label {
                    anchors.right: parent.right
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 2
                    anchors.rightMargin: 2
                    font.pointSize: 8
                    text: appCore.valueModel.get(AppCore.IE_ratio).upperLimit
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.top: parent.top
                    color: "black"
                    width: parent.width * ((1 / (appCore.valueModel.get(AppCore.IE_ratio).upperLimit - appCore.valueModel.get(AppCore.IE_ratio).lowerLimit)) * (appCore.valueModel.get(AppCore.IE_ratio).target - appCore.valueModel.get(AppCore.IE_ratio).lowerLimit))
                }
            }
        }
    }
}
