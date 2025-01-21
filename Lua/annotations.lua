---@meta

---@class RGB
---@param r number
---@param g number
---@param b number
---@return number
function RGB(r, g, b) end

---@class Font
Font = {}
---@param name string
---@param bold boolean
---@param italic boolean
---@param underline boolean
---@param size number
---@return Font
function Font.new(name, bold, italic, underline, size) end

---@class RECT
---@field left number
---@field top number
---@field right number
---@field bottom number
RECT = {}
---@param left number
---@param top number
---@param right number
---@param bottom number
---@return RECT
function RECT.new(left, top, right, bottom) end

--- @class POINT
--- @field x number The x-coordinate of the point (LONG).
--- @field y number The y-coordinate of the point (LONG).
POINT = {}
--- @overload fun(): POINT
--- @param x? number The x-coordinate (LONG). Defaults to 0 if not provided.
--- @param y? number The y-coordinate (LONG). Defaults to 0 if not provided.
--- @return POINT A new POINT object with the specified x and y.
function POINT.new(x, y)
    x = x or 0
    y = y or 0

    local self = setmetatable({}, { __index = POINT })
    self.x = x
    self.y = y
    return self
end

---@class SIZE
---@field cx number The width of the rectangle (LONG).
---@field cy number The height of the rectangle (LONG).
SIZE = {}
---@param cx? number The width of the rectangle (LONG).
---@param cy? number The height of the rectangle (LONG).
---@return SIZE A new SIZE object with the specified width and height.
function SIZE.new(cx, cy)
    cx = cx or 0
    cy = cy or 0

    local self = setmetatable({}, { __index = SIZE })
    self.cx = cx
    self.cy = cy
    return self
end

-- Define the Bitmap type
---@class Bitmap
---@field filename string
local Bitmap = {}

--______________________________________________________________________________________________________________________
---@class GameEngine
GAME_ENGINE = {}

---@param title string
function GAME_ENGINE:SetTitle(title) end

---@param left number
---@param top number
function GAME_ENGINE:SetWindowPosition(left, top) end

---@param region any
---@return boolean
function GAME_ENGINE:SetWindowRegion(region) end

---@param keyList string
function GAME_ENGINE:SetKeyList(keyList) end

---@param frameRate number
function GAME_ENGINE:SetFrameRate(frameRate) end

---@param width number
function GAME_ENGINE:SetWidth(width) end

---@param height number
function GAME_ENGINE:SetHeight(height) end

---@return boolean
function GAME_ENGINE:GoFullscreen() end

---@return boolean
function GAME_ENGINE:GoWindowedMode() end

---@param show boolean
function GAME_ENGINE:ShowMousePointer(show) end

function GAME_ENGINE:Quit() end

---@return boolean
function GAME_ENGINE:HasWindowRegion() end

---@return boolean
function GAME_ENGINE:IsFullscreen() end

---@param key number
---@return boolean
function GAME_ENGINE:IsKeyDown(key) end

---@overload fun(message: string)
---@overload fun(message: number)
function GAME_ENGINE:MessageBox(message) end

---@param message string
---@return boolean
function GAME_ENGINE:MessageContinue(message) end

---@overload fun(text: string, font: Font, rect:RECT): SIZE
---@return SIZE
function GAME_ENGINE:CalculateTextDimensions(text, font) end

---@param color RGB
function GAME_ENGINE:SetColor(color) end

---@param font Font
function GAME_ENGINE:SetFont(font) end

---@param color RGB
---@return boolean
function GAME_ENGINE:FillWindowRect(color) end

---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@return boolean
function GAME_ENGINE:DrawLine(x1, y1, x2, y2) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@return boolean
function GAME_ENGINE:DrawRect(left, top, right, bottom) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@overload fun(left: number, top: number, right: number, bottom: number, opacity: number): boolean
function GAME_ENGINE:FillRect(left, top, right, bottom) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param radius number
---@return boolean
function GAME_ENGINE:DrawRoundRect(left, top, right, bottom, radius) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param radius number
---@return boolean
function GAME_ENGINE:FillRoundRect(left, top, right, bottom, radius) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@return boolean
function GAME_ENGINE:DrawOval(left, top, right, bottom) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param opacity number
---@overload fun(left: number, top: number, right: number, bottom: number, opacity: number): boolean
function GAME_ENGINE:FillOval(left, top, right, bottom) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param startDegree number
---@param angle number
---@return boolean
function GAME_ENGINE:DrawArc(left, top, right, bottom, startDegree, angle) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param startDegree number
---@param angle number
---@return boolean
function GAME_ENGINE:FillArc(left, top, right, bottom, startDegree, angle) end

---@overload fun(text: string, left: number, top: number, right: number, bottom: number): number
function GAME_ENGINE:DrawString(text, left, top) end

