/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2019 Jean-Christophe FRISCH <natureh.510@gmail.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "editid.h"
#include "cursormanager.h"
#include "../rtengine/coord.h"

class Geometry;
class EditDataProvider;

/** @file
 * See editwidgets.h for documentation
 */

/// @brief Method for client tools needing Edit information
class EditSubscriber
{

public:

private:
    EditUniqueID ID; /// this will be used in improcfun to locate the data that has to be stored in the buffer; it must be unique in RT
    EditType editingType;
    BufferType bufferType;
    EditDataProvider *provider;

protected:
    std::vector<Geometry*> visibleGeometry;    /// displayed geometry
    std::vector<Geometry*> mouseOverGeometry;  /// mouseOver geometry, drawn in a hidden buffer
    enum {
        ES_ACTION_NONE,      ///
        ES_ACTION_DRAGGING,  /// set action to this value in the buttonPressed event to start dragging and ask for drag event
        ES_ACTION_PICKING    /// set action to this value in the buttonPressed event whenever the user is picking something through a single click. In this case, the pickX events will be called INSTEAD of buttonXReleased !
    } action;                /// object mode only, ignored in Pipette mode

public:
    explicit EditSubscriber (EditType editType);
    virtual ~EditSubscriber () {}

    void               setEditProvider(EditDataProvider *provider);
    EditDataProvider*  getEditProvider ();
    void               setEditID(EditUniqueID ID, BufferType buffType);
    bool               isCurrentSubscriber();
    virtual void       subscribe();
    virtual void       unsubscribe();
    virtual void       switchOffEditMode ();    /// Occurs when the user want to stop the editing mode
    EditUniqueID       getEditID();
    EditType           getEditingType();
    BufferType         getPipetteBufferType();
    bool               isDragging();            /// Returns true if something is being dragged and drag events has to be sent (object mode only)
    bool               isPicking();             /// Returns true if something is being picked

    /** @brief Get the cursor to be displayed when above handles
    @param objectID object currently "hovered" */
    virtual CursorShape getCursor (const int objectID);

    /** @brief Triggered when the mouse is moving over an object
    This method is also triggered when the cursor is moving over the image in ET_PIPETTE mode
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool mouseOver (const int modifierKey);

    /** @brief Triggered when mouse button 1 is pressed, together with the CTRL modifier key if the subscriber is of type ET_PIPETTE
    Once the key is pressed, RT will enter in drag1 mode on subsequent mouse movements
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool button1Pressed (const int modifierKey);

    /** @brief Triggered when mouse button 1 is released
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool button1Released ();

    /** @brief Triggered when mouse button 2 is pressed (middle button)
    Once the key is pressed, RT will enter in drag2 mode on subsequent mouse movements
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool button2Pressed (const int modifierKey);

    /** @brief Triggered when mouse button 2 is released (middle button)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool button2Released ();

    /** @brief Triggered when mouse button 3 is pressed (right button)
    Once the key is pressed, RT will enter in drag3 mode on subsequent mouse movements
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool button3Pressed (const int modifierKey);

    /** @brief Triggered when mouse button 3 is released (right button)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool button3Released ();

    /** @brief Triggered when the user is moving while holding down mouse button 1
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool drag1 (const int modifierKey);

    /** @brief Triggered when the user is moving while holding down mouse button 2
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool drag2 (const int modifierKey);

    /** @brief Triggered when the user is moving while holding down mouse button 3
    @param modifierKey Gtk's event modifier key (GDK_CONTROL_MASK | GDK_SHIFT_MASK | ...)
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool drag3 (const int modifierKey);

    /** @brief Triggered when the user is releasing mouse button 1 while in action==ES_ACTION_PICKING mode
    No modifier key is provided, since having a different modifier key than on button press will set picked to false.
    @param picked True if the cursor is still above the the same object than on button pressed and with the same modifier keys.
                  If false, the user moved the cursor away or the modifier key is different, so the element is considered as NOT selected.
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool pick1 (const bool picked);

    /** @brief Triggered when the user is releasing mouse button 2 while in action==ES_ACTION_PICKING mode
    @param picked True if the cursor is still above the the same object than on button pressed and with the same modifier keys.
                  If false, the user moved the cursor away or the modifier key is different, so the element is considered as NOT selected.
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool pick2 (const bool picked);

    /** @brief Triggered when the user is releasing mouse button 3 while in action==ES_ACTION_PICKING mode
    @param picked True if the cursor is still above the the same object than on button pressed and with the same modifier keys.
                  If false, the user moved the cursor away or the modifier key is different, so the element is considered as NOT selected.
    @return true if the preview has to be redrawn, false otherwise */
    virtual bool pick3 (const bool picked);

