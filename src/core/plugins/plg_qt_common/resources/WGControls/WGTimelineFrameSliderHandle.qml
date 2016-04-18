import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0

/*!
 \An invisible Slider handle intended for the WGTimelineBarSlider Control.

 Shouldn't really be needed to be used on it's own but may be useful in some other kind of slider.
*/

WGSliderHandle {
    id: sliderHandle
    objectName: "SliderHandle"

    //property QtObject previousHandle: null

    //property QtObject nextHandle: null

    property string frameType

    property bool selected: false

    property bool handleDragging: false

    property string label: ""

    property bool keyframeActive: false

    property Component handleStyle: Item {

        implicitWidth: defaultSpacing.minimumRowHeight
        implicitHeight: defaultSpacing.minimumRowHeight

        Rectangle {
            anchors.centerIn: parent
            width: 2
            height: parent.height
            color: parentSlider.barColor
        }

        Rectangle {
            id: keyframeWidget
            anchors.centerIn: parent

            height: parent.height - defaultSpacing.doubleMargin
            width: parent.width - defaultSpacing.doubleMargin

            border.width: 1
            border.color: selected ? palette.highlightColor : "transparent"

            color: "#CCCCCC"

            transformOrigin: Item.Center
            rotation: frameType == "linear" ? 45 : 0

            radius: frameType == "bezier" ? height : 0
            clip: true

            Rectangle {
                id: activeHighlight
                anchors.fill: parent
                anchors.margins: 2
                radius: parent.radius
                color: "red"

                visible: true
                opacity: 0

                states: [
                    State {
                        name: "INACTIVE"
                        when: !sliderHandle.keyframeActive
                        PropertyChanges { target: activeHighlight; opacity: 0.0}
                    },
                    State {
                        name: "ACTIVE"
                        when: sliderHandle.keyframeActive
                        PropertyChanges { target: activeHighlight; opacity: 1.0}
                    }

                ]
                transitions: [
                    Transition {
                        from: "ACTIVE"
                        to: "INACTIVE"
                        NumberAnimation { target: activeHighlight; properties: "opacity"; duration: 800}
                    }
                ]
            }

        }

        WGLabel {
            anchors.left: keyframeWidget.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: defaultSpacing.standardMargin

            visible: parentSlider.showLabel
            text: label
        }
    }

    Connections {
        target: parentSlider

        onDragSelectedHandles: {
            if (sliderHandle.selected && !handleDragging)
            {
                sliderHandle.value = parentSlider.initialValues[__handlePosList.indexOf(sliderHandle)] + delta
            }
        }
    }
}