---@overload fun(bitmap: Bitmap, left: number, top: number, rect: RECT): boolean
function GAME_ENGINE:DrawBitmap(bitmap, left, top) end

---@overload fun(points: POINT[], count: number, close: boolean): boolean
function GAME_ENGINE:DrawPolygon(points, count) end

---@overload fun(points: POINT[], count: number, close: boolean): boolean
function GAME_ENGINE:FillPolygon(points, count) end

---@return RGB
function GAME_ENGINE:GetDrawColor() end

---@return boolean
function GAME_ENGINE:Repaint() end

---@return string
function GAME_ENGINE:GetTitle() end

---@return number
function GAME_ENGINE:GetWidth() end

---@return number
function GAME_ENGINE:GetHeight() end

---@return number
function GAME_ENGINE:GetFrameRate() end

---@return number
function GAME_ENGINE:GetFrameDelay() end

---@return POINT
function GAME_ENGINE:GetWindowPosition() end

---TODO: key

---@class Audio
Audio = {}

--- Constructs a new Audio object with the given filename.
---@param filename string The name of the audio file to load.
---@return Audio A new Audio object.
function Audio.new(filename) end

--- Updates the audio each frame (typically used for handling updates like playback state).
function Audio:Tick() end

--- Plays the audio.
---@return boolean True if the audio started playing successfully.
function Audio:Play() end

--- Pauses the audio if it is currently playing.
---@return boolean True if the audio was paused successfully.
function Audio:Pause() end

--- Stops the audio and resets the playback position.
---@return boolean True if the audio was stopped successfully.
function Audio:Stop() end

--- Sets the volume of the audio.
---@param volume number The volume level to set (0.0 to 1.0).
---@return boolean True if the volume was set successfully.
function Audio:SetVolume(volume) end

--- Sets whether the audio should repeat when finished.
---@param isRepeat boolean If `true`, the audio will repeat.
---@return boolean True if the repeat setting was updated successfully.
function Audio:SetRepeat(isRepeat) end

--- Gets the name of the audio file.
---@return string The name of the audio file.
function Audio:GetName() end

--- Gets the alias of the audio.
---@return string The alias of the audio.
function Audio:GetAlias() end

--- Gets the total duration of the audio in seconds.
---@return number The duration of the audio in seconds.
function Audio:GetDuration() end

--- Checks if the audio is currently playing.
---@return boolean True if the audio is playing.
function Audio:IsPlaying() end

--- Checks if the audio is currently paused.
---@return boolean True if the audio is paused.
function Audio:IsPaused() end

--- Gets the repeat setting of the audio (whether it will loop).
---@return boolean True if the audio is set to repeat.
function Audio:GetRepeat() end

--- Checks if the audio file exists and is valid.
---@return boolean True if the audio file exists and is valid.
function Audio:Exists() end

--- Gets the current volume of the audio.
---@return number The current volume level of the audio (0.0 to 1.0).
function Audio:GetVolume() end

--- Gets the type of the audio (e.g., "mp3", "wav").
---@return string The type/format of the audio file.
function Audio:GetType() end

---@enum HitRegionShape
---@field Ellipse number
---@field Rectangle number
HitRegionShape = {
    Ellipse = 0,
    Rectangle = 1
}

---@class HitRegion
HitRegion = {}

---@param shape HitRegionShape
---@param left number
---@param top number
---@param right number
---@param bottom number
---@return HitRegion
function HitRegion.new(shape, left, top, right, bottom) end

---@param dx number
---@param dy number
function HitRegion:Move(dx, dy) end

---@overload fun(x: number, y: number): boolean
---@overload fun(region: HitRegion): boolean
function HitRegion:HitTest(...) end

---@param other HitRegion
---@return boolean
function HitRegion:CollisionTest(other) end

---@return RECT
function HitRegion:GetBounds()
    return RECT.new(self.left, self.top, self.right, self.bottom)
end

---@return boolean
function HitRegion:Exists() end

---@return any
function HitRegion:GetHandle() end
    
-- #################################################################
-- # Game Loop Functions
-- #################################################################
function initialize() end
function start() end
function endGame() end
---@param rect RECT
function paint(rect) end
function tick() end
---@param isLeft boolean
---@param isDown boolean
---@param x number
---@param y number
function mouseButtonAction(isLeft, isDown, x, y, wParam) end
---@param x number
---@param y number
---@param distance number
---@param wParam number
function mouseWheelAction(x, y, distance, wParam) end
---@param x number
---@param y number
---@param wParam number
function mouseMove(x, y, wParam) end
function checkKeyboard() end
---@param key string
function keyPressed(key) end
---@param callerPtr any
function callAction(callerPtr) end