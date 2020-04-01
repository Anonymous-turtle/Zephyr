import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Item {
    property alias pressureModeButton:pressureModeButton
    property alias volumeModeButton:volumeModeButton

    RowLayout{
        anchors.fill: parent
        DelayButton {
            id: pressureModeButton
            Layout.fillHeight: true
            Layout.preferredWidth: 150
            checked: true
            enabled: !checked
            ButtonGroup.group: modeButtons

            contentItem: Label {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Pressure Mode"
                color: parent.checked? "white": "black"
            }
        }
        DelayButton {
            id: volumeModeButton
            Layout.fillHeight: true
            Layout.preferredWidth: 150
            enabled: !checked
            ButtonGroup.group: modeButtons
            contentItem: Label {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Volume Mode"
                color: parent.checked? "white": "black"
            }
        }

        DelayButton {
            id: alarmButton
            Layout.fillHeight: true
            Layout.fillWidth: true
            onActivated: {
                progress = 0
                appCore.resetAlarms()
            }
            contentItem: Label {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: appCore.alarm===""? "No Alarms": appCore.alarm
                color: "white"
            }
            background: Rectangle {
                color: appCore.alarm===""? "green": "red"
            }
        }
    }
}
