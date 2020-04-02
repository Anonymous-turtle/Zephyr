import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtCharts 2.0

import Backend 1.0

Item {

    ColumnLayout {
        anchors.fill: parent
        CustomDisplayItem_Big{
            id: pPeakDisplay
            Layout.fillHeight: true
            Layout.fillWidth: true
            textColor: "#209fdf"//"blue"
            headerText: appCore.valueModel.get(AppCore.P_peak).name + "(" + appCore.valueModel.get(AppCore.P_peak).unit + ")"
            value: appCore.valueModel.get(AppCore.P_peak).value//appCore.pPeakVal
            min: appCore.valueModel.get(AppCore.P_aw).minAlarm
            max: appCore.valueModel.get(AppCore.P_aw).maxAlarm
            minEnabled: appCore.valueModel.get(AppCore.P_aw).minActive
            maxEnabled: appCore.valueModel.get(AppCore.P_aw).maxActive

            MouseArea{
                anchors.fill: parent
                onPressAndHold: {
                    alarmDialog.index = AppCore.P_aw
                    alarmDialog.open()
                }
            }
        }
        CustomDisplayItem_Big{
            id: peepDisplay
            Layout.fillHeight: true
            Layout.fillWidth: true
            textColor: "#209fdf"//"red"
            headerText: appCore.valueModel.get(AppCore.PEEP).name + " (" + appCore.valueModel.get(AppCore.PEEP).unit + ")"
            value: appCore.valueModel.get(AppCore.PEEP).value//appCore.pPeakVal
            min: appCore.valueModel.get(AppCore.PEEP).minAlarm
            max: appCore.valueModel.get(AppCore.PEEP).maxAlarm
            minEnabled: appCore.valueModel.get(AppCore.PEEP).minActive
            maxEnabled: appCore.valueModel.get(AppCore.PEEP).maxActive
            MouseArea{
                anchors.fill: parent
                onPressAndHold: {
                    alarmDialog.index = AppCore.PEEP
                    alarmDialog.open()
                }
            }
        }
//        CustomDisplayItem_Double{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            textColor: "#209fdf"//"blue"
//            headerText1: appCore.valueModel.get(AppCore.P_mean).name + "\n(" + appCore.valueModel.get(AppCore.P_mean).unit + ")"
//            value1: appCore.valueModel.get(AppCore.P_mean).value//appCore.pPeakVal
//            headerText2: appCore.valueModel.get(AppCore.PEEP).name + "\n(" + appCore.valueModel.get(AppCore.PEEP).unit + ")"
//            value2: appCore.valueModel.get(AppCore.PEEP).value//appCore.pPeakVal
//        }
        CustomDisplayItem_Big{
            id: rrDisplay
            Layout.fillHeight: true
            Layout.fillWidth: true
            textColor: "#bf593e"//"red"
            headerText: appCore.valueModel.get(AppCore.RR).name + " (" + appCore.valueModel.get(AppCore.RR).unit + ")"
            value: appCore.valueModel.get(AppCore.RR).value//appCore.pPeakVal
            min: appCore.valueModel.get(AppCore.RR).minAlarm
            max: appCore.valueModel.get(AppCore.RR).maxAlarm
            minEnabled: appCore.valueModel.get(AppCore.RR).minActive
            maxEnabled: appCore.valueModel.get(AppCore.RR).maxActive
            MouseArea{
                anchors.fill: parent
                onPressAndHold: {
                    alarmDialog.index = AppCore.RR
                    alarmDialog.open()
                }
            }
        }
        CustomDisplayItem_Big{
            id: o2Display
            Layout.fillHeight: true
            Layout.fillWidth: true
            textColor: "#bf593e"//"#bf593e"//"red"
            headerText: appCore.valueModel.get(AppCore.F_iO2).name + " (" + appCore.valueModel.get(AppCore.F_iO2).unit + ")"
            value: appCore.valueModel.get(AppCore.F_iO2).value//appCore.pPeakVal
            min: appCore.valueModel.get(AppCore.F_iO2).minAlarm
            max: appCore.valueModel.get(AppCore.F_iO2).maxAlarm
            minEnabled: appCore.valueModel.get(AppCore.F_iO2).minActive
            maxEnabled: appCore.valueModel.get(AppCore.F_iO2).maxActive
            MouseArea{
                anchors.fill: parent
                onPressAndHold: {
                    alarmDialog.index = AppCore.F_iO2
                    alarmDialog.open()
                }
            }
        }
        //        Rectangle {
        //            id: canvas
        //            Layout.minimumHeight: 100
        //            Layout.fillWidth: true
        //            Layout.columnSpan: 3
        //            clip: true
        //            color: "transparent"


        //            ChartView{ //ChartView
        //                id: graphView
        //                anchors.fill: parent
        //                //anchors.left: parent.left
        //                //anchors.right: parent.right
        //                //anchors.bottom: parent.bottom
        //                //height: width
        //                anchors.topMargin: -20
        //                anchors.bottomMargin: -30
        //                anchors.leftMargin:  -20
        //                anchors.rightMargin: -20

        //                legend.visible: false

        //                margins.top: 0
        //                margins.bottom: 0
        //                margins.left: 0
        //                margins.right: 0

        //                backgroundRoundness: 0
        //                backgroundColor: "transparent"//"#424242"
        //                Component.onCompleted: {
        //                    appCore.setSeries(index, chart.series(0));
        //                }

        //                ValueAxis {
        //                    id: axisY

        //                    min: appCore.valueModel.get(AppCore.V_t).lowerLimit
        //                    max: appCore.valueModel.get(AppCore.V_t).upperLimit
        //                    labelsVisible: false //true
        //                    labelsColor: "white"
        //                    labelFormat: 'd' // somehow makes the labels disappear including indent
        //                    tickCount: 3
        //                }

        //                ValueAxis {
        //                    id: axisX
        //                    min: appCore.valueModel.get(AppCore.P_aw).lowerLimit
        //                    max: appCore.valueModel.get(AppCore.P_aw).upperLimit
        //                    color: "transparent"
        //                    labelsVisible: false
        //                    tickCount: 3
        //                }
        //                LineSeries {
        //                    color: "#99ca53"
        //                    axisX: axisX
        //                    axisY: axisY
        //                    useOpenGL: true
        //                }
        //            }
        //        }

        CustomDisplayItem_Big_ratio{
            id: ieDisplay
            Layout.fillHeight: true
            Layout.fillWidth: true
            textColor: "#99ca53"//"green"

            headerText: appCore.valueModel.get(AppCore.IE_ratio).name
            value: appCore.valueModel.get(AppCore.IE_ratio).value
            min: appCore.valueModel.get(AppCore.IE_ratio).minAlarm
            max: appCore.valueModel.get(AppCore.IE_ratio).maxAlarm
            minEnabled: appCore.valueModel.get(AppCore.IE_ratio).minActive
            maxEnabled: appCore.valueModel.get(AppCore.IE_ratio).maxActive
            precision: 1
            MouseArea{
                anchors.fill: parent
                onPressAndHold: {
                    alarmDialog.index = AppCore.IE_ratio
                    alarmDialog.open()
                }
            }
        }
//        CustomDisplayItem_Double{
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            textColor: "#99ca53"//"blue"
//            headerText1: "VTi\n(ml)"
//            value1: 10//appCore.vTiVal
//            headerText2: "VTe\n(ml)"
//            value2: 3
//        }
    }
}
