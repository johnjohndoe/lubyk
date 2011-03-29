--[[------------------------------------------------------

  lk.Inlet test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Inlet')

local function mockNode()
  return {inlets = {}, pending_inlets = {}}
end

function should.createInlet()
  local inlet = lk.Inlet(mockNode(), 'tempo', 'Set tempo [bpm].')
  assertEqual('tempo', inlet.name)
  assertEqual('Set tempo [bpm].', inlet.info)
end

function should.createPendingInlet()
  local inlet = lk.Inlet('tempo')
  assertEqual('tempo', inlet.name)
  assertEqual(nil, inlet.node)
end

function should.usePendingInletOnCreate()
  local node = mockNode()
  -- pending inlet
  local inlet = lk.Inlet('tempo')
  node.pending_inlets = {tempo=inlet}

  assertEqual(inlet, lk.Inlet(node, 'tempo', 'Set tempo [bpm].'))
  assertEqual('Set tempo [bpm].', inlet.info)
end

function should.addInletToNodeOnce()
  local node  = mockNode()
  local inlet = lk.Inlet(node, 'tempo', 'Set tempo [bpm].')
  local inlet2= lk.Inlet(node, 'tempo', 'Set tempo [bpm].')
  assertEqual(inlet, inlet2)
  assertEqual(inlet, node.inlets.tempo)
end

test.all()