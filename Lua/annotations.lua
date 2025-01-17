---@meta

-- Define the POINT type
---@class POINT
---@field x number
---@field y number
local POINT = {}

-- Define the RECT type
---@class RECT
---@field left number
---@field top number
---@field right number
---@field bottom number
local RECT = {}

-- Define the Bitmap type
---@class Bitmap
---@field filename string
local Bitmap = {}

-- Define the SIZE type
---@class SIZE
---@field width number
---@field height number
local SIZE = {}

-- Define the Font type
---@class Font
---@field name string
---@field bold boolean
---@field italic boolean
---@field underline boolean
---@field size number
local Font = {}

---@class GameEngine
GAME_ENGINE = {}

---@field SetWindowPosition fun(x: number, y: number):void
---@field SetWindowRegion fun(region: any):void
---@field SetKeyList fun(keyList: any):void

---@param title string
function GAME_ENGINE:SetTitle(title) end

---@param x number
---@param y number
function GAME_ENGINE:SetWindowPosition(x, y) end

---@param region any
function GAME_ENGINE:SetWindowRegion(region) end

---@param keyList any
function GAME_ENGINE:SetKeyList(keyList) end

---@param frameRate number
function GAME_ENGINE:SetFrameRate(frameRate) end

---@param width number
function GAME_ENGINE:SetWidth(width) end

---@param height number
function GAME_ENGINE:SetHeight(height) end

function GAME_ENGINE:GoFullscreen() end

function GAME_ENGINE:GoWindowedMode() end

---@param show boolean
function GAME_ENGINE:ShowMousePointer(show) end

function GAME_ENGINE:Quit() end

---@return boolean
function GAME_ENGINE:HasWindowRegion() end

---@return boolean
function GAME_ENGINE:IsFullscreen() end

---@param key any
---@return boolean
function GAME_ENGINE:IsKeyDown(key) end

---@overload fun(message: string)
---@overload fun(message: string)
---@overload fun(message: number)
---@overload fun(message: number)
function GAME_ENGINE:MessageBox(message) end

function GAME_ENGINE:MessageContinue() end

---@overload fun(text: string, font: Font): SIZE
---@overload fun(text: string, font: Font, rect: RECT): SIZE
function GAME_ENGINE:CalculateTextDimensions(text, font, rect) end

---@param color any
function GAME_ENGINE:SetColor(color) end

---@param font Font
function GAME_ENGINE:SetFont(font) end

---@param rect any
function GAME_ENGINE:FillWindowRect(rect) end

---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
function GAME_ENGINE:DrawLine(x1, y1, x2, y2) end

---@param left number
---@param top number
---@param right number
---@param bottom number
function GAME_ENGINE:DrawRect(left, top, right, bottom) end

---@overload fun(left: number, top: number, right: number, bottom: number): boolean
---@overload fun(left: number, top: number, right: number, bottom: number, opacity: number): boolean
function GAME_ENGINE:FillRect(left, top, right, bottom, opacity) end

---@param left number
---@param top number
---@param right number
---@param bottom number
function GAME_ENGINE:DrawOval(left, top, right, bottom) end

---@overload fun(left: number, top: number, right: number, bottom: number): boolean
---@overload fun(left: number, top: number, right: number, bottom: number, opacity: number): boolean
function GAME_ENGINE:FillOval(left, top, right, bottom, opacity) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param startDegree number
---@param angle number
function GAME_ENGINE:DrawArc(left, top, right, bottom, startDegree, angle) end

---@param left number
---@param top number
---@param right number
---@param bottom number
---@param startDegree number
---@param angle number
function GAME_ENGINE:FillArc(left, top, right, bottom, startDegree, angle) end

---@overload fun(text: string, x: number, y: number): number
---@overload fun(text: string, x: number, y: number, width: number, height: number): number
function GAME_ENGINE:DrawString(text, x, y, width, height) end

---@overload fun(bitmap: Bitmap, x: number, y: number): boolean
---@overload fun(bitmap: Bitmap, x: number, y: number, rect: RECT): boolean
function GAME_ENGINE:DrawBitmap(bitmap, x, y, rect) end

---@overload fun(points: POINT[], count: number): boolean
---@overload fun(points: POINT[], count: number, close: boolean): boolean
function GAME_ENGINE:DrawPolygon(points, count, close) end

---@overload fun(points: POINT[], count: number): boolean
---@overload fun(points: POINT[], count: number, close: boolean): boolean
function GAME_ENGINE:FillPolygon(points, count, close) end

---@return any
function GAME_ENGINE:GetDrawColor() end

function GAME_ENGINE:Repaint() end

---@return string
function GAME_ENGINE:GetTitle() end

---@return any
function GAME_ENGINE:GetInstance() end

---@return any
function GAME_ENGINE:GetWindow() end

---@return number
function GAME_ENGINE:GetWidth() end

---@return number
function GAME_ENGINE:GetHeight() end

---@return number
function GAME_ENGINE:GetFrameRate() end

---@return number
function GAME_ENGINE:GetFrameDelay() end

---@return any
function GAME_ENGINE:GetWindowPosition() end