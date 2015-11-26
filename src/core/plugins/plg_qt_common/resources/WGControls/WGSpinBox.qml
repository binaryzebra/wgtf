/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.2
import BWControls 1.0

/*!
 \brief A reimplementation of Spinbox with the following properties:
 Single clicked increment/decrement  via stepSize property
 Click and drag increment/decrement based on vertical linear mouse distance moved
 Multiplier on speed of number change via keyboard toggle (Ctrl) whilst dragging
 Releasing Ctrl drops number change rate back to default
 User can drag up (increment) and then down (decrement) within a single click and hold event allowing for correction of overshoot.
 MouseWheel up/down will increment/decrement based on stepSize property
 Control must be active (selected) before MouseWheel events are enabled
 Left/Middle clicked in the Blue area will cause the text to be selected so that any keyboard entry will wipe the existing data.
 Double left click inserts a cursor at the location of the double click allowing for modification of the existing data
 Right click in the Blue area will cause the text to be selected and bring up a context menu

Example:
\code{.js}
WGSpinBox {
    width: 120
    value: 25
    minimumValue: 0
    maximumValue: 100
}
\endcode
/*

/*TODO:
1. Fast drag will not work if the text box is given focus prior to the use of fast drag
2. The workaround for preventing double click on spinners feels bad
3. Mouse wheel will only increment on spinners if the text box is given focus first.
4. Up Down Keys dont increment value when textbox has focus
*/

/*
    \qmltype SpinBox
    \inqmlmodule QtQuick.Controls
    \since 5.1
    \ingroup controls
    \brief Provides a spin box control.

    SpinBox allows the user to choose a value by clicking the up or down buttons, or by
    pressing up or down on the keyboard. The user can also type the value in manually.

    By default the SpinBox provides discrete values in the range [0-99] with a \l stepSize of 1 and 0 \l decimals.

    \code
    SpinBox {
        id: spinbox
    }
    \endcode

    Note that if you require decimal values you will need to set the \l decimals to a non 0 value.

    \code
    SpinBox {
        id: spinbox
        decimals: 2
    }
    \endcode
*/

