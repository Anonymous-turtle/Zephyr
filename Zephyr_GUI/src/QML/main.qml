import QtQuick 2.7
import QtQuick.Window 2.2
import QtCharts 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import Backend 1.0
import "qrc:/QML/CustomComponents"

Window {
    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("Zephyr")
    color: "black"

    AppCore{
        id: appCore
        onModeChanged: {
            // TEST
            // Open Value setting dialog on mode change
            if (appCore.mode === AppCore.PressureMode)
            {
                footer.pvButton.index = checkedButton === header.pressureModeButton
            }
            else if (appCore.mode === AppCore.PressureMode)
            {
                footer.pvButton.index = checkedButton === header.volumeModeButton
            }
        }
    }

    // Button Groups:
    ButtonGroup{
        id: modeButtons
        exclusive: true
        onCheckedButtonChanged: {
            footer.pvButton.index = checkedButton === header.pressureModeButton ? AppCore.P_aw :AppCore.V_t
            footer.pvButton.checked = true

            settingDialog.open()

            // Update mode value
            if (checkedButton === header.pressureModeButton)
            {
                appCore.mode = AppCore.PressureMode
            }
            else if (checkedButton === header.volumeModeButton)
            {
                appCore.mode = AppCore.VolumeMode
            }
            else
            {
                appCore.mode = -1
            }
        }
    }

    ButtonGroup{
        id: parameterButtons
        onCheckedButtonChanged: {
            if (checkedButton && checkedButton.checked) settingDialog.open()
        }
    }

    // Header: | Pressure Mode | Volume Mode | Alarm |
    CustomHeader {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: window.height/15
    }

    // Sidebar: | Paw | PEEP | RR | FiO2 | I:E |
    CustomSidebar {
        id: sidebar
        anchors.top: header.bottom
        anchors.bottom: footer.top
        anchors.right: parent.right
        width: window.width/7
        anchors.topMargin: 10
        anchors.bottomMargin: 10
    }

    // Charts: | Paw | Flow | Volume |
    ColumnLayout{
        id: chartViews
        anchors.bottom: footer.top
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: sidebar.left
        anchors.rightMargin: -35
        anchors.bottomMargin: -20
        spacing: -30

        CustomChartView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            plotColor:"#209fdf"//"blue"
            index: 1
            plotTitle: appCore.valueModel.get(AppCore.P_aw).name
            unit: appCore.valueModel.get(AppCore.P_aw).unit
            yMin: 0 //appCore.valueModel.get(AppCore.P_aw).lowerLimit
            yMax: 50 //appCore.valueModel.get(AppCore.P_aw).upperLimit
        }
        CustomChartView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            plotColor:"#bf593e"//"red"
            index: 2
            plotTitle: appCore.valueModel.get(AppCore.Flow).name
            unit: appCore.valueModel.get(AppCore.Flow).unit
            yMin: -100 //appCore.valueModel.get(AppCore.Flow).lowerLimit
            yMax: 100  //appCore.valueModel.get(AppCore.Flow).upperLimit
        }
        CustomChartView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            plotColor:"#99ca53"//"green"
            index: 3
            plotTitle: appCore.valueModel.get(AppCore.V_t).name
            unit: appCore.valueModel.get(AppCore.V_t).unit
            yMin: 0 //appCore.valueModel.get(AppCore.V_t).lowerLimit
            yMax: 900 //appCore.valueModel.get(AppCore.V_t).upperLimit
        }
    }

    // Footer: | Paw or Vt | RR | PEEP | FiO2 | I:E |
    CustomFooter {
        id: footer
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: window.height/9
    }

    // Dialog to set target, min and max
    CustomSetupDialog {
        id: settingDialog
    }

    // Dialog to set min and max
    CustomAlarmDialog {
        id: alarmDialog
    }
    Dialog{
        id: comPortSelectorDialog

        x: (window.width - width) / 2
        y: (window.height - height) / 2

        //width: 600
        //height: 350
        //width: 400

        modal: Qt.WindowModal
        closePolicy: Popup.NoAutoClose

        background: Rectangle{
            color: "black" //"#303030"
            radius: 10
        }
        header: Label{
            id: title
            padding: 12
            color: "white"
            font.bold: true
            font.pixelSize: 14
            text: qsTr("Select a Serial Port:")
        }

        Column{
            anchors.centerIn: parent
            spacing: 15
            ComboBox{
                id: comPortComboBox
                width: 300
                model: appCore.getSerialPorts()
            }
            Row{
                anchors.right: parent.right
                spacing: 10
                Button {
                    text: qsTr("Refresh")
                    onClicked: {
                        comPortComboBox.model = appCore.getSerialPorts()
                    }
                }
                Button {
                    text: qsTr("Connect")
                    onClicked: {
                        appCore.connectToPort(comPortComboBox.currentText)
                        comPortSelectorDialog.close()
                    }
                }
                Button {
                    text: qsTr("Cancel")
                    onClicked:
                    {
                        comPortSelectorDialog.close()
                    }
                }
            }
        }
    }
}
