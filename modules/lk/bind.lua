--[[------------------------------------------------------

  lk bindings generator
  ---------------------

  This uses the 'dub' tool and Doxygen to generate the
  bindings for lk.

  Input:  headers in 'include/lk'
  Output: cpp files in 'src/core'

--]]------------------------------------------------------
require 'lubyk'

local base = lk.pathDir(lk.scriptPath())

local ins = dub.Inspector {
  INPUT    = base .. '/include/lk',
}

local binder = dub.LuaBinder()

binder:bind(ins, {
  output_directory = base .. '/src/more',
  -- Remove this part in included headers
  header_base = base .. '/include',
  -- Execute all lua_open in a single go
  -- with lua_openmdns (creates mdns_core.cpp).
  single_lib = 'lk',
  luaopen    = 'lk_more_c',
  lib_prefix = false,
})

