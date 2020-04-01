import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Item {
    // TODO: transform to index based
    property string textColor:""
    property string headerText1:""
    property var value1: 0
    property string headerText2:""
    property var value2: 0
    property var precision:0

    ColumnLayout{
        anchors.fill: parent
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            Label {
                id: topItemHeaderLabel
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 100
                minimumPointSize: 8
                fontSizeMode: Text.Fit

                color: textColor
                text: headerText1
            }
            Label {
                id: topItemValueLabel
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 100
                minimumPointSize: 8
                fontSizeMode: Text.Fit
                color: textColor
                text: value1.toLocaleString(Qt.locale("us_US"), 'f', precision)
            }
        }
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            Label {
                id: bottomItemHeaderLabel
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 100
                minimumPointSize: 8
                fontSizeMode: Text.Fit
                color: textColor
                text: headerText2
            }
            Label {
                id: bottomItemValueLabel
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 100
                minimumPointSize: 8
                fontSizeMode: Text.Fit
                color: textColor
                text: value2.toLocaleString(Qt.locale("us_US"), 'f', precision)
            }
        }
    }
}