    /** @brief Get the geometry to be shown to the user */
    const std::vector<Geometry*>& getVisibleGeometry ();

    /** @brief Get the geometry to be drawn in the "mouse over" channel, hidden from the user */
    const std::vector<Geometry*>& getMouseOverGeometry ();
};

/** @brief Class to handle the furniture of data to the subscribers.
 *
 * It is admitted that only one Subscriber can ask data at a time. If the Subscriber is of type ET_PIPETTE, it will have to
 * trigger the usual event so that the image will be reprocessed to compute the buffer of the current subscriber.
 */
class EditDataProvider
{

private:
    EditSubscriber *currSubscriber;

public:
    int object;            /// ET_OBJECTS mode: Object detected under the cursor, 0 otherwise; ET_PIPETTE mode: 1 if above the image, 0 otherwise
    float pipetteVal[3];   /// Current pipette values; if bufferType==BT_SINGLEPLANE_FLOAT, #2 & #3 will be set to 0

    rtengine::Coord posScreen;       /// Location of the mouse button press, in preview image space
    rtengine::Coord posImage;        /// Location of the mouse button press, in the full image space
    rtengine::Coord deltaScreen;     /// Delta relative to posScreen
    rtengine::Coord deltaImage;      /// Delta relative to posImage
    rtengine::Coord deltaPrevScreen; /// Delta relative to the previous mouse location, in preview image space
    rtengine::Coord deltaPrevImage;  /// Delta relative to the previous mouse location, in the full image space

    EditDataProvider();
    virtual ~EditDataProvider() {}

    virtual void        subscribe(EditSubscriber *subscriber);
    virtual void        unsubscribe();         /// Occurs when the subscriber has been switched off first
    virtual void        switchOffEditMode ();  /// Occurs when the user want to stop the editing mode
    virtual CursorShape getCursor(int objectID);
    int                 getPipetteRectSize ();
    EditSubscriber*     getCurrSubscriber();
    virtual void        getImageSize (int &w, int&h) = 0;
};

inline EditDataProvider* EditSubscriber::getEditProvider () {
    return provider;
}

inline CursorShape EditSubscriber::getCursor (const int objectID) {
    return CSHandOpen;
}

inline bool EditSubscriber::mouseOver (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::button1Pressed (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::button1Released () {
    return false;
}

inline bool EditSubscriber::button2Pressed (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::button2Released () {
    return false;
}

inline bool EditSubscriber::button3Pressed (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::button3Released () {
    return false;
}

inline bool EditSubscriber::drag1 (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::drag2 (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::drag3 (const int modifierKey) {
    return false;
}

inline bool EditSubscriber::pick1 (const bool picked) {
    return false;
}

inline bool EditSubscriber::pick2 (const bool picked) {
    return false;
}

inline bool EditSubscriber::pick3 (const bool picked) {
    return false;
}

inline const std::vector<Geometry*>& EditSubscriber::getVisibleGeometry () {
    return visibleGeometry;
}

inline const std::vector<Geometry*>& EditSubscriber::getMouseOverGeometry () {
    return mouseOverGeometry;
}

inline int EditDataProvider::getPipetteRectSize () {
    return 8; // TODO: make a GUI
}
