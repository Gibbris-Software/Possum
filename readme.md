Possum Game Engine
------------------

The Possum game engine is a creation of Gibbris for use in their game Harley.
It is built on SFML, and using SFML directly is required.

It uses scenes with entities to manage game state.

Concepts
========

- Game

    The game is an object connected to an SFML window, a scene, and global state.
    It manages things like the event loop, passing events (including collisions, 
    updates, and redrawing) to the entities in the current scene, and redrawing the screen
    when the `run` method is called. There can only be one running game per thread, since
    `run` blocks until the window is closed.

- Scene

    A scene is a collection of entities and a background color. Entities in a scene are created using 
    the `create` method on a scene.

- Entity

    An entity is a game object with a position and radius, along with an SFML texture and sprite.
    It also has it's own state. Event callbacks can be assigned using the `register_event` method.
    `register_event` calls can be chained.

- State

    States, whether attached to the game or an entity, is a mapping of strings to integers.
    Values are assigned by the `set` method, and retrieved by the `get` method.

- Callback

    An event callback is a function with the signature `void blah(possum::Entity&, possum::State&, void*)`.
    The first argument is the entity the event is called on, the second is the global state of the game,
    and the third is a pointer to whatever data is needed for the event, as shown in the table below.

    <table>
        <th>
            <td>Event</td><td>Data</td>
        </th>
        <tr>
            <td>possum::UPDATE</td><td>Elapsed time since last update, as sf::Time</td>
        </tr>
        <tr>
            <td>possum::REDRAW</td><td>sf::RenderWindow for drawing sprites.</td>
        </tr>
        <tr>
            <td>possum::COLLISION</td><td>possum::Entity that hit this entity.</td>
        </tr>
        <tr>
            <td>possum::BUTTON_UP, BUTTON_DOWN</td><td>sf::Event::MouseButtonEvent. It is not guaranteed to have hit the entity.<td>
        </tr>
        <tr>
            <td>possum::POINTER_MOVE</td><td>sf::Event::MouseMoveEvent</td>
        </tr>
        <tr>
            <td>possum::KEY_UP, KEY_DOWN</td><td>sf::Event::KeyEvent</td>
        </tr>
        <tr>
            <td>possum::USER_EVENT_x</td><td>Whatever you send it. Possum will never use these.</td>
        </tr>
    </table>

For more information and an example of usage, see the possum game in the examples directory.

Compilation
===========

For most cases, you can just copy these files into your project and compile as normal.

### Examples

possum_test.cpp: g++ examples/possum_test.cpp src/game.cpp src/scene.cpp -I include -lsfml-system -lsfml-window -lsfml-graphics -o examples/possum_test
