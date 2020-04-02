import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

DelayButton {
    property string name: ""
    property string value: ""
    property var index:0
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
            text: appCore.valueModel.get(index).name + ":"
            color: parent.checked? "white": "black"
        }
        Label{
            Layout.fillHeight: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 12
            text: appCore.valueModel.get(index).target  + " " + appCore.valueModel.get(index).unit
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
            text: appCore.valueModel.get(index).lowerLimit
        }
        Label {
            anchors.right: parent.right
            anchors.bottom: parent.top
            anchors.bottomMargin: 2
            anchors.rightMargin: 2
            font.pointSize: 8
            text: appCore.valueModel.get(index).upperLimit
        }
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            color: "black"
            width: parent.width * ((1 / (appCore.valueModel.get(index).upperLimit - appCore.valueModel.get(index).lowerLimit)) * (appCore.valueModel.get(index).target - appCore.valueModel.get(index).lowerLimit))
        }
    }
}
