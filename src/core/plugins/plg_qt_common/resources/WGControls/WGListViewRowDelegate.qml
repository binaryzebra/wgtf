import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import BWControls 1.0

/*!
 \brief WGListViewRowDelegate is used within WGListView's delegate.
 WGListViewRowDelegate will load custom column delegates in its delegate or fall back to a default if none exists.
 WGListViewRowDelegate should only be used within the contexts of a ListView.
 See WGTreeItem for an example of its use.

*/

Item {
    id: rowDelegate
    objectName: "WGListViewRowDelegate"
    height: minimumRowHeight
    clip: true

    property int handlePosition

    /*!
        This property defines the indentation before the first element on each row
        The default value is \c 0
    */
    property int indentation: 0

    /*!
        This property holds the index of the selected row in the list
    */
    property int rowIndex: index

	/*!
		This property represents the model index (QModelIndex) of the selected row in the list
	*/
	property var modelIndex: null

    /*!
        This property contains a default column delegate.
        The default value is \c null
    */
    property var defaultColumnDelegate: null

    /*!
        This property contains the items to be delegated by the WGListViewRowDelegate's delegate.
        The default value is an empty list
    */
    property var columnDelegates: []

    /*!
        This property describes mouse selection behaviour
    */
    property var selectionExtension: null

    /*! This property holds the active focus state of the control
        The default value is \c false */
    property bool hasActiveFocusDelegate: false

	/* This property specifies the main colour for the row background */
	property bool showBackgroundColour: false

	/* This property specifies the main colour for the row background */
	property color backgroundColour: palette.MidDarkColor

	/* This property specifies the alternate colour for the row background */
	property color alternateBackgroundColour: Qt.darker(palette.MidLightColor,1.2)

    /*! This signal is sent on a single click
    */
    signal clicked(var mouse)

    //TODO: Improve documentation
    /*! This signal is sent on a double click
    */
    signal doubleClicked(var mouse)

    MouseArea {
        id: itemMouseArea
        parent: rowDelegate.parent
        anchors.fill: rowDelegate
        hoverEnabled: true
		acceptedButtons: Qt.RightButton | Qt.LeftButton;

        onPressed: {
			if ((selectionExtension == null) || (typeof Selected == 'undefined'))
			{
				return;
			}

            if (mouse.button == Qt.LeftButton || mouse.button == Qt.RightButton)
            {
                var multiSelect = selectionExtension.multiSelect;

                if (mouse.modifiers & Qt.ControlModifier)
                {
                    Selected = !Selected;
                }
                else if (mouse.modifiers & Qt.ShiftModifier)
                {
                    if (multiSelect)
                    {
                        selectionExtension.prepareRangeSelect();
                        Selected = true;
                    }
                }
                else
                {
                    if (multiSelect && ((mouse.button == Qt.LeftButton) || (mouse.button == Qt.RightButton && modelIndex != null && !selectionExtension.indexInSelection(modelIndex))))
                    {
                        selectionExtension.clearOnNextSelect();
                    }

                    Selected = true;
                }
            }
        }

        onClicked: {
            rowDelegate.clicked(mouse)
            // NOTE: Do not give the parent active focus here. The tree view and the list view have different ways to utilize
            //		 us, so giving parent focus will break keyboard input event handles.
        }

        onDoubleClicked: rowDelegate.doubleClicked(mouse)

    	Rectangle {
    		id: background
    		anchors.fill: parent
    		visible: rowDelegate.showBackgroundColour
    		color: index % 2 === 0 ? rowDelegate.backgroundColour : rowDelegate.alternateBackgroundColour
    	}

    	Rectangle {
            id: selectionHighlight
            color: hasActiveFocusDelegate ? palette.HighlightShade : "grey"
            anchors.fill: itemMouseArea
            anchors.margins: selectionMargin
            visible: !itemMouseArea.pressed && typeof Selected != 'undefined' && Selected
        }

        Rectangle {
            id: mouseOverHighlight
            anchors.fill: itemMouseArea
            visible: itemMouseArea.containsMouse
            opacity: 0.5
            color: palette.HighlightShade
        }

        ListView {
            id: columns

            model: ColumnModel
            x: indentation
            width: Math.max(0, parent.width - indentation)
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            orientation: Qt.Horizontal
            interactive: false
            spacing: columnSpacing

            delegate: Loader {
                id: columnDelegate

                anchors.top: parent.top
                anchors.bottom: parent.bottom

                property var itemData: model
                property int rowIndex: rowDelegate.rowIndex
                property int columnIndex: index
                property int indentation: rowDelegate.indentation

                sourceComponent:
                    columnIndex < columnDelegates.length ? columnDelegates[columnIndex] :
                    defaultColumnDelegate

                onLoaded: {
                    var widthFunction = function()
                    {
                        if (columns.count < 2)
                        {
                        	return columns.width
                        }

                        var wholeRowWidth = columns.width + indentation
                        var firstColumn = wholeRowWidth * rowDelegate.handlePosition / wholeRowWidth - indentation
                        var otherColumns = columns.width - firstColumn

                        if (columnIndex == 0)
                        {
                            return firstColumn - columnSpacing;
                        }
                        else
                        {
                            return Math.ceil((otherColumns - columnSpacing) / (columns.count - 1));
                        }
                    }

                    item.width = Qt.binding(widthFunction);
                    rowDelegate.height = Math.max(height, minimumRowHeight);
                }
            }
        }
    }
}
