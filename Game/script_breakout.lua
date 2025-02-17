-- TODO: display amount of lives
local game_won = false
local game_over = false
local lives = 3
local score = 0
local paddleSound = Audio.new("paddle.mp3")
local wallSound = Audio.new("breakBlock.mp3")
local breakBlockSound = Audio.new("breakBlock.mp3")

local WINDOW_WIDTH = 640
local WINDOW_HEIGHT = 560
local WINDOW_FPS = 240
local WINDOW_FONT = Font.new("Press Start 2P", false, false, false, 64)
local LIVES_FONT = Font.new("Press Start 2P", false, false, false, 11)
local function initializeWindow()
    GAME_ENGINE:SetTitle("Breakout")
    GAME_ENGINE:SetWidth(WINDOW_WIDTH);
    GAME_ENGINE:SetHeight(WINDOW_HEIGHT);
    GAME_ENGINE:SetFrameRate(WINDOW_FPS);
    GAME_ENGINE:SetFont(WINDOW_FONT)
end

local blocks = {}
local function createBreakoutBlocks()
    local red = {255, 0, 0}
    local orange = {255, 165, 0}
    local yellow = {255, 255, 0}
    local green = {0, 255, 0}
    local blue = {0, 0, 255}
    local purple = {128, 0, 128}

    local colors = {red, orange, yellow, green, blue, purple}

    local rectWidth = 60
    local rectHeight = 30
    local margin = 4
    local halfMargin = 2

    for colorIndex, color in ipairs(colors) do
        blocks[colorIndex] = {}
        for i = 1, 10 do
            -- Start column from 1, as per your preference
            local block = {
                left = (rectWidth + margin) * (i - 1) + halfMargin, -- Column positions remain unchanged
                top = (rectHeight + margin) * (colorIndex - 1) + margin,
                color = color
            }
            blocks[colorIndex][i] = block
        end
    end
end

local function drawBreakoutBlocks()
    for _, colorRow in pairs(blocks) do
        -- Use `pairs` to iterate over all rows
        for _, block in pairs(colorRow) do
            -- Use `pairs` again for individual blocks
            GAME_ENGINE:SetColor(RGB(block.color[1], block.color[2], block.color[3]))
            GAME_ENGINE:FillRect(block.left, block.top, block.left + 60, block.top + 30)
        end
    end
end

local Ball = {} -- Properly initialize Ball as a table
Ball.__index = Ball -- Now this works because Ball is a valid table
local BALL_SPEED = 400.0 -- Pixels per second
local RADIUS = 10
local CENTER_X = WINDOW_WIDTH / 2
local CENTER_Y = WINDOW_HEIGHT * 0.75
function Ball.new(x, y, radius, color, dx, dy)
    local self = setmetatable({}, Ball)
    self.x = x
    self.y = y
    self.radius = radius
    self.color = color
    self.dx = dx or 0
    self.dy = dy or 0
    return self
end

local function createBall()
    ball = Ball.new(CENTER_X, CENTER_Y, RADIUS, {255, 255, 255}, -- White color
    math.random(2) == 1 and -1 or 1, -- Random x direction
    -1)
end

function Ball:move(elapsed_time)
    -- Calculate distance based on speed and delta time
    local distance_x = self.dx * BALL_SPEED * elapsed_time
    local distance_y = self.dy * BALL_SPEED * elapsed_time

    -- Update position
    self.x = self.x + distance_x
    self.y = self.y + distance_y

    -- Bounce off the walls
    if self.x - self.radius < 0 or self.x + self.radius > WINDOW_WIDTH then
        self.dx = -self.dx
        self.x = math.max(self.radius, math.min(self.x, WINDOW_WIDTH - self.radius))
        wallSound:Play()
    end

    if self.y - self.radius < 0 then
        self.dy = -self.dy
        self.y = math.max(self.radius, math.min(self.y, WINDOW_HEIGHT - self.radius))
        wallSound:Play()
    elseif self.y + self.radius > WINDOW_HEIGHT then
        loseLife()
    end
end

function Ball:draw()
    GAME_ENGINE:SetColor(RGB(self.color[1], self.color[2], self.color[3]))
    left = math.floor(self.x - self.radius)
    top = math.floor(self.y - self.radius)
    right = math.floor(self.x + self.radius)
    bottom = math.floor(self.y + self.radius)
    GAME_ENGINE:FillOval(left, top, right, bottom)
end

function Ball:check_collision_with_blocks()
    for _, row in pairs(blocks) do
        for colIndex, block in pairs(row) do
            -- Check if the ball intersects the block
            if self.x + self.radius > block.left and self.x - self.radius < block.left + 60 and self.y + self.radius >
                block.top and self.y - self.radius < block.top + 30 then
                -- Reflect the ball's velocity
                if self.x > block.left and self.x < block.left + 60 then
                    -- Collision on the top or bottom of the block
                    self.dy = -self.dy
                else
                    -- Collision on the left or right of the block
                    self.dx = -self.dx
                end

                -- Remove the block by setting it to `nil`
                row[colIndex] = nil
                breakBlockSound:Play()
                if checkVictory() then
                    game_won = true
                end
                score = score + math.abs(_-6)+1

                -- Break out of the loops since the ball can only hit one block at a time
                return
            end
        end
    end
