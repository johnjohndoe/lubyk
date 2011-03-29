--[[------------------------------------------------------

  lk.TCPSocket
  ------------

  The TCP socket is a simple wrapper around a Streaming
  BSD socket.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Socket')
local counter = 0

--function should.bind_to_random_port()
--  local socket = lk.TCPSocket()
--  local port = socket:bind()
--
--  assertTrue(port < 65535)
--  assertTrue(port > 1024)
--  assertEqual(port, socket:port())
--
--  local socket2 = lk.TCPSocket()
--  local port2 = socket2:bind()
--  assertTrue(port2 < 65535)
--  assertTrue(port2 > 1024)
--  assertEqual(port2, socket2:port())
--  assert_not_equal(port, port2)
--end
--
--function should.convert_to_string()
--  assertEqual('string', type(lk.TCPSocket():__tostring()))
--end
--
--function should.create_many_sockets()
--  local sock_list = {}
--  for i = 1,100 do
--    sock_list[i] = lk.TCPSocket()
--    sock_list[i]:bind() -- creates socket fd + bind
--  end
--  -- should just work
--  assertEqual('userdata', type(sock_list[100]))
--end
--
--function should.connect()
--  local socket = lk.TCPSocket()
--  socket:connect('lubyk.org', 80)
--  assertMatch('--> lubyk.org:80', socket:__tostring())
--end
--
--function should.listen_and_accept()
--  local socket = lk.TCPSocket()
--  local port   = socket:bind()
--  local continue = false
--  socket:listen()
--  lk.Thread(function()
--    local sock2 = socket:accept()
--    continue = true
--    -- should bind to different port
--    assert_not_equal(port, sock2:remote_port())
--  end)
--
--  sleep(20) -- start thread
--
--  -- I can now connect to this remote
--  local socket2 = lk.TCPSocket()
--  socket2:connect('localhost', port)
--  while not continue do
--    sleep(20)
--  end
--end
--
function should.send_and_recv()
  local port
  local server = lk.TCPSocket(function(server)
    port = server:bind()
    server:listen()
    local acc = server:accept()
    print('accept-->', acc)
    acc:send(1, 2, "hello")
    print("send done")
    --acc:send(5)
  end)
  sleep(20) -- start server

  local client = lk.TCPSocket()
  print("connecting")
  client:connect('localhost', port)
  print('client', client)
  local received = {client:recv()}

  assertEqual({1, 2, "hello"}, received)

  received = {client:recv()}

  assertEqual({5}, received)
end

--function should.send_and_receive()
--  local port
--  local continue = false
--  local received = {}
--  local server = lk.TCPSocket(function(server)
--    port = server:bind()
--    server:listen()
--    local acc = server:accept()
--    acc:send(true) -- tell client that connection is OK
--
--    while received do
--      -- keep receiving
--      received = {acc:recv()}
--      print('<---', unpack(received))
--      continue = true
--    end
--  end)
--  sleep(20) -- start server
--
--  local client = lk.TCPSocket()
--  client:connect('localhost', port)
--  -- wait for accept
--  client:recv()
--
--  local function send_and_receive(...)
--    continue = false
--    -- cannot use ... or arg is set to nil
--    client:send(unpack(arg))
--    while not continue do
--      sleep(10)
--    end
--
--    for i, v in ipairs(arg) do
--      assertEqual(arg, received)
--    end
--  end
--
--  -- string
--  send_and_receive("Hello Lubyk!")
--  -- number, nil, bool
--  send_and_receive(1.234567)
--  send_and_receive(nil)
--  send_and_receive(true)
--  send_and_receive(false)
--  -- array
--  -- we cannot send an array like this
--  -- send_and_receive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
--  send_and_receive(1, {1, 2, 3})
--
--  send_and_receive(1, {1, 2, {4, 5}})
--
--  send_and_receive("1", {1, "zombie", {4.44, 5}})
--
--  -- hash
--  send_and_receive("/one/two", {one=2, three="four"})
--
--  -- multi values
--  send_and_receive(1,2,3)
--  send_and_receive("/amp/gain", 3.5)
--  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})
--  send_and_receive(nil)
--end

test.all()