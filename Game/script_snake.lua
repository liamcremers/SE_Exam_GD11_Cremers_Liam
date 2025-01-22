local game_lost = false

local WINDOW_WIDTH = 640
local WINDOW_HEIGHT = 560
local WINDOW_FPS = 240
local WINDOW_FONT = Font.new("Press Start 2P", false, false, false, 64)
local function initializeWindow()
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
local function AddKeyListKeys()
    keyList = string.char(VK_LEFT) .. string.char(VK_RIGHT) .. string.char(VK_UP) .. string.char(VK_DOWN)
    keyList = keyList .. "R"
    GAME_ENGINE:SetKeyList(keyList)
end

local Grid = {}
local GRID_SIZE = 20
local function createGrid()
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

local function drawGrid()
    GAME_ENGINE:SetColor(RGB(100, 100, 100))
    for i = 0, WINDOW_WIDTH, GRID_SIZE do
        GAME_ENGINE:DrawLine(i, 0, i, WINDOW_HEIGHT)
    end
    for j = 0, WINDOW_HEIGHT, GRID_SIZE do
        GAME_ENGINE:DrawLine(0, j, WINDOW_WIDTH, j)
    end
end

-- Define the Snake class
local Snake = {}
Snake.__index = Snake

local Direction = {
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
}

local opposite = {
    [Direction.UP] = Direction.DOWN,
    [Direction.DOWN] = Direction.UP,
    [Direction.LEFT] = Direction.RIGHT,
    [Direction.RIGHT] = Direction.LEFT
}

local key_to_direction = {
    [VK_UP] = Direction.UP,
    [VK_DOWN] = Direction.DOWN,
    [VK_LEFT] = Direction.LEFT,
    [VK_RIGHT] = Direction.RIGHT
}

--- Creates a new snake object.
--- @param x number The initial x-coordinate of the snake's head.
--- @param y number The initial y-coordinate of the snake's head.
--- @param initial_length number The initial length of the snake.
--- @param direction Direction The initial direction of the snake's movement (default is Direction.RIGHT).
--- @return Snake A new Snake object.
--- @class Snake
--- @field hitbody HitRegion[] The segments of the snake's body as HitRegions.
--- @field direction Direction The current direction of the snake's movement.
--- @field grow_pending number The number of segments the snake needs to grow.
function Snake:new(x, y, initial_length, direction)
    local snake = {
        hitbody = {},
        direction = direction,
        grow_pending = 0
    }

    for i = 0, initial_length - 1 do
        local left = math.floor(x - i * GRID_SIZE)
        local top = math.floor(y)
        local right = math.floor(x - i * GRID_SIZE + GRID_SIZE)
        local bottom = math.floor(y + GRID_SIZE)

        table.insert(snake.hitbody, HitRegion.new(Shape.RECTANGLE, left, top, right, bottom))
    end

    setmetatable(snake, Snake)
    return snake
end

local function createSnake()
    mySnake = Snake:new(100, 100, 1, Direction.RIGHT)
end

--- @param new_direction number: The new direction to set (must be a Direction enum).
function Snake:setDirection(new_direction)
    if new_direction and new_direction ~= opposite[self.direction] and self.canChangeDirection then
        self.direction = new_direction
        self.canChangeDirection = false
    end
end

local snakeSpeed = 200
--- @param elapsedSeconds number: The time elapsed since the last frame.
function Snake:move(elapsedSeconds)
    local moveDistance = snakeSpeed * elapsedSeconds
    self.partialMove = (self.partialMove or 0) + moveDistance

    -- Only move the snake when enough distance has accumulated for one grid step
    if self.partialMove >= GRID_SIZE then
        local dx, dy = 0, 0
        if self.direction == Direction.UP then
            dy = -GRID_SIZE
        elseif self.direction == Direction.DOWN then
            dy = GRID_SIZE
        elseif self.direction == Direction.LEFT then
            dx = -GRID_SIZE
        elseif self.direction == Direction.RIGHT then
            dx = GRID_SIZE
        end

        -- Store current positions of each segment
        local prev_positions = {}
        for i, segment in ipairs(self.hitbody) do
            local bounds = segment:GetBounds()
            prev_positions[i] = {
                left = bounds.left,
                top = bounds.top
            }
        end

        -- Move the head
        self.hitbody[1]:Move(dx, dy)

        -- Update body segments to follow the previous one
        for i = 2, #self.hitbody do
            self.hitbody[i]:Move(prev_positions[i - 1].left - self.hitbody[i]:GetBounds().left,
                prev_positions[i - 1].top - self.hitbody[i]:GetBounds().top)
        end

        self.partialMove = self.partialMove - GRID_SIZE
        self.canChangeDirection = true
    end
