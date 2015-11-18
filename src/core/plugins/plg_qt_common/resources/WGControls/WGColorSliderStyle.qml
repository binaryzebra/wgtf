import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Layouts 1.1
import BWControls 1.0

/*!
    A slider style that contains a color gradient as the background
*/
WGSliderStyle {
    id: sliderStyle
    objectName: "WGColorSliderStyle"

    //a large rectangle handle that fills the entire gradient groove
    property Component defaultHandle: WGButtonFrame {
        id: defaultHandleFrame
        implicitHeight: __horizontal ? control.height - 2 : 8
        implicitWidth: __horizontal ? 8 : control.width - 2
        color: control.__hoveredHandle == buttonid ? "white" : palette.OverlayLighterShade
        borderColor: palette.OverlayDarkerShade
        innerBorderColor: control.__activeHandle == buttonid && control.activeFocus ? palette.HighlightShade : "transparent"

        radius: defaultSpacing.halfRadius
    }

    //a small arrow handle that is offset below the gradient groove. It also contains a color swatch.
    //lots of magic numbers here as needed to use an icon to get the triangle shape.
    property Component arrowHandle: Item {
        implicitHeight: __horizontal ? control.height - 2 : 11
        implicitWidth: __horizontal ? 11 : control.width - 2
            Image {
                id: arrowHandleFrame
                source: "icons/arrow_handle.png"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom

                Rectangle {
                    id: colorSquare
                    height: parent.width - 4
                    width: parent.width - 4
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: 2

                    color: control.colorData[buttonid]

                    radius: buttonid == control.__activeHandle ? 5 : 0

                    border.width: 1
                    border.color: Qt.darker(colorSquare.color, 1.2)

                    Connections {
                        target: control
                        onUpdateColorBars : {
                            colorSquare.color = control.colorData[buttonid]
                        }
                    }
                }
        }
    }

    handle: control.offsetArrowHandles ? arrowHandle : defaultHandle

    groove: Item {

        anchors.verticalCenter: __horizontal ? parent.verticalCenter : undefined
        anchors.horizontalCenter: !__horizontal ? parent.horizontalCenter : undefined

        //changing between odd and even values causes pixel 'wiggling' as the center anchors move around.
        //can't use anchors.fill because the gradients need rotating
        height: control.height - control.height % 2
        width: control.width - control.width % 2

        WGTextBoxFrame {
            radius: defaultSpacing.halfRadius

            anchors.top: __horizontal ? parent.top : undefined
            anchors.left: !__horizontal ? parent.left : undefined

            width: {
                if (control.offsetArrowHandles)
                {
                    __horizontal ? parent.width : parent.width - 4
                }
                else
                {
                    parent.width
                }
            }
            height: {
                if (control.offsetArrowHandles)
                {
                    __horizontal ? parent.height - 4 : parent.height
                }
                else
                {
                    parent.height
                }
            }
            color: "transparent"

            //grid pattern for transparent colors
            Image {
                source: "icons/bw_check_6x6.png"
                fillMode: Image.Tile
                anchors.fill: parent
                anchors.margins: defaultSpacing.standardBorderSize
                z: -1
            }

            //Item that holds the gradient
            //QML can't make horizontal gradients so this is always vertical, then possibly rotated.

            Item {
                id: gradientFrame
                anchors.centerIn: parent

                height:__horizontal ? parent.width - 2 : parent.height - 2
                width: __horizontal ? parent.height - 2 : parent.width - 2

                rotation: __horizontal ? -90 : 0

                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Repeater {
                        model: control.__colorBarModel

                        Rectangle
                        {
                            id: colorBar

                            property color minColor: control.colorData[minColorVal]
                            property color maxColor: control.colorData[maxColorVal]

                            Connections {
                                target: control
                                onUpdateColorBars: {
                                    colorBar.minColor = control.colorData[minColorVal]
                                    colorBar.maxColor = control.colorData[maxColorVal]
                                }
                            }

                            Layout.fillWidth: true
                            Layout.preferredHeight: (maxValue - minValue) * (gradientFrame.height / (control.maximumValue - control.minimumValue))

                            MouseArea
                            {
                                anchors.fill: parent
                                propagateComposedEvents: true

                                onPressed: {
                                    //adds handles when bar is Shift Clicked
                                    if ((mouse.button == Qt.LeftButton) && (mouse.modifiers & Qt.ShiftModifier) && control.addDeleteHandles)
                                    {
                                        //get the position of the mouse inside the current bar
                                        var mousePos = mapToItem(gradientFrame, mouseX, mouseY)

                                        var newPos = mousePos.y / (gradientFrame.height / (control.maximumValue - control.minimumValue))

                                        //find the mid point
                                        //TODO: Make this find a non-mid point
                                        var midColor = Qt.rgba(((minColor.r + maxColor.r)/2),((minColor.g + maxColor.g)/2),((minColor.b + maxColor.b)/2),((minColor.a + maxColor.a)/2))

                                        //add a new point to the data
                                        control.addData(index, newPos, midColor)
                                    }
                                    else
                                    {
                                        mouse.accepted = false
                                    }
                                }
                            }

                            gradient: Gradient {
                                GradientStop {
                                    position: 0
                                    color: minColor
                                }
                                GradientStop {
                                    position: 1
                                    color: maxColor
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    bar: null
}
