import QtQuick 2.3
import QtQuick.Layouts 1.1
import WGControls 1.0

/*!
 \brief A vector4 component variation of the vectorN control for reflected data
*/

WGVector4 {
    id: vector4
    objectName: itemData != null ? itemData.IndexPath : "vector4_component"
    value: itemData.Value

    Binding {
        target: itemData
        property: "Value"
        value: vector4.value
    }
}
