import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import BWControls 1.0

// Thumbnail with default size and boder
// Use WGThumbnailButton if thumbnail is editable

/*!
 \brief Thumbnail image with default size and boder.
 Use WGThumbnailButton if thumbnail is meant as a button.

Intended use:
Where a thumbnail of an asset is to be displayed.
 For example, in the asset browser

\code{.js}
WGThumbnail{
    parentControl: (id_of_parent_control)
    parentControlsMouseOverInfo: (id_of_parent_control).mouseOverInfo
}
\endcode
*/

Rectangle {
    id: thumbnail
    objectName: "WGThumbnail"

    /*! This property holds the location of the image to be displayed as the thumbnail
        The default value is false
    */
    //TODO: This should be renamed, it does not require "_". In WGThumbnailButton it is called iconSource.
    property url source_: ""

    /*! This property holds the default text string that will be displayed if /c source_ is not defined.
        The default value is /c "Image not found"
    */
    property string notFoundString: "Image not found"

    color: palette.LightShade

    implicitWidth: {
        defaultSpacing.minimumRowHeight * 4
    }

    implicitHeight: {
        defaultSpacing.minimumRowHeight * 4
    }

    // support copy&paste
    WGCopyable {
        id: copyableControl

        BWCopyable {
            id: copyableObject

            onDataCopied : {
                setValue( thumbnail.source_ )
            }

            onDataPasted : {
                // readonly control
                console.log("ReadOnly Control WGThumbnail");
                //thumbnail.source_ = data
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

    Item {
        anchors.fill: parent
        Image {
            id: icon
            anchors.fill: parent
            anchors.margins: defaultSpacing.standardBorderSize
            source: thumbnail.source_

            Component.onCompleted: {
                if (thumbnail.source_ == ""){
                    missingText.visible = true
                }
            }
        }
        WGLabel{
            id: missingText
            anchors.centerIn: parent
            width: (parent.width - (defaultSpacing.leftMargin + defaultSpacing.rightMargin))
            horizontalAlignment: "AlignHCenter"
            verticalAlignment: "AlignVCenter"
            text: notFoundString
            visible: false
            wrapMode: "Wrap"
        }
    }
}
