POSSUM = 0
TREE = 1
ACORN = 2

--[[
Global variables set up before this loads:
    state: Holds data attached to the game
      v_x, v_y, v_width, v_height: Viewport of screen
      width: The width of the screen
      height: The height of the screen
      scene: The index of the current scene. Setting this may cause another scene file to be loaded.
      ... other values are converted to ints and stored in the Game's state, then given to all future callbacks
  
    scene: Interface to the scene described by this file
      create(entity): Adds an entity to the scene

    *entity: Description of an entity, created in Lua
       type: a number to identify a group of similar entities, used mainly in collisions
       x, y: collision center of entity (position)
       radius: radius of the entity, used internally for collision detection
       texture: texture (index) used by entity
       visible: for invisible walls and similar things
       callbacks: Called when things happen, with the entity, the scene, the game state, and another argument. If nil, nothing happens
         update: called with time since last update in seconds. Schould move the entity, spawn any new entities, or do anything else necessary.
         keypress: called when a key is pressed, with the keycode
         keyrelease: ''          ''     released,   ''
         buttonpress/release: same with mouse buttons, which button  (1 2 3)
         collide: called when two entities get close to each other, determined by positions and radii. Argument is the other entity
         motion: called when the mouse pointer moves with {x, y}
       sprite: interface to SFML sprite
         x, y: position of center of sprite. Not always same as entity position, and must be set separately
         rotation: angle in degrees the sprite is rotated
         scale: how much the sprite is scaled when drawing
         texture_rect: {x=, y=, w=, h=} controls what is drawn to the screen of the texture
       data: table of random information attached to the entity. All values converted to ints and sotred in the Entity state. You should just add values to the entity table unless you actually need this.
         
]]

local possum = {type=POSSUM, x=320, y=240, radius=10, texture="possum.png", visible=true, callbacks={}, rotation=0}

possum.callbacks.update = function(e, scene, state, time)
    e.x = e.x + time * math.cos(math.rad(e.rotation))*60
    e.y = e.y + time * math.sin(math.rad(e.rotation))*60
    --e.sprite.x = e.x
    --e.sprite.y = e.y
end

possum.callbacks.keypress = function(e, scene, state, key)
    if key == keyboard.Left then
        e.rotation = e.rotation - 10
    elseif key == keyboard.Right then
        e.rotation = e.rotation + 10
    end
end

possum.callbacks.collide = function(e, scene, state, other)
    if other.type == TREE then
        local factor = (other.radius + e.radius)^2 / ((other.x - e.x)^2 + (other.y - e.y)^2)
        e.x = other.x + factor * (e.x - other.x)
        e.y = other.y + factor * (e.y - other.y)
    elseif other.type == ACORN then
        other.dead = true
        state.scene = state.scene + 1
    end
end

scene.create(possum)

local acorn = {type=ACORN, x=math.random(640), y=math.random(480), radius=5, texture="acorn.png", visible=true}
scene.create(acorn)

local function collide_tree(e, scene, state, other)
    if other.type == TREE then
        e.x = math.random(640)
        e.y = math.random(480)
        e.sprite.x = e.x
        e.sprite.y = e.y
    end
end

local function click_tree(e, scene, state, event)
    if event.button == mouse.left and ((event.x - e.x)^2 + (event.y - e.y)^2) < e.radius^2 then
        e.dead = true;
        state["killed_trees"] = state["killed_trees"] + 1
    end
end

for i = 0, state.scene do
    local tree = {type=TREE, x=math.random(640), y=math.random(480), radius=20, texture="tree.png", visible=true, callbacks={collide=collide_tree}}
    scene.create(tree)
end
