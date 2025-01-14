function initializeWindow()
    SetTitle(_T("Breakout Game"));
    SetWidth(640);     
    SetHeight(480);    
    SetFrameRate(60);  
    -- print("Window initialized: 640x480 @ 60 FPS")
end

local blocks = {}

function createBreakoutBlocks()
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
    local halfmargin = 2

    -- print("Creating breakout blocks...")

    for colorIndex, color in ipairs(colors) do
        blocks[colorIndex] = {}
        for i = 1, 10 do  -- Start column from 1, as per your preference
            local block = {
                left = (rectWidth + margin) * (i - 1) + halfmargin,  -- Column positions remain unchanged
                top = (rectHeight + margin) * (colorIndex - 1) + margin,
                color = color
            }
            blocks[colorIndex][i] = block
            -- print(string.format("Block created: Row %d, Col %d, Color (%d, %d, %d)", colorIndex, i, color[1], color[2], color[3]))
        end
    end
end


function drawColoredRectangles()
    -- print("Drawing blocks...")
    for colorIndex, colorRow in ipairs(blocks) do
        for colIndex, block in pairs(colorRow) do -- Use pairs to include all keys
            -- print(string.format("Drawing block: Row %d, Col %d at (%d, %d)", colorIndex, colIndex, block.left, block.top))

            -- Set the color and draw the rectangle
            SetColor(block.color[1], block.color[2], block.color[3])
            FillRect(block.left, block.top, block.left + 60, block.top + 30)
        end
    end
end

function initialize()
    initializeWindow()
end

function start()
	createBreakoutBlocks()  
end

function paint()
    drawColoredRectangles()
end
