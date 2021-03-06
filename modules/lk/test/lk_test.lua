require 'lubyk'

local should = test.Suite('lk.path helpers')

function should.readAll()
  assertEqual('Hello Lubyk!', lk.content(fixture.path('io.txt')))
end

function should.absolutizePath()
  assertEqual(lfs.currentdir() .. '/foo/bar', lk.absolutizePath('foo/bar'))
  assertEqual('/foo/bar', lk.absolutizePath('/foo/bar'))
  -- assertEqual('/One/two/foo/bar', lk.absolutizePath('foo/bar', '/One/two'))
  -- assertEqual('/foo/bar', lk.absolutizePath('/foo/bar', '/One/two'))
end

function should.merge()
  local base = {a = { b = {x=1}}, c = {d = 4}}
  lk.merge(base, {
    a = 'hello',
    d = 'boom',
  })
  assertValueEqual({a = 'hello', c = {d = 4}, d = 'boom'}, base)
end

function should.deepMerge()
  local base = {a = { b = {x=1}, c = {d = 4}}}
  local a2   = { b = {x=3}, c = {e = 5}, g = 2}
  assertFalse(lk.deepMerge(base, 'a', {c = {d = 4}}))
  assertTrue(lk.deepMerge(base, 'a', a2))
  assertValueEqual({a = { b = {x=3}, c = {d = 4, e = 5}, g = 2}}, base)
end

function should.makePath()
  local path = fixture.path('tmp/foo/bar/baz')
  lk.rmTree(fixture.path('tmp'), true)
  assertPass(function()
    lk.makePath(path)
  end)
  assertEqual('directory', lk.fileType(fixture.path('tmp/foo')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/foo/bar')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/foo/bar/baz')))
  lk.rmTree(fixture.path('tmp'), true)
end

function should.notRmTreeRecursive()
  local path = fixture.path('tmp/fo"o/bar/baz')
  assertPass(function()
    lk.makePath(path)
  end)
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar/baz')))
  assertFalse(lk.rmTree(fixture.path('tmp/fo"o')))
  assertTrue(lk.exist(fixture.path('tmp/fo"o')))
  lk.rmTree(fixture.path('tmp'), true)
end

function should.rmTree()
  local path = fixture.path('tmp/fo"o/bar/baz')
  assertPass(function()
    lk.makePath(path)
  end)
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar/baz')))
  lk.rmTree(fixture.path('tmp/fo"o'), true)
  assertFalse(lk.exist(fixture.path('tmp/fo"o')))
end

function should.move()
  local path  = fixture.path('tmp.txt')
  lk.writeall(path, 'Hello')
  local path2 = fixture.path('tmp2.txt')
  assertTrue(lk.exist(path))
  assertFalse(lk.exist(path2))

  lk.move(path, path2)
  assertTrue(lk.exist(path2))
  assertFalse(lk.exist(path))

  -- cleanup
  lk.rmFile(path)
  lk.rmFile(path2)
end

function should.copy()
  local path  = fixture.path('tmp.txt')
  lk.writeall(path, 'Hello')
  local path2 = fixture.path('tmp2.txt')
  assertTrue(lk.exist(path))
  assertFalse(lk.exist(path2))

  lk.copy(path, path2)
  assertTrue(lk.exist(path2))
  assertTrue(lk.exist(path))

  -- cleanup
  lk.rmFile(path)
  lk.rmFile(path2)
end

function should.writeall()
  local foo = fixture.path('tmp/foo')
  lk.rmTree(foo, true)
  local tmp_path = foo .. '/bar/lk_test_writeall.txt'
  os.remove(tmp_path)
  lk.writeall(tmp_path, 'This is the message')
  assertEqual('This is the message', lk.content(tmp_path))
  lk.rmTree(foo, true)
end

function should.split()
  local list = lk.split('cat,dog,mouse', ',')
  assertEqual('cat'  , list[1])
  assertEqual('dog'  , list[2])
  assertEqual('mouse', list[3])
  assertEqual(3, #list)
end

function should.splitChars()
  local list = lk.split('cat')
  assertEqual('c', list[1])
  assertEqual('a', list[2])
  assertEqual('t', list[3])
  assertEqual(3, #list)
end

function should.strip()
  assertEqual('hop hop', lk.strip(' \t\nhop hop '))
  assertEqual('hop hop', lk.strip('hop hop '))
  assertEqual('hop hop', lk.strip('  hop hop'))
  assertEqual('hop hop', lk.strip('hop hop'))
end

function should.absToRel()
  assertEqual('play/in/trigger', lk.absToRel('/foo/play/in/trigger', '/foo'))
  assertEqual('/foo/bar', lk.absToRel('/foo/bar', '/foo/bar'))
  assertEqual('/foo/baz/boom', lk.absToRel('/foo/baz/boom', '/foo/bar'))
end

function should.returnEmptyOnSpitStartingWithSep()
  local list = lk.split('/my/home', '/')
  assertEqual(''    , list[1])
  assertEqual('my'  , list[2])
  assertEqual('home', list[3])
  assertEqual(3, #list)
end

function should.provideSource()
  assertMatch('test/lk_test.lua$', lk.scriptSource())
end

function should.provideDir()
  assertMatch('test$', lk.scriptDir())
end

function should.provideFile()
  assertMatch('test/lk_test.lua$', lk.scriptPath())
end

function should.testFileExistence()
  assertEqual('file', lk.fileType(fixture.path('foo.yml')))
  assertEqual(nil, lk.fileType(fixture.path('complex.yml')))
  assertEqual('directory', lk.fileType(lk.scriptDir()))
  assertEqual(nil, lk.fileType(nil))
end

function should.shellQuote()
  assertEqual('"foo"', lk.shellQuote('foo'))
  -- foo 25"  --> "foo 25\""
  assertEqual('"foo 25\\\""', lk.shellQuote('foo 25"'))
  -- foo 25\" --> "foo 25\\\""
  assertEqual('"foo 25\\\\\\\""', lk.shellQuote('foo 25\\"'))
end

function should.spawnProcess(t)
  local tmp_path = fixture.path('lk_spawnProcess.txt')
  lk.rmFile(tmp_path)
  assertNil(lk.fileType(tmp_path))
  msg = 'Hello Lubyk'
  lk.spawn([[
    require 'lubyk'
    local function test(file, msg)
      local file = assert(io.open(file, 'w'))
      file:write(msg)
      file:close()
    end
    test(%s)
  ]], tmp_path, msg)
  t:timeout(function()
    return lk.fileType(tmp_path)
  end)
  assertEqual('file', lk.fileType(tmp_path))
  assertEqual(msg, lk.content(tmp_path))
  lk.rmFile(tmp_path)
end

test.all()
