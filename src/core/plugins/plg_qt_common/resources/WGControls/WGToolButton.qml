import QtQuick 2.3
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0

//Button with no frame in it's default state or text.

WGPushButton {

    noFrame_: true

    implicitHeight: {
        if (panelProps.rowHeight_){
            panelProps.rowHeight_
        } else {
            22
        }
    }

    implicitWidth: implicitHeight

    text: ""
}