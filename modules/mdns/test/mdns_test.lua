--[[------------------------------------------------------

  test mDNS for Lua
  -----------------

  mDNS implementation with callbacks to integrate with
  rubyk.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mdns')
local timeout = 5000

function should.register()
  local continue = false
  -- register our service at port 12345
  local registration = mdns.Registration(rubyk.service_type, 'Service1', 12345, function()
    continue = true
  end)
  local now = worker:now()
  while not continue and worker:now() < now + timeout do
    sleep(10)
  end
  assert_true(continue)
end

function should.browse()
  local continue    = false
  local should_op   = 'add'
  local hostname    = nil
  local device_list = {}
  -- register a service at port 12345
  local registration = mdns.Registration(rubyk.service_type, 'Service for browse', 12346, function(service)
    hostname = service.host
    continue = true
  end)
  
  -- wait (and give time for callback to enter Lua State)
  while not continue do
    sleep(10)
  end
  continue = false

  local browser = mdns.Browser(rubyk.service_type, function(service)
    if service.op == should_op and service.name == 'Service for browse' then
      continue = true
    end
  end)
  -- wait (and give time for callback to enter Lua State)
  local now = worker:now()
  while not continue and worker:now() < now + timeout do
    sleep(10)
  end
  assert_true(continue)

  continue   = false
  should_op  = 'remove'
  registration = nil  -- delete
  collectgarbage('collect')

  -- wait (and give time for callback to enter Lua State)
  local now = worker:now()
  while not continue and worker:now() < now + timeout do
    sleep(10)
  end
  assert_true(continue)
end

test.all()