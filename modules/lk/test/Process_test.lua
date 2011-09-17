--[[------------------------------------------------------

  lk.Process test
  ---------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Process')
local timeout = 3000

function should.loadCode()
  assertTrue(lk.Process)
end

function should_createPatch()
  local process = lk.Process(fixture.path('simple.yml'))
  assertType('table', process.nodes.add)
  assertType('table', process.nodes.store)
end

function should.announceService(t)
  local process = lk.Process('simple')
  local continue = false
  local browser = mdns.Browser(lubyk.service_type, function(self, service)
    if service.op == 'add' and service.name == Lubyk.zone .. ':simple' then
      continue = true
    end
  end)
  -- wait (and give time for callback to enter Lua State)
  t:timeout(3000, function(done)
    if continue or done then
      assertTrue(continue)
      return true
    end
  end)
end

test.all()
