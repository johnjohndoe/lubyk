--[[------------------------------------------------------

  editor.Process
  --------------

  The editor.Process manages the connection with the
  remote Process and holds information on the state of the
  remote. This is like lk.Client but with a single
  subscription.

--]]------------------------------------------------------

local lib      = {type='editor.Process'}
lib.__index    = lib
editor.Process = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, name)
  local self = {
    name           = name,
    x              = 100,
    y              = 100,
    nodes          = {},
    pending_inlets = {},
  }

  setmetatable(self, lib)
  return self
end})

--- Nodes in patch changed, store change and update view if needed.
-- To remove a node, set it's value to false.
local function setNodes(self, nodes_def)
  local nodes = self.nodes
  for node_name, node_def in pairs(nodes_def) do
    local node = nodes[node_name]
    if node then
      if node_def then
        -- set existing node
        node:set(node_def)
      else
        -- remove node
        node:remove()
        nodes[node_name] = nil
      end
    elseif node_def then
      -- create new node
      node = editor.Node(self, node_name, node_def)
      nodes[node_name] = node
    end
  end
end

local function doSet(self, definition)
  for k, v in pairs(definition) do
    if k == 'name' then
      self.name = v
      if self.view then
        self.view:setName(v)
      end
    elseif k == 'nodes' then
      setNodes(self, v)
    else
      self[k] = v
    end
  end
end

-- Synchronize with remote process.
function lib:sync()
  local definition = self.req:request(lubyk.dump_url)
  self:set(definition)
end

--- Change remote content.
function lib:change(definition)
  self.push:send(lubyk.update_url, definition)
end

-- If self.view is nil, only set the data without
-- creating/changing views.
function lib:set(definition)
  if self.view then
    app:post(function()
      doSet(self, definition)
      self:updateView()
      self.view:processChanged()
    end)
  else
    doSet(self, definition)
  end
end

-- When the ProcessView is created, it triggers this method
-- to build/update views
function lib:updateView()
  self:redrawViews()

  self.view:processChanged()
end

--- Used when the process is dragged
function lib:redrawViews()
  for _,node in pairs(self.nodes) do
    node:updateView()
  end

  for _,node in pairs(self.nodes) do
    for _,outlet in ipairs(node.outlets) do
      for _,link in pairs(outlet.links) do
        link:updateView()
      end
    end
  end
end

--- Find a node from a given url (same as lk.Patch.get).
lib.get = lk.Patch.get

--- Find a node from a path (same as lk.Patch.findByPath).
lib.findByPath = lk.Patch.findByPath

--- Find a process by name.
-- FIXME: write our own... Why ?
lib.findProcess = lk.Patch.findProcess

-- Create a pending inlet from an url relative to this process (nearly the same
-- as lk.Patch.pendingInlet).
function lib:pendingInlet(inlet_url)
  print('pendingInlet', inlet_url)
  -- inlet_url example:
  --   node/in/slot
  local parts = lk.split(inlet_url, '/')
  local node_name, inlet_name
  if #parts == 3 and parts[2] == 'in' then
    node_name, inlet_name = parts[1], parts[3]
  else
    return nil, string.format("Invalid pendingInlet url '%s'.", inlet_url)
  end

  local node = self.nodes[node_name]
  local inlet = nil
  if node then
    -- Node exists but inlet is not created yet.
    inlet = editor.Inlet(node.pending_inlets, inlet_name)
    node.pending_inlets[inlet_name] = inlet
  else
    -- node not created yet
    local pending_inlets = self.pending_inlets[node_name]
    if not pending_inlets then
      pending_inlets = {}
      self.pending_inlets[node_name] = pending_inlets
    end
    inlet = pending_inlets[inlet_name]
    if not inlet then
      inlet = editor.Inlet(pending_inlets, inlet_name)
      pending_inlets[inlet_name] = inlet
    end
  end
  return inlet
end

--- Display ProcessView in the PatchingView.
-- Should be called from withing GUI thread.
function lib:toggleView()
  self.delegate:toggleView(self)
end

function lib:deleteView()
  -- TODO: why don't we use Qt signals and slots ?
  for k, node in pairs(self.nodes) do
    node:deleteView()
  end

  if self.view then
    self.view:delete()
    self.view = nil
  end
end

--- Make a new node in the remote process with the given
-- definition.
function lib:newNode(definition)
  if not definition.class then
    definition.code = definition.code or [=[
--[[
inlet('input', 'Information on input [type].')
output = outlet('output', 'Information on output [type].')

function inlet.input(val)
  -- print and pass through
  print(val)
  output(val)
end
--]]


]=]
  end
  self:change {nodes = {[definition.name or 'new node'] = definition}}
end

-- Process is coming online.
function lib:connect(remote_service, delegate, process_watch)
  -- The delegate is used by views.
  self.delegate       = delegate
  -- This is used to resolve links.
  self.process_watch  = process_watch
  self.push           = remote_service.push
  self.req            = remote_service.req

  self.hue = remote_service.info.hue or 0.5

  --======================================= SUB client
  self.sub = zmq.SimpleSub(function(changes)
    -- we receive notifications, update content
    self:set(changes)
  end)
  self.sub:connect(remote_service.sub_url)

  self:sync()
end

-- A process is going offline, disconnect all links to this dying
-- process and mark them as pending.
function lib:disconnectProcess(process_name)
  -- Disconnect all linked inlets
  for _, node in pairs(self.nodes) do
    node:disconnectProcess(process_name)
  end
end

-- The remote process is dying.
function lib:disconnect()
  -- disconnect self.sub ?
end

function lib:url()
  return '/' .. self.name
end