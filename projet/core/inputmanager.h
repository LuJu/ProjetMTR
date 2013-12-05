#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Qt>
#include <QDebug>
#include <QPoint>
#include "uistate.h"

class InputManager
{
public:
    enum EventTypes {
        Key_Pressed,
        Key_Released,
        Moved
    };



    //! Indicates the press of a key on the keyboard
    /*!
        \n Status  1 : not implemented
        \param  key The key pressed
        \param eventType Key_Pressed or Key_Released
    */
    void keyboardEvent( int key,int eventType);

    //! Indicates the press of a key on the mouse
    /*!
        This function only concerns key pushes, for the wheel, see wheel() method.
        \n Status  1 : not implemented
        \param  key The key pressed
        \param eventType Key_Pressed or Key_Released
    */
    void mouseEvent(int eventType, QPoint pos, int key=0);

    //! Gets the action of a key
    /*!
        Returns the actions done by the key passed in parameter, -1 if no action is done
        \n Status  1 : not implemented
        \param  key The key to return the action
        \return The action done by the key, or -1 if no action is done
    */
    qint16 get_key_action(int key);

    //! Indicates an event with the wheel of the mouse
    /*!
        description
        \n Status  1 : not implemented
        \param  orientation Vertical or horizontal
        \param  delta The distance of movement
    */
    void wheelEvent(int orientation, int delta, QPoint pos);

    //! Allows to change the default assignment of a key
    /*!
        \n Status  1 : not implemented
        \param old_key_id The key the is to be changed
        \param new_key The new key ID
    */
    bool changeAssignment(int old_key_id, int new_key);

    //! Puts the assignment of all keys back to default
    /*!
    */
    void resetAssignment();

    static InputManager * instance();
private:
    InputManager();
    InputManager(const InputManager&);


    static const quint8 NUMBER_OF_KEYS=9;
    static const int _default_keyboard_matching[NUMBER_OF_KEYS];
    bool _keyboard_activated;
    bool _mouse_activated;
    bool _mouse_left_dragging;
    bool _keys_activated[NUMBER_OF_KEYS];
    int _keyboard_matching[NUMBER_OF_KEYS];
    UIState * _ui;
};

#endif // INPUTMANAGER_H
