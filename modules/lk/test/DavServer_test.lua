--[[------------------------------------------------------

  lk.DavServer test
  -----------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('lk.DavServer')
local crypto = lk.crypto

function should.autoLoad()
  assertTrue(lk.DavServer)
end

test.all()

srv = lk.DavServer(1024)
srv.res = {}
print(srv.port)

local function mockResource(path)
  local self = {davProperty = rawget, path = path}
  if path == '/' then
    self.resourcetype = {xml = 'collection'}
    self.contentlength = 100 -- ???
    self.getlastmodified = 9298347
    self.creationdate    = 8909234
    return self
  elseif path == '/foo.lua' then
    self.body = [[
-- This is a test script "Foo.lua"
require 'lubyk'
print('ok!')
]]
  elseif path == '/bar.lua' then
    self.body = [[
-- This is a test script "Foo.lua"
require 'lubyk'
print('ok!')
]]
  end
  -- common to foo.lua and bar.lua
  self.getcontentlength = string.len(self.body)
  self.contenttype = 'text/plain'
  self.getlastmodified = 9298447
  self.creationdate    = 8909334
  return self
end

function srv:find(path)
  print(string.format("srv:resource '%s'", path))
  -- mock a resource
  if path == '/' or path == '/foo.lua' or path == '/bar.lua' then
    local res = self.res[path]
    if not res then
      res = mockResource(path)
      self.res[path] = res
    end
    return res
  end
end

function srv:findChildren(resource)
  if resource.path == '/' then
    if not resource.children then
      resource.children = {
        self:find('/foo.lua'),
        self:find('/bar.lua'),
      }
      setmetatable(resource.children, {__mode='v'})
    end
    return resource.children
  end
end

function srv:update(resource, content)
  -- forbidden
  resource.body = content
  resource.getcontentlength = string.len(resource.body)
  return nil, {status = "200"}
end

srv:listen()


