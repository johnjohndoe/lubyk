--[[------------------------------------------------------

  Basic tests for lubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.Painter')

function should.load_constants()
  assertEqual(1, mimas.SolidPattern)
end

function should.paintPath(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  t.win:move(300, 300)
  t.win:resize(100, 100)
  function t.win.paint(p, w, h)
    local box_padding = 5
    local path = mimas.Path()
    local hpen_width = 5 -- half pen width
    local bp = hpen_width + box_padding -- full box padding
    local arc_radius = 30
    local text_padding = 20
    path:moveTo(w, bp)
    path:lineTo(bp + arc_radius, bp)
    path:cubicTo(bp, bp, bp, bp, bp, bp + arc_radius)
    path:lineTo(bp, h - bp - arc_radius)
    path:cubicTo(bp, h - bp, bp, h - bp, bp + arc_radius, h - bp)
    path:lineTo(w, h - bp)
    p:setBrush(mimas.Color(0.5, 0.5, 0.5))
    p:setPen(mimas.Pen(hpen_width * 2, mimas.Color(0.2, 0.8, 0.8, 0.8)))
    p:drawPath(path)
    p:drawText(text_padding, text_padding, w - 2*text_padding, h - 2*text_padding, mimas.AlignCenter, 'Saturn')
  end
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(400)
    t.win:close()
    assertTrue(true)
  end)
end

function should.draw_rounded_rect(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  t.win:move(300, 450)
  t.win:resize(100, 100)
  function t.win.paint(p, w, h)
    local box_padding = 5
    local hpen_width = 2 -- half pen width
    local bp = hpen_width + box_padding -- full box padding
    local arc_radius = 15
    local text_padding = 20
    local pen = mimas.Pen(hpen_width * 2, mimas.Color(0.2, 0.8, 0.8, 0.8))
    pen:setStyle(mimas.DashLine)
    p:setBrush(mimas.Color(0.5, 0.5, 0.5))
    p:setPen(pen)
    p:drawRoundedRect(bp, bp, w + arc_radius + bp, h - 2 * bp, arc_radius)
    p:drawText(text_padding, text_padding, w - 2*text_padding, h - 2*text_padding, mimas.AlignCenter, 'Saturn')
  end
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(400)
    t.win:close()
    assertTrue(true)
  end)
end

function should.drawRect(t)
  -- we use the test env to protect from gc
  t.win = mimas.Window()
  t.win:move(300, 600)
  t.win:resize(100, 100)
  function t.win.paint(p, w, h)
    local box_padding = 5
    local hpen_width = 5 -- half pen width
    local bp = hpen_width + box_padding -- full box padding
    local arc_radius = 15
    local text_padding = 20
    local pen = mimas.Pen(hpen_width * 2, mimas.Color(0.1, 0.8, 0.8, 0.8))
    pen:setJoinStyle(mimas.RoundJoin)
    p:setBrush(mimas.Color(0.5, 0.5, 0.5))
    p:setPen(pen)
    p:drawRect(bp, bp, w - 2*bp, h - 2*bp)
    p:drawText(text_padding, text_padding, w - 2*text_padding, h - 2*text_padding, mimas.AlignCenter, 'Saturn')
  end
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(400)
    t.win:close()
    assertTrue(true)
  end)
end

function should.accept_destroy_from_gui(t)
  t.win = mimas.Window()
  t.win:move(100, 170)
  t.win:resize(50, 50)
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(200)
    t.win:close()
    while not t.win:deleted() do
      sleep(200)
    end
    -- should be deleted by GUI
    assertMatch('NULL', t.win:__tostring())
  end)
end

function should.accept_destroy_from_Lua()
  local win = mimas.Window()
  win:move(100, 240)
  win:resize(50, 50)
  win:show()
  local label = mimas.Label("Hop", win)

  thread = lk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    -- proof that win was deleted in C++
    assertTrue(label:deleted())
  end)
end

function should.drawPathOutline(t)
  t.win = mimas.Window()
  t.win:move(10, 10)
  t.win:resize(200, 200)
  t.win:show()
  t.path = mimas.Path()
  t.path:moveTo(50, 50)
  t.path:cubicTo(
    50,  50  + 35,
    150, 150 - 35,
    150, 150
  )
  t.outline = t.path:outlineForWidth(20)
  function t.win.paint(p, w, h)
    p:setBrush(mimas.colors.Red:colorWithAlpha(0.2))
    p:setPen(1, mimas.colors.Blue)
    p:drawPath(t.outline)
    p:setPen(1, mimas.colors.Yellow)
    p:setBrush(mimas.NoBrush)
    p:drawPath(t.path)
  end

  function t.win.click(x, y)
    if t.outline:contains(x, y) then
      t.win:close()
    end
  end

  t.win:update()
  t.thread = lk.Thread(function()
    sleep(2000)
    t.win:close()
    assertTrue(true)
  end)
end

test.all()