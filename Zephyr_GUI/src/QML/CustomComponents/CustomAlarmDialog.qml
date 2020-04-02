import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import Backend 1.0

Dialog{
    id: dialog

    property bool isOpen: false
    property int index:0

    x: (window.width - width) / 2
    y: (window.height - height) / 2

    width: 600
    height: 350

    modal: Qt.WindowModal
    closePolicy: Popup.NoAutoClose

    header: Label{
        id: title
        padding: 12
        color: "white"
        font.bold: true
        font.pixelSize: 30
        text: isOpen? appCore.valueModel.getLast().name + " Setting": ""
    }
    background: Rectangle{
        color: "black" //"#303030"
        radius: 10
    }

    onOpened: {
        appCore.setTempValueObj(index)
        valueSlider.from = appCore.valueModel.getLast().lowerLimit
        valueSlider.to = appCore.valueModel.getLast().upperLimit
        isOpen = true
    }
    onClosed: {
        isOpen = false
    }

    ButtonGroup{
        id: settableButtons
        onCheckedButtonChanged: {
            if (checkedButton == minButton)
            {
                valueSlider.value = appCore.valueModel.getLast().minAlarm
                valueDiaplay.text = appCore.valueModel.getLast().minAlarm
            }
            else if (checkedButton == maxButton)
            {
                valueSlider.value = appCore.valueModel.getLast().maxAlarm
                valueDiaplay.text = appCore.valueModel.getLast().maxAlarm
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Button {
                    id: minButton
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    checkable: true
                    ButtonGroup.group: settableButtons
                    text: isOpen?  "Alarm above: " + appCore.valueModel.getLast().minAlarm+ " " + appCore.valueModel.getLast().unit: ""
                }
                Button {
                    id: maxButton
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    checkable: true
                    ButtonGroup.group: settableButtons
                    text: isOpen?  "Alarm below: " + appCore.valueModel.getLast().maxAlarm+ " "+ appCore.valueModel.getLast().unit: ""
                }
            }
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                enabled: settableButtons.checkState
                Label {
                    id: valueDiaplay
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 60
                    color: "white"
                    text: {
                        // TODO: find better way. works for now
                        if (settableButtons.checkedButton == minButton)
                            appCore.valueModel.getLast().minAlarm
                        else if (settableButtons.checkedButton == maxButton)
                            appCore.valueModel.getLast().maxAlarm
                        else
                            "0"
                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Button {
                        id: minusButton
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        contentItem: Label {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 40
                            text: "-"
                        }
                        onClicked: {
                            // TODO: find better way. works for now
                            if (settableButtons.checkedButton == minButton)
                            {
                                if (appCore.valueModel.getLast().minAlarm > appCore.valueModel.getLast().lowerLimit)
                                    appCore.updateTempMin(appCore.valueModel.getLast().minAlarm -1)
                                valueSlider.value = appCore.valueModel.getLast().minAlarm
                                valueDiaplay.text = appCore.valueModel.getLast().minAlarm
                            }
                            else if (settableButtons.checkedButton == maxButton)
                            {
                                if (appCore.valueModel.getLast().maxAlarm > appCore.valueModel.getLast().lowerLimit)
                                    appCore.updateTempMax(appCore.valueModel.getLast().maxAlarm -1)
                                valueSlider.value = appCore.valueModel.getLast().maxAlarm
                                valueDiaplay.text = appCore.valueModel.getLast().maxAlarm
                            }
                        }
                    }
                    Button {
                        id: plusButton
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        contentItem: Label {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 40
                            text: "+"
                        }
                        onClicked: {
                            // TODO: find better way. works for now
                            if (settableButtons.checkedButton == minButton)
                            {
                                if (appCore.valueModel.getLast().minAlarm < appCore.valueModel.getLast().upperLimit)
                                    appCore.updateTempMin(appCore.valueModel.getLast().minAlarm +1)
                                valueSlider.value = appCore.valueModel.getLast().minAlarm
                                valueDiaplay.text = appCore.valueModel.getLast().minAlarm
                            }
                            else if (settableButtons.checkedButton == maxButton)
                            {
                                if (appCore.valueModel.getLast().maxAlarm < appCore.valueModel.getLast().upperLimit)
                                    appCore.updateTempMax(appCore.valueModel.getLast().maxAlarm +1)
                                valueSlider.value = appCore.valueModel.getLast().maxAlarm
                                valueDiaplay.text = appCore.valueModel.getLast().maxAlarm
                            }
                        }
                    }
                }
                Slider {
                    id: valueSlider
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (settableButtons.checkedButton == minButton)
                        {
                            appCore.updateTempMin(value)
                            valueDiaplay.text = appCore.valueModel.getLast().minAlarm
                        }
                        else if (settableButtons.checkedButton == maxButton)
                        {
                            appCore.updateTempMax(value)
                            valueDiaplay.text = appCore.valueModel.getLast().maxAlarm
                        }
                    }
                }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 55

            Button {
                id: cancelButton
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 150
                text: "Cancel"
                onClicked: {
                    dialog.close()
                    appCore.removeTempValueObj()
                    settableButtons.checkState = Qt.Unchecked
                }
            }
            Button {
                id: saveButton
                anchors.right: cancelButton.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 150
                anchors.rightMargin: 5
                text: "Save"
                onClicked: {
                    dialog.close()
                    appCore.saveTempValues(index)
                    settableButtons.checkState = Qt.Unchecked
                }
            }
        }
    }
}

