import QtQuick 2.3
import QtQuick.Layouts 1.1

/*!
    \brief a column of objects that expands to fill the space.
    By default only has 1 column. Has 2 columns if formLayout_ = true with labels added to the first column.

\code{.js}
WGColumnLayout {
    WGCheckBox {
        text: "File Option 1"
    }
    WGCheckBox {
        text: "File Option 2"
    }
    WGCheckBox {
        enabled: false
        text: "Disabled File Option"
    }
}
\endcode
*/

GridLayout {
    id: mainColumn
    objectName: "WGColumnLayout"

    //TODO: Is this property used?
    /*! This property sets the row height.
        The default value is set by defaultSpacing.minimumRowHeight
    */
    property int rowHeight_: defaultSpacing.minimumRowHeight

    /*! This property moves all children into a second column.
        Creates labels if possible in the first column.
        The default value is \c false
    */
    property bool formLayout_: false

    /*! This property should be set to true if you don't want the label width being influenced by other WGColumnLayouts with formLayout_: true
        The default value is \c false
    */
    property bool localForm_: false

    /*! This property is used to define the buttons label when used in a WGFormLayout
        The default value is an empty string
    */
    //TODO: This should be renamed, it does not require "_"
    property string label_: ""

    /*! This property
        The default value is \c
    */
    rowSpacing: defaultSpacing.rowSpacing

    columns: formLayout_ ? 2 : 1

    //Only valid for form layouts
    Component.onCompleted: {
        if (formLayout_)
        {
            var labelList = []
            var heightList = []

            for (var i=0; i<children.length; i++)
            {
                children[i].Layout.alignment = Qt.AlignLeft | Qt.AlignTop

                //if child has a label_ adds it to the label list otherwise adds a blank label
                if (children[i].label_ != "")
                {
                    labelList.push(children[i].label_)
                }
                else
                {
                    labelList.push("")
                }
                //moves the child to the second column and puts it down a row to avoid warning messages
                children[i].Layout.column = 1
                children[i].Layout.row = i + 1
                heightList.push(children[i].height) //records the childs height

            }

            //creates the labels
            for (var j=0; j<labelList.length; j++)
            {
                //if height=0  & no label_, don't make a blank label (this is usually so Repeaters don't take a row for themselves)
                if (heightList[j] > 0 && labelList[j] != "")
                {
                    //Sets properties in the label to use the panel wide form width
                    if (!localForm_)
                    {
                        var newLabel = Qt.createComponent("WGLabel.qml");
                        if (newLabel.status === Component.Ready)
                        {
                            newLabel.createObject(mainColumn, {
                                                       "text": labelList[j],
                                                       "formLabel_": true,
                                                       "Layout.minimumHeight": heightList[j],
                                                       "Layout.column": 0,
                                                       "Layout.row": j+1
                                                   });
                        }

                    //Sets properties in the label to ignore the panel wide form width and use the columns instead
                    }
                    else
                    {
                        var newLabel = Qt.createComponent("WGLabel.qml");
                        if (newLabel.status === Component.Ready)
                        {
                            newLabel.createObject(mainColumn, {
                                                       "text": labelList[j],
                                                       "formLabel_": true,
                                                       "localForm_": true,
                                                       "Layout.alignment": Qt.AlignRight,
                                                       "Layout.minimumHeight": heightList[j],
                                                       "Layout.column": 0,
                                                       "Layout.row": j+1
                                                   });

                        }
                    }
                }
            }

        }
    }
}
