--[[------------------------------------------------------

  mdns.Service
  ------------

  This class is returned from mdns.Browser.

--]]------------------------------------------------------
require 'mdns.Service_core'
local lib = mdns.Service_
local constr = mdns.Service
local private = {}

-- Disable constructor from Lua for now.
mdns.Service = nil