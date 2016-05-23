import QtQuick 2.3
import QtQuick.Controls 1.2
import WGControls 1.0
import WGControls 2.0 as WGTwo
import BWControls 1.0

WGTwo.WGDropDownBox {
    id: combobox
    objectName:  itemData != null ? itemData.IndexPath : "polystruct_component"
    anchors.left: parent.left
    anchors.right: parent.right

    Component.onCompleted: {
        currentIndex = Qt.binding( function() {
            var modelIndex = polyModel.find( itemData.Definition, "Value" );
            return polyModel.indexRow( modelIndex ); } )
    }

    model: polyModel
    textRole: "display"

    WGListModel {
        id: polyModel
        source: itemData.DefinitionModel

        ValueExtension {}
    }

    Connections {
        target: combobox
        onCurrentIndexChanged: {
            if (currentIndex < 0) {
                return;
            }
            var modelIndex = polyModel.index( currentIndex );
            itemData.Definition = polyModel.data( modelIndex, "Value" );
        }
    }
}
