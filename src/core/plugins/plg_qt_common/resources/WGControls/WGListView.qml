import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

ListView {
    id: listView
	boundsBehavior: Flickable.StopAtBounds
    currentIndex: -1
	clip: true
	leftMargin: 2
	rightMargin: 2
	topMargin: 2
	bottomMargin: 2
	spacing: 1
	
	//TODO: Move WGPanelProperties into C++ as a context property.
    property QtObject panelProps: WGPanelProperties{}
	property var selectionExtension: null
	property var columnDelegates: []
	property real minimumRowHeight: panelProps.rowHeight_
	property real columnSpacing: 1
	property real selectionMargin: 0
	property bool verticalScrollBar: true
	
	property var defaultColumnDelegate: Component {
		Item {
			Layout.fillWidth: true
			Layout.preferredHeight: minimumRowHeight

			Rectangle {
				anchors.fill: parent
				color: palette.MainWindowColor
			}
			
			Text {
				id: value
				clip: true
				anchors.left: parent.left
				anchors.top: parent.top
				anchors.bottom: parent.bottom
				anchors.margins: 4
				verticalAlignment: Text.AlignVCenter
				text: typeof itemData.Value === "string" ? itemData.Value : typeof itemData.Value
				color: palette.TextColor
			}
		}
	}

	delegate: WGListViewRowDelegate {
		anchors.left: parent.left
		width: parent.width - leftMargin - rightMargin - (verticalScrollBar ? verticalScrollBar.collapsedWidth : 0) - 1
		defaultColumnDelegate: listView.defaultColumnDelegate
		columnDelegates: listView.columnDelegates
		selectionExtension: listView.selectionExtension
	}

	WGScrollBar {
		id: verticalScrollBar
		width: panelProps.rightMargin_
		anchors.top: listView.top
		anchors.right: listView.right
		anchors.bottom: listView.bottom
		orientation: Qt.Vertical
		position: listView.visibleArea.yPosition
		pageSize: listView.visibleArea.heightRatio
		scrollFlickable: listView
		visible: listView.contentHeight > listView.height && verticalScrollBar
	}
}