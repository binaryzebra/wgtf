import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQml.Models 2.2
import WGControls 2.0


/*!
 \brief WGListView displays data from a model defined by its delegate.
 The WGListView is contructed from a WGListViewBase which creates rows and columns.

Example:
\code{.js}

ScrollView {
    anchors.top: lastControl.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    WGListView {
        id: example
        model: sourceModel
        columnWidth: 50
        columnSpacing: 1
        columnDelegates: [columnDelegate, exampleDelegate]
        roles: ["value"]
        model: sourceModel

        Component {
            id: exampleDelegate

            Text {
                id: textItem

                visible: typeof itemData.value === "string"
                text: typeof itemData.value === "string" ? itemData.value : ""
                color: palette.textColor
            }
        }
    }
}

\endcode
*/
WGListViewBase {
    id: listView

    property alias roles: itemView.roles

    /*! The default component to be used for columns that are not specified
        by columnDelegates.
    */
    property alias columnDelegate: itemView.columnDelegate

    /*! A list of components to be used for each column.
        Item 0 for column 0, item 1 for column 1 etc.
        If a column is not in the list, then it will default to columnDelegate.
        The default value is an empty list.
    */
    property alias columnDelegates: itemView.columnDelegates

    /*! This property holds a list of indexes to adapt from the model's columns
        to the view's columns.
        e.g. if the input model has 1 column, but columnSequence is [0,0,0]
             then the view can have 3 columns that lookup column 0 in the model.
        The default value is an empty list
    */
    property alias columnSequence: itemView.columnSequence
    property alias columnWidth: itemView.columnWidth
    property alias columnWidths: itemView.columnWidths
    property alias columnSpacing: itemView.columnSpacing

    property alias internalModel: listView.model

    /*! This property holds the data model information that will be displayed
        in the view.
    */
    property alias model: itemView.model

    property var extensions: []

    function moveKeyHighlightPrevious(event) {
        // Move keyboard highlight
        keyboardHighlightModelIndex = listExtension.getPreviousIndex(keyboardHighlightModelIndex);

        // When Shift is pressed, selection area increases with the keyboard highlight
        if (event.modifiers & Qt.ShiftModifier) {
            var selection = listExtension.itemSelection(itemView.selectionModel.currentIndex, keyboardHighlightModelIndex);
            itemView.selectionModel.select(selection,
                itemView.selectionModel.Clear | ItemSelectionModel.Select);
        }

        // When Ctrl is not pressed, selection moves with the keyboard highlight
        else if (!(event.modifiers & Qt.ControlModifier)) {
            itemView.selectionModel.setCurrentIndex(keyboardHighlightModelIndex,
                ItemSelectionModel.Clear | ItemSelectionModel.Select);
        }
    }

    function moveKeyHighlightNext(event) {
        // Move keyboard highlight
        keyboardHighlightModelIndex = listExtension.getNextIndex(keyboardHighlightModelIndex);

        // When Shift is pressed, selection area increases with the keyboard highlight
        if (event.modifiers & Qt.ShiftModifier) {
            var selection = listExtension.itemSelection(itemView.selectionModel.currentIndex, keyboardHighlightModelIndex);
            itemView.selectionModel.select(selection,
                itemView.selectionModel.Clear | ItemSelectionModel.Select);
        }

        // When Ctrl is not pressed, selection moves with the keyboard highlight
        else if (!(event.modifiers & Qt.ControlModifier)) {
            itemView.selectionModel.setCurrentIndex(keyboardHighlightModelIndex,
                ItemSelectionModel.Clear | ItemSelectionModel.Select);
        }
    }

    clip: true
    view: itemView
    internalModel: itemView.extendedModel

    Keys.onUpPressed: {
        if (orientation == ListView.Vertical) {
            moveKeyHighlightPrevious(event);
        }
    }
    Keys.onDownPressed: {
        if (orientation == ListView.Vertical) {
            moveKeyHighlightNext(event);
        }
    }
    Keys.onLeftPressed: {
        if (orientation == ListView.Horizontal) {
            moveKeyHighlightPrevious(event);
        }
    }
    Keys.onRightPressed: {
        if (orientation == ListView.Horizontal) {
            moveKeyHighlightNext(event);
        }
    }

    // Data holder for various C++ extensions.
    // Pass it down to children
    WGItemViewCommon {
        id: itemView

        // WGControls.ListExtension C++
        ListExtension {
            id: listExtension
        }

        property var listExtensions: listView.extensions.concat(commonExtensions.concat([listExtension]))
        extensions: listExtensions

        Connections {
            target: listView
            onItemPressed: {
                // Variable exists for the entire function scope
                var selection;
                if ((mouse.modifiers & Qt.ShiftModifier) && (mouse.modifiers & Qt.ControlModifier)) {
                    selection = listExtension.itemSelection(itemView.selectionModel.currentIndex, rowIndex);
                    itemView.selectionModel.select(selection,
                        ItemSelectionModel.Select);
                }
                else if (mouse.modifiers & Qt.ShiftModifier) {
                    selection = listExtension.itemSelection(itemView.selectionModel.currentIndex, rowIndex)
                    itemView.selectionModel.select(selection,
                        itemView.selectionModel.Clear | ItemSelectionModel.Select);
                }
                else if (mouse.modifiers & Qt.ControlModifier) {
                    itemView.selectionModel.setCurrentIndex(rowIndex,
                        ItemSelectionModel.Toggle);
                }
                else {
                    itemView.selectionModel.setCurrentIndex(rowIndex,
                        ItemSelectionModel.Clear | ItemSelectionModel.Select);
                }
                listView.keyboardHighlightModelIndex = rowIndex;
            }
        }
    }
}
