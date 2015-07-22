import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

TabViewStyle {
    tabsMovable: true
    frameOverlap: 0
    property int tabHeight: 22
    property int tabHeightDiff: 2
    property int tabMinWidth: 60
    property int tabSpacer: 0
    property int tabTextSpacer: 10
	   
    tab: Rectangle { //The space encompassed by the tabs including gap and top
            id: tabSpace
			color: "transparent"
            implicitWidth: Math.max(text.width + tabTextSpacer + tabSpacer, tabMinWidth + tabSpacer)
            implicitHeight: tabHeight

            Rectangle { //Used to anchor the tabs to the bottom
                id: visualTabSpace
                color: "transparent"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                implicitWidth: Math.max(text.width + tabTextSpacer, tabMinWidth)
                implicitHeight: tabHeight

                Rectangle { //The visible tab, only the top half shown, rest obscured.
                    id: tabTopHalf

                    anchors.bottom: (tabPosition == Qt.TopEdge)? parent.bottom : undefined
                    anchors.top: (tabPosition == Qt.BottomEdge)? parent.top : undefined

                    anchors.left: parent.left
                    color: styleData.selected ? palette.MidLightColor : palette.MidDarkColor
                    /*gradient: Gradient { //Tab highlight gradient
                        GradientStop { position: 0.0; color: styleData.selected ? palette.LighterShade : palette.MidDarkColor }
                        GradientStop { position: 0.5; color: styleData.selected ? palette.MidLightColor : palette.MidDarkColor }
                    }*/                    
                    implicitWidth: Math.max(text.width + tabTextSpacer, tabMinWidth)
                    implicitHeight: styleData.selected ? tabHeight :  tabHeight -  tabHeightDiff
                    radius: 0 //tabHeight / 4

                    Rectangle { //Bottom half of the tab for obscuring rounded bottoms of tab
                        id: tabBottomHalf
                        anchors.top: parent.verticalCenter
                        color: styleData.selected ? palette.MidLightColor : palette.MidDarkColor
                        implicitWidth: Math.max(text.width + tabTextSpacer, tabMinWidth)
                        implicitHeight: parent.height / 2
                        radius: 0
                    }
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: styleData.selected ? palette.TextColor : palette.DisabledTextColor                        
                    }
					Rectangle { //highlight line at top of tab
                        id: topTabHighlight
                        anchors.top: parent.top
                        implicitHeight: 1
                        implicitWidth: Math.max(text.width + tabTextSpacer, tabMinWidth)
                        //color: styleData.selected ? palette.LightestShade : palette.LighterShade
						color: (tabPosition == Qt.BottomEdge)?( "transparent") : (styleData.selected ? palette.LightestShade : palette.LighterShade)
                    }
                    Rectangle { //highlight remover line at bottom of tab if not selected
                        id: bottomTabHighlight
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        implicitHeight: 1
                        implicitWidth: parent.width //Math.max(text.width + tabTextSpacer -1, tabMinWidth -1)
					   color: (tabPosition == Qt.BottomEdge)?(styleData.selected ? palette.DarkerShade: "transparent") : (styleData.selected ? "transparent" : palette.LightestShade)
                    }
                    Rectangle { //Shading on the right side of a tab
                        id: rightTabShading
                        anchors.right: parent.right
                        anchors.top: topTabHighlight.top
                        implicitHeight: styleData.selected ? parent.height -0 : parent.height -1
                        implicitWidth: 1
                        color: palette.DarkShade
                    }
                    Rectangle { //Highlight on the left side of a tab
                        id: leftTabHighlight
                        anchors.left: parent.left
                        anchors.top: topTabHighlight.bottom
                        implicitHeight: styleData.selected ? parent.height -1 : parent.height -2
                        implicitWidth: 1
                        color: styleData.selected ? palette.LightestShade : palette.LighterShade
                    }
                    Rectangle { //Active focus highlight
                        //anchors.centerIn: parent
                        anchors.bottom: parent.bottom
                        height: parent.height - defaultSpacing.doubleBorderSize + 1
                        width: parent.width - defaultSpacing.doubleBorderSize
                        color : "transparent"
                        border.width: 1
                        border.color: styleData.activeFocus ? palette.HighlightColor : "transparent"
						
                    }
                }
            }
    }

    leftCorner: Rectangle { //Area spacer on the left of tabs
        color:"transparent" //covered by tabBar
        implicitWidth: defaultSpacing.standardMargin
        implicitHeight: tabHeight
    }

    tabBar: Rectangle {
        color: palette.MainWindowColor
        Rectangle { //highlight line at bottom of tabbar
            id: bottomTabBarHighlight
            anchors.bottom: parent.bottom
            implicitHeight: 1
            implicitWidth: parent.width
			color: (tabPosition == Qt.BottomEdge)?( "transparent") : palette.LightestShade
        }
    }

    frame: Rectangle {            
            color: palette.MidLightColor
        }
}
