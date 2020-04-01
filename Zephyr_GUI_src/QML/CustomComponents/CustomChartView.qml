import QtQuick 2.7
import QtCharts 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Item {
    property string plotColor: ""
    property int index: 0
    property string plotTitle: ""
    property string unit: ""
    property int yMin: 0
    property int yMax: 10

    ChartView {
        id:chart

        anchors.fill: parent
        margins.left: 30
        margins.top: 0
        margins.bottom: 0

        antialiasing: true
        legend.visible: false
        animationOptions: ChartView.NoAnimation
        localizeNumbers: true
        backgroundColor: "transparent"

        Component.onCompleted: {
            // COnnect series to C++/appcore
            appCore.setSeries(index, chart.series(0), chart.series(1));
        }

        ValueAxis {
            id: axisY
            min: yMin
            max: yMax
            labelsVisible: false
            labelsColor: "white"
            labelFormat: 'd' // somehow makes the labels disappear including indent
            tickCount: 3
        }

        ValueAxis {
            id: axisX
            min: 0
            max: appCore.xRange/1000
            color: "transparent"
            labelsVisible: false
        }

        LineSeries {
            color: plotColor
            axisX: axisX
            axisY: axisY
            useOpenGL: true
        }
        LineSeries {
            color: plotColor
            axisX: axisX
            axisY: axisY
            useOpenGL: true
        }
    }
    Label{
        id: titleLabel
        anchors.top:  parent.top
        anchors.left: parent.left
        anchors.topMargin: 30
        anchors.leftMargin: 10
        font.bold: true
        font.pointSize: 10
        color: "white"
        text: plotTitle + ": "
    }
    Label{
        id: unitLabel
        anchors.top:  titleLabel.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.italic: true
        font.pointSize: 8
        text: unit
        color:"white"
    }
    Label {
        anchors.top: chart.top
        anchors.right: chart.left
        anchors.rightMargin: -40
        anchors.topMargin: 12
        font.pointSize: 8
        horizontalAlignment: Text.AlignRight
        text: yMax
        color: "white"
    }
    Label {
        anchors.bottom: chart.bottom
        anchors.right: chart.left
        anchors.rightMargin: -40
        anchors.bottomMargin: 25
        font.pointSize: 8
        horizontalAlignment: Text.AlignRight
        text: yMin
        color: "white"
    }
}
