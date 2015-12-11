import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import BWControls 1.0
import WGControls 1.0

WGPanel {
	id: rootFrame
	title: "Tree to List Test"
	layoutHints: { 'test': 0.1 }
	property var sourceModel: source
	property var topControlsHeight: 20
	color: palette.MainWindowColor
	z: 1

	// Internal bool to track what view was clicked for a selection
	property bool wasTreeClick: false

	// Data Models
	WGFilteredTreeModel {
		id: testTreeModel
		source: sourceModel

		filter: WGTokenizedStringFilter {
			id: stringFilter			
			filterText: searchBox.text
			splitterChar: " "
		}

        ValueExtension {}
        ColumnExtension {}
        ComponentExtension {}
        TreeExtension {
			id: treeModelExtension            
			selectionExtension: treeModelSelection
		}
        ThumbnailExtension {}
        SelectionExtension {
            id: treeModelSelection
        }
	}

	WGTreeListAdapter {
		id: testTreeListAdapter

		ValueExtension {}
		SelectionExtension {
			id: testListAdapterSelection
		}

		onSourceIndexChanged: {
			testListView.forceLayout();
		}
	}

	// Main Layout
	ColumnLayout {
        id: viewsColumnLayout
        anchors.fill: parent		

		WGExpandingRowLayout {
			id: searchRowLayout
			anchors.fill: parent
		
			Label {
				id: searchBoxLabel
				width: 100
				height: topControlsHeight
				text: "Search:"
			}

			WGTextBox {
				id: searchBox
				Layout.fillWidth: true
				height: topControlsHeight
			}
		}//searchRowLayout

		WGExpandingRowLayout {
			id: mainRowLayout
			width: 300
			height: 300

			WGTreeView {
				id: testTreeView
				model: testTreeModel
				Layout.fillHeight: true
				Layout.fillWidth: true
				rightMargin: 8 // leaves just enought space for conventional slider
				columnDelegates: [defaultColumnDelegate]
				selectionExtension: treeModelSelection
				treeExtension: treeModelExtension
				childRowMargin: 2
				columnSpacing: 4
				lineSeparator: false

				autoUpdateLabelWidths: true

				flatColourisation: false
				depthColourisation: 5

				onRowClicked: {				
					testTreeListAdapter.sourceIndex = modelIndex;
				}
			}//WGTreeView

			WGListView {
				id: testListView
				model: testTreeListAdapter
				Layout.fillHeight: true
				Layout.fillWidth: true
				selectionExtension: testListAdapterSelection
				columnDelegates: [defaultColumnDelegate]
			}//WGListView
		}//mainRowLayout
	}//ColumnLayout
}
