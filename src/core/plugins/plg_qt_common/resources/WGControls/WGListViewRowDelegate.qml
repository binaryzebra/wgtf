import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import BWControls 1.0

/*!
 \brief WGListViewRowDelegate is used within WGListView's delegate.
 WGListViewRowDelegate will load WGListViewColumnDelegate in its delegate or fall back to a default if none exists.
 WGListViewRowDelegate should only be used within the contexts of a ListView.
*/

Item {
    id: rowDelegate
    objectName: "WGListViewRowDelegate"
    height: minimumRowHeight
    clip: true

    //TODO: This needs testing
    /*!
        This property defines the indentation before the first element on each row
        The default value is \c 0
    */
    property int indentation: 0

    /*!
        This property holds the index of the selected row in the list
    */
    property int rowIndex: index

    //TODO: Improve documentation
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

    //TODO: Improve documentation
    /*!
        This property describes mouse selection behaviour
    */
    property var selectionExtension: null

    //TODO: Improve documentation
    /*! This signal is sent on a single click
    */
    signal clicked(var mouse)

    //TODO: Improve documentation
    /*! This signal is sent on double click
    */
    signal doubleClicked(var mouse)

    MouseArea {
        id: itemMouseArea
        parent:rowDelegate.parent
        anchors.fill: rowDelegate
        hoverEnabled: true

        onPressed: {
            if (mouse.button === Qt.LeftButton && selectionExtension != null)
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
                    if (multiSelect)
                    {
                        selectionExtension.clearOnNextSelect();
                    }

                    Selected = true;
                }
            }
        }

        onClicked: {
            rowDelegate.clicked(mouse)
            rowDelegate.parent.forceActiveFocus()
        }
        onDoubleClicked: rowDelegate.doubleClicked(mouse)

        Rectangle {
            id: selectionHighlight
            color: rowDelegate.parent.activeFocus ? palette.HighlightShade : palette.LightestShade
            anchors.fill: itemMouseArea
            anchors.margins: selectionMargin
            visible: selectionExtension != null && Selected
        }

        Rectangle {
            id: mouseOverHighlight
            anchors.fill: itemMouseArea
            visible: itemMouseArea.containsMouse
            color: palette.LighterShade
        }

        ListView {
            id: columns
            model: ColumnModel
            x: indentation
            width: parent.width - indentation
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

                sourceComponent:
                    columnIndex < columnDelegates.length ? columnDelegates[columnIndex] :
                    defaultColumnDelegate

                onLoaded: {
                    var widthFunction = function()
                    {
                        var firstColumn = Math.ceil(columns.width * 0.25);
                        var otherColumns = Math.ceil(columns.width * 0.75);

                        if(columnIndex == 0)
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
