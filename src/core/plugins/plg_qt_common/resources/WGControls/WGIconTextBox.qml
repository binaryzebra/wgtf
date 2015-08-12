import QtQuick 2.3
import QtQuick.Controls 1.2
import BWControls 1.0

//WIP

Item {
    property bool isLeft_ : true
    property alias placeholderText : textBox.placeholderText
    property alias label_ : textBox.label_
    property alias text : textBox.text

    implicitHeight: {
        if (defaultSpacing.minimumRowHeight){
            defaultSpacing.minimumRowHeight
        } else {
            22
        }
    }

    TextField {
        id: textBox
        property bool noFrame_ : false
        property string label_: ""

        anchors.left: isLeft_ ? icon.right : parent.left
        anchors.right: isLeft_ ? parent.right : icon.left
        anchors.leftMargin: isLeft_ ? defaultSpacing.rowSpacing : undefined
        anchors.rightMargin: isLeft_ ? undefined : defaultSpacing.rowSpacing

        activeFocusOnTab: enabled

		// support copy&paste
		WGCopyable {
			id: copyableControl

			BWCopyable {
				id: copyableObject

				onDataCopied : {
					setValue( textBox.text )
				}

				onDataPasted : {
					textBox.text = data
				}
			}

			onSelectedChanged : {
				if(selected)
				{
					selectControl( copyableObject )
				}
				else
				{
					deselectControl( copyableObject )
				}
			}
		}

        implicitHeight: {
            if (defaultSpacing.minimumRowHeight){
                defaultSpacing.minimumRowHeight
            } else {
                22
            }
		}

        //Placeholder text in italics
        font.italic: text == "" ? true : false

        style: WGTextBoxStyle {

        }
    }

    Image {
        id: icon
        source: "icons/search_16x16.png"
        anchors.left: isLeft_ ? parent.left : undefined
        anchors.right: isLeft_ ? undefined : parent.right
        anchors.verticalCenter: parent.verticalCenter

        width : 16
        height: 16
    }

}
