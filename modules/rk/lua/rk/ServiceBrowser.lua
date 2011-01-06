--[[------------------------------------------------------

  rk.ServiceBrowser
  -----------------

  The ServiceBrowser listens for incoming services and
  publishes them in the application with ipc://service_type
  url.

--]]------------------------------------------------------
local lib = {}
lib.__index = lib
rk.ServiceBrowser  = lib

local browsers = {}

--- Creates an instance of the ServiceBrowser
-- Keep a hash of created browser by service_type
-- Always return the same browser for the given service_type
--
setmetatable(lib, {
  -- new method
 __call = function(table, service_type)
  local instance = browsers[service_type]
  if instance then
    return instance
  end
  instance = {service_type = service_type, pending = {}, services = {}}

  -- We must bind before any client does a subscribe.
  instance.pub = zmq.Pub(string.format('inproc://%s', service_type))

  instance.browser = mdns.Browser(service_type, function(remote_service)
    if remote_service.op == 'add' then
      if not instance.services[remote_service.name] then
        remote_service.url = string.format('tcp://%s:%i', remote_service.host, remote_service.port)
        instance.services[remote_service.name] = remote_service
        instance.pub:send(rubyk.add_service_url, remote_service)
      end
    elseif instance.services[remote_service.name] then
      instance.pub:send(rubyk.rem_service_url, remote_service)
      instance.services[remote_service.name] = nil
    end
  end)
  browsers[service_type] = instance
  setmetatable(instance, lib)
  return instance
end})
