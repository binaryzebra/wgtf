import QtQuick 2.3
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.1

/*!
 \brief Raised button with border and label.
 Button width determined by label width.
 Can have text, text+icon or just icon

Example:
\code{.js}
WGPushButton {
    text: "Open"
    iconSource: "icons/file"
}
\endcode
*/

WGAbstractButton {
    id: pushButton
    objectName: "WGPushButton"

    /*! This property determines the checked state of the control
        The default value is false
    */
    property bool checked: false

    buttonChecked: checked

    /*! This property determines the radius of the button corners
    */
    property int radius: defaultSpacing.standardRadius

    /*! This property is used to define the buttons label when used in a WGFormLayout
        The default value is an empty string
    */
    //TODO: This should be renamed, it does not require "_"
    property string label_: ""

	/*! This property holds the menu for the button
		This intentionally hides the menu property of the Button base class
	*/
	property WGMenu menu: null

    /*! This property determines if the down arrow should appear if the button has a menu
    */
    property bool showMenuIndicator: true

    onButtonCheckedChanged: {
        setValueHelper( pushButton, "checked", buttonChecked);
    }

    onCheckedChanged: {
        buttonChecked = checked
    }

    implicitHeight: defaultSpacing.minimumRowHeight ? defaultSpacing.minimumRowHeight : 22

    /*! This property holds the target control's id to be bound to this control's b_Value */
    property alias b_Target: dataBinding.target

    /*! This property determines b_Target's property which is to be bound to this control's b_Value */
    property alias b_Property: dataBinding.property

    /*! This property determines this control's value which will drive b_Target's b_Property */
    property alias b_Value: dataBinding.value

    Binding {
        id: dataBinding

    }

    style: WGButtonStyle{

    }

	Connections {
        target: __behavior
		onReleased: {
            if (__behavior.containsMouse && menu)
                menu.popup()
        }
    }
}
