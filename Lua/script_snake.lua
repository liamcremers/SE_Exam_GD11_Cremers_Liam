local WINDOW_WIDTH = 640
local WINDOW_HEIGHT = 560
local WINDOW_FPS = 240
local WINDOW_FONT = Font.new("Press Start 2P", false, false, false, 64)
function initializeWindow()
    GAME_ENGINE:SetTitle("Snake")
    GAME_ENGINE:SetWidth(WINDOW_WIDTH);
    GAME_ENGINE:SetHeight(WINDOW_HEIGHT);
    GAME_ENGINE:SetFrameRate(WINDOW_FPS);
    GAME_ENGINE:SetFont(WINDOW_FONT)
end
local VK_LEFT = 0x25
local VK_UP = 0x26
local VK_RIGHT = 0x27
local VK_DOWN = 0x28
function AddKeyListKeys()
    keyList = string.char(VK_LEFT) .. string.char(VK_RIGHT) .. string.char(VK_UP) .. string.char(VK_DOWN)
    GAME_ENGINE:SetKeyList(keyList)
end

local Grid = {}
local GRID_SIZE = 20
function createGrid()
    Grid = {}
    for i = 0, WINDOW_WIDTH, GRID_SIZE do
        for j = 0, WINDOW_HEIGHT, GRID_SIZE do
            table.insert(Grid, {
                x = i,
                y = j
            })
        end
    end
end
function drawGrid()
    GAME_ENGINE:SetColor(RGB(100, 100, 100))
    for i = 1, #Grid do
        GAME_ENGINE:DrawLine(Grid[i].x, Grid[i].y, Grid[i].x + GRID_SIZE, Grid[i].y)
        GAME_ENGINE:DrawLine(Grid[i].x, Grid[i].y, Grid[i].x, Grid[i].y + GRID_SIZE)
        GAME_ENGINE:DrawLine(Grid[i].x + GRID_SIZE, Grid[i].y, Grid[i].x + GRID_SIZE, Grid[i].y + GRID_SIZE)
        GAME_ENGINE:DrawLine(Grid[i].x, Grid[i].y + GRID_SIZE, Grid[i].x + GRID_SIZE, Grid[i].y + GRID_SIZE)
    end
end

local Snake = {}
Snake.__index = Snake

-- Constructor
function Snake:new(x, y, initial_length, direction)
    local snake = {
        body = {}, -- Stores segments of the snake as {x, y} pairs
        direction = direction or "right", -- Initial direction
        grow_pending = 0, -- Tracks how many segments to grow
        grid_size = GRID_SIZE -- Default grid size for movement
    }

    -- Initialize the snake body
    for i = 0, initial_length - 1 do
        table.insert(snake.body, {
            x = x - i * snake.grid_size,
            y = y
        })
    end

    setmetatable(snake, Snake)
    return snake
end
function createSnake()
    mySnake = Snake:new(100, 100, 1, "right")
end

-- Update the snake's direction (ignores reverse directions)
function Snake:setDirection(new_direction)
    local opposite = {
        up = "down",
        down = "up",
        left = "right",
        right = "left"
    }

    if new_direction ~= opposite[self.direction] then
        self.direction = new_direction
    end
end

-- Move the snake
local move_speed = 200
function Snake:move(elapsedSeconds)
    local head = self.body[1] -- Current head of the snake
    local new_head = {
        x = head.x,
        y = head.y
    }

    self.partial_move = (self.partial_move or 0) + move_speed * elapsedSeconds

    local distance = 0

    if self.partial_move >= self.grid_size then
        distance = self.grid_size
        self.partial_move = self.partial_move - self.grid_size
    end

    -- Update position based on direction
    if self.direction == "up" then
        new_head.y = new_head.y - distance
    elseif self.direction == "down" then
        new_head.y = new_head.y + distance
    elseif self.direction == "left" then
        new_head.x = new_head.x - distance
    elseif self.direction == "right" then
        new_head.x = new_head.x + distance
    end

    -- Insert the new head at the front of the body
    table.insert(self.body, 1, new_head)

    -- Remove the tail if not growing
    if self.grow_pending > 0 then
        self.grow_pending = self.grow_pending - 1
    else
        table.remove(self.body)
    end
end

function Snake:grow(segments)
    self.grow_pending = self.grow_pending + (segments or 20)
end

-- Check if the snake collides with itself
function Snake:checkSelfCollision()
    local head = self.body[1]
    for i = 2, #self.body do
        if head.x == self.body[i].x and head.y == self.body[i].y then
            return true
        end
    end
    return false
end

-- Check if the snake is out of bounds
function Snake:checkOutOfBounds(width, height)
    local head = self.body[1]
    return head.x < 0 or head.y < 0 or head.x >= width or head.y >= height
end

-- Draw the snake
function Snake:draw()
    GAME_ENGINE:SetColor(RGB(255, 255, 255))
    for _, segment in ipairs(self.body) do
        GAME_ENGINE:FillRect(segment.x, segment.y, segment.x + GRID_SIZE, segment.y + GRID_SIZE)
    end
end

local Apple = {}
local apple_size = GRID_SIZE

function createApple()
    Apple.x = math.random(0, (WINDOW_WIDTH / GRID_SIZE) - 1) * GRID_SIZE
    Apple.y = math.random(0, (WINDOW_HEIGHT / GRID_SIZE) - 1) * GRID_SIZE
end

function drawApple()
    GAME_ENGINE:SetColor(RGB(255, 0, 0))
    GAME_ENGINE:FillRect(Apple.x, Apple.y, Apple.x + apple_size, Apple.y + apple_size)
end

function checkAppleCollision()
    local head = mySnake.body[1]
    if head.x == Apple.x and head.y == Apple.y then
        mySnake:grow()
        createApple()
    end
end

-- ______________________________________________________________________________________________________________________
function initialize()
    initializeWindow()
    AddKeyListKeys()
    createGrid()
    createSnake()
    createApple()
end

function paint()
    GAME_ENGINE:FillWindowRect(RGB(0, 0, 0))

    drawGrid()
    mySnake:draw()
    drawApple()
end

function tick()
    local elapsed_time = GAME_ENGINE:GetFrameDelay() * 0.001

    mySnake:move(elapsed_time)

    checkAppleCollision()
    if mySnake:checkSelfCollision() or mySnake:checkOutOfBounds(WINDOW_WIDTH, WINDOW_HEIGHT) then
        endGame()
    end
end
function key_pressed(key)
    if key == VK_UP then
        mySnake:setDirection("up")
    elseif key == VK_DOWN then
        mySnake:setDirection("down")
    elseif key == VK_LEFT then
        mySnake:setDirection("left")
    elseif key == VK_RIGHT then
        mySnake:setDirection("right")
    end
end