end

local PADDLE_WIDTH = 80
local PADDLE_HEIGHT = 20
local PADDLE_SPEED = 1000.0 -- Pixels per second
local Paddle = {}
local VK_LEFT = 0x25
local VK_RIGHT = 0x27
Paddle.__index = Paddle
function Paddle.new(x, y, width, height, color)
    local self = setmetatable({}, Paddle)
    self.x = x
    self.y = y
    self.width = width
    self.height = height
    self.color = color
    self.dx = 0 -- Paddle movement speed (0 = stationary)
    return self
end

local function createPaddle()
    paddle =
        Paddle.new(CENTER_X - PADDLE_WIDTH / 2, WINDOW_HEIGHT - (PADDLE_HEIGHT * 2), -- Position the paddle near the bottom
            PADDLE_WIDTH, PADDLE_HEIGHT, {255, 255, 255} -- White color
        )
end

function Paddle:move(elapsed_time)
    self.x = self.x + self.dx * PADDLE_SPEED * elapsed_time
    self.x = math.max(0, math.min(self.x, WINDOW_WIDTH - self.width))
end

function Paddle:draw()
    left = math.floor(self.x)
    top = math.floor(self.y)
    right = math.floor(self.x + self.width)
    bottom = math.floor(self.y + self.height)

    GAME_ENGINE:SetColor(RGB(self.color[1], self.color[2], self.color[3]))
    GAME_ENGINE:FillRect(left, top, right, bottom)
end

function Paddle:handle_input()
    if GAME_ENGINE:IsKeyDown(VK_LEFT) then
        self.dx = -1
    elseif GAME_ENGINE:IsKeyDown(VK_RIGHT) then
        self.dx = 1
    else
        self.dx = 0
    end
end

function Paddle:check_collision_with_ball(ball)
    if ball.x + ball.radius > self.x and ball.x - ball.radius < self.x + self.width and ball.y + ball.radius > self.y and
        ball.y - ball.radius < self.y + self.height then
        ball.dy = -ball.dy

        local hit_position = (ball.x - self.x) / self.width
        ball.dx = (hit_position - 0.5) * 2 -- Normalize to range [-1, 1]

        paddleSound:Play()
    end
end

local function AddKeyListKeys()
    keyList = "R"
    GAME_ENGINE:SetKeyList(keyList)
end

local function displayVictoryMessage()
    GAME_ENGINE:SetFont(WINDOW_FONT)
    GAME_ENGINE:SetColor(RGB(255, 255, 255))
    message = "You Won!"
    Dimensions = GAME_ENGINE:CalculateTextDimensions(message, WINDOW_FONT)
    textWidth = Dimensions.cx
    textHeight = Dimensions.cy
    x = math.floor((WINDOW_WIDTH / 2) - textWidth / 2)
    y = math.floor((WINDOW_HEIGHT / 2) - textHeight / 2)
    GAME_ENGINE:DrawString(message, x, y)
end

local function displayGameOverMessage()
    GAME_ENGINE:SetFont(WINDOW_FONT)
    GAME_ENGINE:SetColor(RGB(255, 255, 255))
    message = "GAME OVER!!!!"
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
    y = math.floor((WINDOW_HEIGHT / 2) - textHeight / 2) + textHeight
    GAME_ENGINE:DrawString(message, x, y)
end

function checkVictory()
    for _, row in pairs(blocks) do
        for _, block in pairs(row) do
            if block ~= nil then
                return false -- Found an existing block, not a victory yet
            end
        end
    end
    return true -- No blocks found, player has won
end

local function resetGame()
    createBall()
    createPaddle()
end

local function hardResetGame()
    game_over = false
    game_won = false
    lives = 3
    score = 0
    createBall()
    createPaddle()
    createBreakoutBlocks()
end

function gameOver()
    game_over = true
end

function loseLife()
    lives = lives - 1
    if lives <= 0 then
        gameOver()
    else
        resetGame()
    end
end

-- ____________________________________________________________________________________________________________________

function initialize()
    initializeWindow()
    AddKeyListKeys()
end

function start()
    createBreakoutBlocks()
    createBall()
    createPaddle()
end

function paint()
    GAME_ENGINE:FillWindowRect(RGB(0, 0, 0))
    if game_won then
        displayVictoryMessage()
        return
    elseif game_over then
        displayGameOverMessage()
        return
    else
        drawBreakoutBlocks()
        ball:draw()
        paddle:draw()

        GAME_ENGINE:SetFont(LIVES_FONT)
        GAME_ENGINE:DrawString("lives: " .. lives, 10, 10)
        GAME_ENGINE:DrawString("score: " .. score, 10, 20)
    end
end

function soundsTick()
    paddleSound:Tick()
    wallSound:Tick()
    breakBlockSound:Tick()
end

function tick()
    if game_won then
        return
    end

    local elapsed_time = GAME_ENGINE:GetFrameDelay() * 0.001

    paddle:handle_input()
    paddle:move(elapsed_time)
    paddle:check_collision_with_ball(ball)

    ball:move(elapsed_time)
    ball:check_collision_with_blocks()

    soundsTick()
end

function key_pressed(key)
    if key == string.byte("R") then
        hardResetGame()
    end
    if key == "R" then
        hardResetGame()
    end
end
