import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.1

Button {
    text: "empty"
    property int textSize: 17
    property string defaultColor: ""
    property string pressedColor: ""
    style: ButtonStyle {
        panel: Item {
            implicitHeight: control.height
            implicitWidth: control.width
            Rectangle {
                anchors.fill: parent
                //color: control.pressed ? "#222" : "transparent"
                color: "transparent"
                border.color: control.pressed ? pressedColor : defaultColor
                border.width: 3
                radius: 3
                Text {
                    id: buttonText
                    text: control.text
                    anchors.centerIn: parent
                    color: (control.pressed || !enabled) ? pressedColor : defaultColor
                    font.pointSize: textSize
                    renderType: Text.NativeRendering
                }
            }
        }
    }
}