Control {
    id: spinbox
    objectName: "WGSpinBox"

    /*TODO:
        WGSpinBox has no implicit height. If used by itself it will not look correct in the UI
        Adding the followign in the style project appears to have no adverse effects
    implicitHeight: defaultSpacing.minimumRowHeight ? defaultSpacing.minimumRowHeight : 22
    */

    /*!
        This property determines the width of the spinner boxes
        The default value is \c 16
    */
    property var spinBoxSpinnerSize: 16

    property alias contentWidth: input.contentWidth

    /*!
        \qmlproperty real SpinBox::value

        The value of this SpinBox, clamped to \l minimumValue and \l maximumValue.

        The default value is \c{0.0}.
    */
    property alias value: validator.value

    /*!
        \qmlproperty real SpinBox::minimumValue

        The minimum value of the SpinBox range.
        The \l value is clamped to this value.

        The default value is \c{0.0}.
    */
    property alias minimumValue: validator.minimumValue

    /*!
        \qmlproperty real SpinBox::maximumValue

        The maximum value of the SpinBox range.
        The \l value is clamped to this value. If maximumValue is smaller than
        \l minimumValue, \l minimumValue will be enforced.

        The default value is \c{99}.
    */
    property alias maximumValue: validator.maximumValue

    /*! \qmlproperty real SpinBox::stepSize
        The amount by which the \l value is incremented/decremented when a
        spin button is pressed.

        The default value is \c{1.0}.
    */
    property alias stepSize: validator.stepSize

    /*! \qmlproperty string SpinBox::suffix
        The suffix for the value. I.e "cm" */
    property alias suffix: validator.suffix

    /*! \qmlproperty string SpinBox::prefix
        The prefix for the value. I.e "$" */
    property alias prefix: validator.prefix

    /*! \qmlproperty int SpinBox::decimals
      This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.
      The default value is \c{0}.
    */
    property alias decimals: validator.decimals

    /*! \qmlproperty font SpinBox::font

        This property indicates the current font used by the SpinBox.
    */
    property alias font: input.font

    /*! This property allows the control to have a default value other than the minimumValue.
        Right clicking the controls spinners will set the value to defaultValue
      */
    property real defaultValue: minimumValue

    /*! This property indicates whether the Spinbox should get active
      focus when pressed.
      The default value is \c true.
    */
    property bool activeFocusOnPress: true

    /*! \qmlproperty enumeration horizontalAlignment
        \since QtQuick.Controls 1.1

        This property indicates how the content is horizontally aligned
        within the text field.

        The supported values are:
        \list
        \li Qt.AlignLeft
        \li Qt.AlignHCenter
        \li Qt.AlignRight
        \endlist

      The default value is style dependent.
    */
    property int horizontalAlignment: Qt.AlignLeft

    /*!
        \qmlproperty bool SpinBox::hovered

        This property indicates whether the control is being hovered.
    */
    readonly property alias hovered: mouseArea.containsMouse

    property bool dragging_: mouseArea.drag.active

    /*!
        \qmlsignal SpinBox::editingFinished()
        \since QtQuick.Controls 1.1

        This signal is emitted when the Return or Enter key is pressed or
        the control loses focus. Note that if there is a validator
        set on the control and enter/return is pressed, this signal will
        only be emitted if the validator returns an acceptable state.

        The corresponding handler is \c onEditingFinished.
    */
    signal editingFinished()

    //style: Qt.createComponent(Settings.style + "/SpinBoxStyle.qml", spinbox)

    /*! \internal */
    property alias __text: input.text

    /*! This property determines if the control is read only
        The default value defined by TextBox base control and is \c false
    */
    property alias readOnly: input.readOnly

    /*! This property is used to define the buttons label when used in a WGFormLayout
        The default value is an empty string
    */
    //TODO: This should be renamed, it does not require "_"
    property string label_: ""

    /*! This property determines if the control will show up and down spinners.
        The default value is an \c true
    */
    property bool hasArrows: true

    /*! This property holds the target control's id to be bound to this controls b_Value */
    property alias b_Target: dataBinding.target

    /*! This property determines b_Target's property which is to be bound to this controls b_Value */
    property alias b_Property: dataBinding.property

    /*! This property determines this control's value which will drive b_Target's b_Property */
    property alias b_Value: dataBinding.value

    /*! This property determines the colour of the text displayed in the control
        The default value is determined by the base TextField control
    */
    property alias textColor: input.textColor

    /*! \internal */
    property real originalValue_: 0 //the value before dragging

    /*! \internal */
    property real tempValueAdd_: 0 //the amount to add to the original value

    /*! \internal */
    property bool fastDrag_: false //if ctrl held down increment is much faster

    /*! \internal */
    property real fakeZero_: 0  //a fake zero after ctrl is held or released

    /*! \internal */
    property bool useValidatorOnInputText: true // Let the validator update the input.text

    property Component textBoxStyle: WGTextBoxStyle{}

    property Component buttonFrame: WGButtonFrame{
        id: button
        radius: 0
        property bool hovered: parent.hovered
        property bool up: parent.up
        property bool pressed: parent.pressed

        Text {
            id: arrowText
            color : palette.NeutralTextColor

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            anchors.horizontalCenterOffset: 1

            font.family : "Marlett"
            font.pixelSize: Math.round(parent.height)

            renderType: Text.QtRendering
            text : button.up ? "\uF074" : "\uF075"
        }

        states: [
            State {
                name: "PRESSED"
                when: button.pressed && spinbox.enabled
                PropertyChanges {target: button; color: palette.DarkShade}
                PropertyChanges {target: button; innerBorderColor: "transparent"}
            },
            State {
                name: "HOVERED"
                when: button.hovered && spinbox.enabled
                PropertyChanges {target: button; highlightColor: palette.LighterShade}
                PropertyChanges {target: arrowText; color: palette.TextColor}
            },
            State {
                name: "DISABLED"
                when: !spinbox.enabled
                PropertyChanges {target: button; color: "transparent"}
                PropertyChanges {target: button; borderColor: palette.DarkShade}
                PropertyChanges {target: button; innerBorderColor: "transparent"}
                PropertyChanges {target: arrowText; color: palette.DisabledTextColor}
            }
        ]
    }

    /*! \internal */
    //increments the value
    function tickValue(amount) {
        value += amount
        if (activeFocus)
            input.selectValue()
    }

    Binding {
        id: dataBinding
    }

    Text {
        id: maxSizeHint
        text: prefix + input.contentWidth + suffix
        font: input.font
        visible: false
    }

    Text {
        id: minSizeHint
        text: prefix + minimumValue.toFixed(decimals) + suffix
        font: input.font
        visible: false
    }

    implicitWidth: {
        if (hasArrows){
            maxSizeHint.paintedWidth + defaultSpacing.doubleMargin + arrowBox.width
        } else {
            maxSizeHint.paintedWidth + defaultSpacing.doubleMargin
        }
    }

    activeFocusOnTab: true


    Accessible.name: input.text
    Accessible.role: Accessible.SpinBox

    Component.onCompleted: {
        copyableControl.disableChildrenCopyable( spinbox );
    }

    WGTextBox {
        id: input
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        assetBrowserContextMenu: false

        activeFocusOnTab: true

        horizontalAlignment: spinbox.horizontalAlignment
        verticalAlignment: Qt.AlignVCenter
        inputMethodHints: Qt.ImhFormattedNumbersOnly

        style: textBoxStyle

        // support copy&paste
        WGCopyable {
            id: copyableControl

            BWCopyable {
                id: copyableObject

                onDataCopied : {
                    setValue( validator.value )
                }

                onDataPasted : {
                    setValueHelper( validator, "value", data );
                    if(validator.value != data)
                    {
                        pasted = false;
                    }
                    else
                    {
                        editingFinished();
                    }
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

        validator: SpinBoxValidator {
            id: validator
            property bool ready: false // Delay validation until all properties are ready
            onTextChanged:
            {
                if (ready && useValidatorOnInputText)
                {
                    input.text = validator.text
                }
            }

            Component.onCompleted:
            {
                if (useValidatorOnInputText)
                {
                    input.text = validator.text
                }
                ready = true
            }
        }

        onAccepted: {
            if (useValidatorOnInputText)
            {
                input.text = validator.text
            }
        }

        //This breaks Tab focus... but not sure if it does anything else useful. Leaving here for now.
        //Keys.forwardTo: spinbox

        onEditAccepted: spinbox.editingFinished()

        function selectValue() {
            select(prefix.length, text.length - suffix.length)
        }
    }

    // Spinbox arrow buttons
    Item {
        id: arrowBox
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        width: spinBoxSpinnerSize

        Loader {
            id: arrowUpButtonFrame
            sourceComponent: buttonFrame
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: Math.round(-(parent.height / 4))
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: parent.opacity

            property bool up: true
            property bool hovered: upButtonMouseArea.containsMouse
            property bool pressed: false

            height: parent.height / 2

            width: parent.width

            MouseArea {
                id: upButtonMouseArea
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                activeFocusOnTab: false
            }
        }

        Loader {
            id: arrowDownButtonFrame
            sourceComponent: buttonFrame
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: Math.round(parent.height / 4)
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: parent.opacity

            property bool up: false
            property bool hovered: downButtonMouseArea.containsMouse
            property bool pressed: false

            height: parent.height / 2

            width: parent.width

            MouseArea {
                id: downButtonMouseArea
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                activeFocusOnTab: false
            }
        }

        states: [
            State {
                name: "ARROWS"
                when: (downButtonMouseArea.containsMouse || upButtonMouseArea.containsMouse || dragBar.Drag.active
                       || ((hasArrows) && (input.contentWidth + defaultSpacing.standardMargin
                                           <= input.width - arrowBox.width)))
                PropertyChanges {target: arrowBox; opacity: 1}
            },
            State {
                name: "NOARROWS"
                when: (!hasArrows || ((hasArrows) && (input.contentWidth + defaultSpacing.standardMargin
                                                      > input.width - arrowBox.width )))
                PropertyChanges {target: arrowBox; opacity: 0}
            }
        ]

        transitions: [
            Transition {
                from: "ARROWS"
                to: "NOARROWS"
                NumberAnimation { properties: "opacity"; duration: 200 }

            },
            Transition {
                from: "NOARROWS"
                to: "ARROWS"
                NumberAnimation { properties: "opacity"; duration: 200 }
            }
        ]
    }

    //invisible line that handles incrementing the value by dragging
    Item {
        id: dragBar
        height: 1
        width: 1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        property int modifier: fastDrag_ ? 1 : 10

        Drag.active: mouseArea.drag.active && !input.readOnly

        states: [
            State {
                when: dragBar.Drag.active

                AnchorChanges {
                    target: dragBar
                    anchors.top: undefined
                }
            }
        ]

        //add the position of the bar to the value. Use a fakezero if fastDrag has been toggled.
        onYChanged:{
            if (Drag.active){
                tempValueAdd_ = (((-y + fakeZero_) / modifier) * stepSize)
                validator.value = originalValue_ + tempValueAdd_
            }
        }

    }

    MouseArea {
        id: mouseArea

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        activeFocusOnTab: false

        anchors.left: undefined
        width: arrowBox.width

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        preventStealing: true
        propagateComposedEvents: true //Allow context menu for textbox

        drag.target: dragBar
        drag.axis: Drag.YAxis

        //start changing the value via dragging dragBar
        // reset the value before and after drag
        drag.onActiveChanged: {
            if (mouseArea.drag.active) {
                originalValue_ = validator.value
            } else {
                tempValueAdd_ = 0
                originalValue_ = 0
                fakeZero_ = 0
                input.focus = false
            }
        }

        onWheel: {
            if (!input.readOnly && input.activeFocus)
            {
                if (wheel.angleDelta.y > 0)
                {
                    tickValue(1)
                }
                else
                {
                    tickValue(-1)
                }

                editingFinished()
            }

            // Returns the wheel controls back to make ScrollView happy
            wheel.accepted = false
        }

        onPressed: {
            // must give spinbox focus to capture control key events
            spinbox.forceActiveFocus()
            if (!input.readOnly)
            {
                var arrowPoint = mouseArea.mapToItem(arrowBox, mouse.x, mouse.y)

                if (arrowBox.contains(Qt.point(arrowPoint.x, arrowPoint.y)))
                {
                    if(mouse.button == Qt.RightButton)
                    {
                        validator.value = defaultValue
                    }

                    if (arrowUpButtonFrame.hovered)
                    {
                        arrowUpButtonFrame.pressed = true
                    }
                    else if (arrowDownButtonFrame.hovered)
                    {
                        arrowDownButtonFrame.pressed = true
                    }
                    editingFinished()
                }
                else if (mouse.button == Qt.RightButton) //mouse is over text box
                {
                    mouse.accepted = false //pass right click to textbox for context menu
                }
            }
        }

        //add/subtract by one if an arrow is clicked. Set to minimum if arrows are right clicked
        onClicked: {
            if (!input.readOnly)
            {
                var arrowPoint = mouseArea.mapToItem(arrowBox, mouse.x, mouse.y)

                if (arrowBox.contains(Qt.point(arrowPoint.x, arrowPoint.y)))
                {
                    if(mouse.button == Qt.RightButton)
                    {
                        validator.value = defaultValue
                    }
                    else if (arrowPoint.y < arrowBox.height / 2)
                    {
                        tickValue(1)
                    }
                    else if (arrowPoint.y > arrowBox.height / 2)
                    {
                        tickValue(-1)
                    }
                    editingFinished()
                    input.focus = false
                }
                else if (mouse.button == Qt.RightButton) //mouse is over text box
                {
                    mouse.accepted = false //pass right click to textbox for context menu
                }
            }
            //need if menu for readonly.. you can copy with readonly but not paste or cut!
        }

        onReleased: {
            arrowUpButtonFrame.pressed = false
            arrowDownButtonFrame.pressed = false
            editingFinished()
            //prevents fastDrag_ getting stuck if mouse is released before key event
            fastDrag_ = false
        }

        //Double click needs to be treated as a second click for incrementing values otherwise the second click is ignored.
        onDoubleClicked: {
            if (!input.readOnly)
            {
                var arrowPoint = mouseArea.mapToItem(arrowBox, mouse.x, mouse.y)

                if (arrowBox.contains(Qt.point(arrowPoint.x, arrowPoint.y)))
                {
                    if(mouse.button == Qt.RightButton)
                    {
                        validator.value = defaultValue
                    }
                    else if (arrowPoint.y < arrowBox.height / 2)
                    {
                        tickValue(1)
                    }
                    else if (arrowPoint.y > arrowBox.height / 2)
                    {
                        tickValue(-1)
                    }
                    editingFinished()
                    input.focus = false
                }
                else if (mouse.button == Qt.RightButton) //mouse is over text box
                {
                    mouse.accepted = false //pass right click to textbox for context menu
                }
            }
            //need if menu for readonly.. you can copy with readonly but not paste or cut!
        }
    }

    Keys.onUpPressed: {
        if (!input.readOnly)
        {
            tickValue(1)
        }
    }
    Keys.onDownPressed: {
        if (!input.readOnly)
        {
            tickValue(-1)
        }
    }

    //toggle fastDrag_ with Ctrl. Also set a new zero point so current value can be changed instead of the original value.
    Keys.onPressed: {
        if (event.key == Qt.Key_Control)
        {
            fastDrag_ = true
            if (dragBar.Drag.active)
            {
                validator.value = originalValue_ + tempValueAdd_
                originalValue_ = validator.value
                tempValueAdd_ = 0
                fakeZero_ = dragBar.y
            }
        }
    }
    Keys.onReleased: {
        if (event.key == Qt.Key_Control)
        {
            fastDrag_ = false
            if (dragBar.Drag.active)
            {
                validator.value = originalValue_ + tempValueAdd_
                originalValue_ = validator.value
                tempValueAdd_ = 0
                fakeZero_ = dragBar.y
            }
        }
    }

    Item {
        id: container
    }

}
