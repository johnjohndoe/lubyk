local lib = {suites = {}}
test = lib

function lib.run(tests)
  lunit.testcase(tests)
  lunit.main()
end

function lib.Suite(name)
  local suite = {_info = {name = name, tests = {}, errors = {}}}
  table.insert(lib.suites, suite)
  -- default setup and teardown functions
  suite.setup    = function() end
  suite.teardown = function() end
  return suite
end

function lib.all()
  lib.total_test = 0
  lib.total_asrt = 0
  lib.total_fail = 0
  if rawget(_G, 'mimas') then
    lib.gui()
  else
    for i, suite in ipairs(lib.suites) do
      lib.run_suite(suite)
      lib.report_suite(suite)
    end
    lib.report()
  end
end

function lib.gui()
  if lib.suites[1] then
    app = mimas.Application()
    app:post(function()
      for i, suite in ipairs(lib.suites) do
        lib.run_suite(suite)
        lib.report_suite(suite)
      end
    end)

    app:exec()
  end
  -- get report when all threads have finished (all windows closed)
  lib.report()
end

function lib.load_all(...)
  if not arg[1] then
    arg = {'modules'}
  end
  for _, mod in ipairs(arg) do
    for file in rk.Dir(mod):glob('test/.+_test[.]lua$') do
      dofile(file)
    end
  end
end

function lib.run_suite(suite)
  local test_count = 0
  local fail_count = 0
  local errors = suite._info.errors
  lib.current_suite = suite
  suite._info.assert_count = 0
  -- list of objects protected from gc
  suite._info.gc_protect = {}
  local gc_protect = suite._info.gc_protect
  -- run all tests in the current file
  for name,func in pairs(suite) do
    if type(func) == 'function' then
      -- make sure it's a test
      if name ~= '_info' and name ~= 'setup' and name ~= 'teardown' then
        test_count = test_count + 1
        gc_protect[name] = {}
        suite.setup()
          local ok, err = pcall(func, gc_protect[name])
          if not ok then
            fail_count = fail_count + 1
            --local file, line, message = string.match(err, "([^/\.]+\.lua):(%d+): (.+)")
            --if message then
            --  errors[name] = message
            --else
              errors[name] = err
            --end
          end
        suite.teardown()
      end
    end
  end

  suite._info.test_count = test_count
  suite._info.fail_count = fail_count
end

function lib.report_suite(suite)
  lib.ok_message = ''
  if suite._info.fail_count == 0 then
    ok_message = 'OK'
  else
    ok_message = string.format('%i Failure(s)', suite._info.fail_count)
  end
  print(string.format('==== %-18s (%2i tests): %s', suite._info.name, suite._info.test_count, ok_message))
  lib.total_test = lib.total_test + suite._info.test_count
  lib.total_asrt = lib.total_asrt + suite._info.assert_count
  if suite._info.fail_count > 0 then
    for name, err in pairs(suite._info.errors) do
      lib.total_fail = lib.total_fail + 1
      local hname = string.gsub(name, '_', ' ')
      print(string.format('  %i. Should %s\n     %s\n', lib.total_fail, hname, err))
    end
  end
end

function lib.report()
  print('\n')

  if lib.total_test == 0 then
    print(string.format('No tests defined. Test functions must end with "_test.lua"'))
  elseif lib.total_fail == 0 then
    if lib.total_test == 1 then
      print(string.format('Success! %i test passes (%i assertions).', lib.total_test, lib.total_asrt))
    else
      print(string.format('Success! %i tests pass (%i assertions).', lib.total_test, lib.total_asrt))
    end
  elseif lib.total_test == 1 then
    if lib.total_fail == 1 then
      print(string.format('Fail... %i failure / %i test', lib.total_fail, lib.total_test))
    else
      print(string.format('Fail... %i failures / %i test', lib.total_fail, lib.total_test))
    end
  else
    if lib.total_fail == 1 then
      print(string.format('Fail... %i failure / %i tests', lib.total_fail, lib.total_test))
    else
      print(string.format('Fail... %i failures / %i tests', lib.total_fail, lib.total_test))
    end
  end
  print('')
end

------------------------------------ ASSERTIONS ---------------------------

local function format_arg(arg)
  local argtype = type(arg)
  if argtype == "string" then
    return "'"..arg.."'"
  elseif argtype == "number" or argtype == "boolean" or argtype == "nil" then
    return tostring(arg)
  else
    return "["..tostring(arg).."]"
  end
end

function lib.assert(ok, msg)
  lib.current_suite._info.assert_count = lib.current_suite._info.assert_count + 1
  if not ok then
    error(msg, 3)
  end
end

function fail(msg)
  lib.assert(false, msg)
end

function assert_false(ok)
  lib.assert(not ok, string.format('Should fail but passed.'))
end

function assert_true(ok)
  lib.assert(ok, string.format('True expected but was false.'))
end

function assert_equal(expected, value)
  if type(expected) == 'table' then
    assert_table_equal(expected, value)
  else
    lib.assert(value == expected, string.format('Expected %s but found %s.', format_arg(expected), format_arg(value)))
  end
end

function assert_table_equal(expected, value)
  assert_equal('table', type(value))
  for i, v in ipairs(expected) do
    assert_equal(v, value[i])
  end
  for k, v in pairs(expected) do
    assert_equal(v, value[k])
  end
end

function assert_not_equal(unexpected, value)
  lib.assert(value ~= unexpected, string.format('Should not equal %s.', format_arg(unexpected)))
end

function assert_match(pattern, value)
  lib.assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  lib.assert(string.find(value, pattern), string.format('Expected to match %s but was %s.', format_arg(pattern), format_arg(value)))
end

function assert_not_match(pattern, actual, msg)
  lib.assert(type(value) == 'string', string.format('Should be a string but was a %s.', type(value)))
  lib.assert(not string.find(value, pattern), string.format('Expected to not match %s but was %s.', format_arg(pattern), format_arg(value)))
end

function assert_error(pattern, func)
  local ok, err = pcall(func)
  lib.assert(not ok, string.format('Should raise an error but none found.'))
  lib.assert(string.find(err, pattern), string.format('Error expected to match %s but was %s', format_arg(pattern), format_arg(err)))
end

function assert_pass(func)
  local ok, err = pcall(func)
  lib.assert(ok, string.format('Should not raise an error but %s found.', err))
end