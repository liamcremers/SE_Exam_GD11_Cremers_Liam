local game_lost = false

function initializeWindow()
    GAME_ENGINE:SetTitle("Snake")
    GAME_ENGINE:SetWidth(640)
    GAME_ENGINE:SetHeight(560)
    GAME_ENGINE:SetFrameRate(60)
end

function initialize()
    initializeWindow()
end

function paint()
    GAME_ENGINE:FillWindowRect(RGB(0,0,0))

    GAME_ENGINE:SetColor(RGB(255, 255, 255))
    GAME_ENGINE:DrawString("test", 10, 10)
end