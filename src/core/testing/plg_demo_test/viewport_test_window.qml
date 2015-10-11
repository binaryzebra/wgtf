import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import WGControls 1.0
import QtGraphicalEffects 1.0


Rectangle {
	id: mainWindow

    property real zoom: 1

    property bool showControls: true
    property bool showImage: true

    property bool slideshow: false

    property alias hue: hueShift.hue
    property alias saturation: hueShift.saturation
    property alias lightness: hueShift.lightness

    property var title: "QML Controls Test Window"

    color: palette.MainWindowColor

    property var viewportList: ["qrc:///viewports/viewport4","qrc:///viewports/viewport1","qrc:///viewports/viewport2","qrc:///viewports/viewport3","qrc:///viewports/viewport5","qrc:///viewports/viewport6","qrc:///viewports/viewport7","qrc:///viewports/viewport8","qrc:///viewports/viewport9","qrc:///viewports/viewport10","qrc:///viewports/viewport11"]
	property int currentViewport: 0

	Image {
        id: background
		anchors.centerIn: parent
        source: showImage ? viewportList[currentViewport] : ""
        width: 1024*zoom
        height: 768*zoom

		MouseArea {
			acceptedButtons: Qt.RightButton | Qt.LeftButton
			anchors.fill: parent
			onClicked: {
				if (mouse.button == Qt.LeftButton){
					if (currentViewport < viewportList.length - 1){
						currentViewport += 1
					} else {
						currentViewport = 0
					}
				} else if (mouse.button == Qt.RightButton){
					if (currentViewport > 0){
						currentViewport -= 1
					} else {
						currentViewport = viewportList.length - 1
					}
				}
			}
		}
	}

    Timer {
        interval: 3000
        repeat: true
        running: slideshow

        onTriggered: {
            if (currentViewport < viewportList.length - 1)
            {
                currentViewport += 1
            }
            else
            {
                currentViewport = 0
            }
        }
    }

    HueSaturation {
        id: hueShift
        anchors.fill: background
        cached: true
        source: background
    }

	WGExpandingRowLayout {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.margins: 5

        height: defaultSpacing.minimumRowHeight

		//Put test viewport controls in here
		//These are all mockup controls to test styling only.

		WGToolButton {
			iconSource: "qrc:///icons/menu_16x16"
            opacity: {
                if (showControls)
                {
                    return 1
                }
                else if (hovered)
                {
                    return 1
                }
                else
                {
                    return 0.1
                }
            }

			menu: WGMenu {
				title: "View Options"
				MenuItem {
                    text: "Reset All Options"
                    onTriggered: {
                        hueShift.hue = 0
                        hueShift.saturation = 0
                        hueShift.lightness = 0
                        mainWindow.zoom = 1
                    }
				}
                MenuSeparator{}
				MenuItem {
                    text: "Show Controls"
					checkable: true
                    checked: true
                    onTriggered: {
                        showControls = checked
                    }
				}
                MenuItem {
                    text: "Show Background"
                    checkable: true
                    checked: true
                    onTriggered: {
                        showImage = checked
                    }
                }
			}
        }

        WGLabel {
            text: "HSL: "
            visible: showControls
        }

        WGSplitTextFrame {
            visible: showControls
            boxList: [
                WGNumberBox{
                    id: hueBox
                    value: 0
                    minimumValue: -100
                    maximumValue: 100

                    onValueChanged: {
                        mainWindow.hue = value / maximumValue
                    }

                    Connections {
                        target: mainWindow
                        onHueChanged: {
                            hueBox.value = hue * maximumValue
                        }
                    }
                },
                WGNumberBox{
                    id: saturationBox
                    value: 0
                    minimumValue: -100
                    maximumValue: 100

                    onValueChanged: {
                        mainWindow.saturation = value / maximumValue
                    }

                    Connections {
                        target: mainWindow
                        onSaturationChanged: {
                            saturationBox.value = saturation * maximumValue
                        }
                    }
                },
                WGNumberBox{
                    id: lightnessBox
                    value: 0
                    minimumValue: -100
                    maximumValue: 100

                    onValueChanged: {
                        mainWindow.lightness = value / maximumValue
                    }

                    Connections {
                        target: mainWindow
                        onLightnessChanged: {
                            lightnessBox.value = lightness * maximumValue
                        }
                    }
                }
            ]
            height: 24
            Layout.preferredWidth: 120
        }



		WGLabel {
            text: "Zoom: "
            visible: showControls
		}

		WGSliderControl {
            id: zoomSlider
            visible: showControls
            Layout.preferredWidth: 120
            minimumValue: 1
            maximumValue: 3
            stepSize: 0.1
            value: 1

            property real tempValue: slider.value

            onTempValueChanged: {
                mainWindow.zoom = tempValue
            }

            Connections {
                target: mainWindow
                onZoomChanged:{
                    zoomSlider.value = zoom
                }
            }
        }

        WGPushButton {
            text: "Reset"
            iconSource: "qrc:///icons/loop_16x16"
            Layout.preferredWidth: 100
            visible: showControls

            onClicked:{
                hueShift.hue = 0
                hueShift.saturation = 0
                hueShift.lightness = 0
                mainWindow.zoom = 1
            }
        }

		Rectangle {
			color: "transparent"
			Layout.fillWidth: true
			Layout.preferredHeight: 1
		}

		ExclusiveGroup {
            id: slideshowGroup
		}

        WGLabel {
            text: "Slideshow: "
            visible: showControls
        }

		WGToolButton {
            visible: showControls
            iconSource: "qrc:///icons/pause_16x16"
            exclusiveGroup: slideshowGroup
			checkable: true
			checked: true

            onCheckedChanged: {
                if(checked)
                {
                    slideshow = false
                }
            }
		}
		WGToolButton {
            visible: showControls
            iconSource: "qrc:///icons/play_16x16"
            exclusiveGroup: slideshowGroup
			checkable: true
			checked: false

            onCheckedChanged: {
                if(checked)
                {
                    slideshow = true
                }
            }
        }

		WGDropDownBox {
            visible: showControls
			id: dropDown

			model: ListModel {
                ListElement { text: "WoT: Fire and Smoke" }
                ListElement { text: "WoWP: Dark Blue Night" }
                ListElement { text: "WoT: Night City" }
                ListElement { text: "WoWS: Bright Morning" }
                ListElement { text: "Blitz: Asian Green" }
                ListElement { text: "WoT: Desert" }
                ListElement { text: "Blitz: Light Desert" }
                ListElement { text: "WoWS: Dark Green Island" }
                ListElement { text: "WoWP: Dark Grey City" }
                ListElement { text: "WoWP: Light Desert" }
                ListElement { text: "WoWS: Blue Sky" }
			}

            onCurrentIndexChanged: {
                mainWindow.currentViewport = currentIndex
            }

            Connections {
                target: mainWindow
                onCurrentViewportChanged: {
                    dropDown.currentIndex = currentViewport
                }
            }
		}
	}
	Text {
        visible: showControls
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.margins: 5
		horizontalAlignment: Text.AlignRight
		color: "white"
        text: "This window demonstrates controls styled so they overlay a simulated 3D viewport."
	}
}