end

--- @param segments number: The number of segments to add (default 1).
function Snake:grow(segments)
    local segments_to_add = segments or 1

    for _ = 1, segments_to_add do
        -- Get the position of the last segment (tail)
        local tail = self.hitbody[#self.hitbody]:GetBounds()
        local new_left, new_top = tail.left, tail.top

        if self.direction == Direction.UP then
            new_top = new_top + GRID_SIZE
        elseif self.direction == Direction.DOWN then
            new_top = new_top - GRID_SIZE
        elseif self.direction == Direction.LEFT then
            new_left = new_left + GRID_SIZE
        elseif self.direction == Direction.RIGHT then
            new_left = new_left - GRID_SIZE
        end

        -- Add the new segment
        table.insert(self.hitbody,
            HitRegion.new(Shape.RECTANGLE, new_left, new_top, new_left + GRID_SIZE, new_top + GRID_SIZE))
    end
end

-- Check if the snake collides with itself
function Snake:checkSelfCollision()
    local head = self.hitbody[1]:GetBounds()
    for i = 2, #self.hitbody do
        local segment = self.hitbody[i]:GetBounds()
        if head.left == segment.left and head.top == segment.top then
            game_lost = true
        end
    end
end

-- Check if the snake is out of bounds
function Snake:checkOutOfBounds(width, height)
    local head = self.hitbody[1]:GetBounds()
    if head.left < 0 or head.top < 0 or head.right > width or head.bottom > height then
        game_lost = true
    end
end

-- Draw the snake
function Snake:draw()
    for i = 1, #self.hitbody do
        local segment = self.hitbody[i]:GetBounds()
        local color_intensity = math.max(100, 255 - (i * 20))
        GAME_ENGINE:SetColor(RGB(0, color_intensity, 0))
        GAME_ENGINE:FillRect(segment.left, segment.top, segment.right, segment.bottom)
    end
end

local Apple = {}
local apple_size = GRID_SIZE

local function createApple()
    x = math.random(0, (WINDOW_WIDTH / GRID_SIZE) - 1) * GRID_SIZE
    y = math.random(0, (WINDOW_HEIGHT / GRID_SIZE) - 1) * GRID_SIZE
    right = x + apple_size
    bottom = y + apple_size
    Apple = HitRegion.new(Shape.RECTANGLE, x, y, right, bottom)
end

local function drawApple()
    GAME_ENGINE:SetColor(RGB(255, 0, 0))
    appleBounds = Apple:GetBounds()
    GAME_ENGINE:FillRect(appleBounds.left, appleBounds.top, appleBounds.right, appleBounds.bottom)
end

local function checkAppleCollision()
    local head = mySnake.hitbody[1] -- Use the correct snake head
    if head:HitTest(Apple) then
        mySnake:grow() -- Grow the snake by 1 segment
        createApple() -- Spawn a new apple
    end
end

local function displayYouLostMessage()
    GAME_ENGINE:SetColor(RGB(255, 255, 255))
    message = "You lost :("
    Dimensions = GAME_ENGINE:CalculateTextDimensions(message, WINDOW_FONT)
    textWidth = Dimensions.cx
    textHeight = Dimensions.cy
    x = math.floor((WINDOW_WIDTH / 2) - textWidth / 2)
    y = math.floor((WINDOW_HEIGHT / 2) - textHeight / 2)
    GAME_ENGINE:DrawString(message, x, y)

    message = "Press R to restart"
    Dimensions = GAME_ENGINE:CalculateTextDimensions(message, WINDOW_FONT)
    textWidth = Dimensions.cx
    textHeight = Dimensions.cy
    x = math.floor((WINDOW_WIDTH / 2) - textWidth / 2)
    y = math.floor((WINDOW_HEIGHT / 2) + textHeight)
    GAME_ENGINE:DrawString(message, x, y)
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
    if game_lost then
        displayYouLostMessage()
        return
    else
        drawGrid()
        mySnake:draw()
        drawApple()
    end
end

function tick()
    if game_lost then
        return
    end
    if mySnake:checkSelfCollision() or mySnake:checkOutOfBounds(WINDOW_WIDTH, WINDOW_HEIGHT) then
        game_lost = true
    end

    local elapsed_time = GAME_ENGINE:GetFrameDelay() * 0.001
    mySnake:move(elapsed_time)
    checkAppleCollision()
end

function key_pressed(key)
    local new_direction = key_to_direction[key]
    if new_direction then
        mySnake:setDirection(new_direction)
    end
    if key == "R" then
        game_lost = false
        createSnake()
        createApple()
    end
end
