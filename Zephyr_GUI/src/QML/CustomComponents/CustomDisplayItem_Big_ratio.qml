import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Item {
    // TODO: transform to index based
    property string textColor:""
    property string headerText:""
    property var value: 0
    property int min:0
    property int max:0
    property var precision:0
    property bool minEnabled:true
    property bool maxEnabled:true

    Label {
        id: headerLabel
        width: parent.width
        height: parent.height/8
        font.pointSize: 100
        minimumPointSize: 8
        fontSizeMode: Text.Fit
        color: textColor
        text: headerText
    }
    Label {
        id: valueLabel
        anchors.top: headerLabel.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        font.pointSize: 100
        minimumPointSize: 6
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        color: textColor
        text: "1:" + value.toLocaleString(Qt.locale("us_US"), 'f', precision)
    }

    ColumnLayout{
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 5
        width: parent.width/9
        Label {
            id: maxLabel
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pointSize: 10
            color: maxEnabled? textColor:"transparent"
            text: max
        }
        Label {
            id: minLabel
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pointSize: 10
            color: minEnabled? textColor:"transparent"
            text: min
        }
    }
}
