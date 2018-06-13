local server = {}

local cache = {}

require("util")

server.storage = function (_args)
  while true do
    local arg = qlnode_recv()
    for k, v in pairs(arg) do
      qlog("k: " .. k .. ", v: " .. v)
    end

    local key = arg.key
    local cmd = arg.cmd
    if not key then
      return server.storage()
    end

    if cmd == "set" then
      cache[key] = arg.value
      local data = {}
      data.response = "STORED\r\n"
      qlnode_send(arg.__src, data)
    elseif cmd == "get" then
      local data = {}
      data.val = cache[key]
      data.response = ""
      if data.val then
	data.response = "VALUE " .. key .. " 0 " .. tostring(string.len(data.val)) .. "\r\n"
	qlog(data.val)
	data.response = data.response .. data.val .. "\r\n"
      end
      data.response = data.response .. "END\r\n"
      qlnode_send(arg.__src, data)
    end
  end
end

function accept(_listen, _storage_id)
  local socket = qltcp_accept(_listen)
  -- spawn a child to handle the request
  local aid = qlnode_spawn("child", "child", {sock = socket, storage_id = _storage_id});
  accept(_listen, _storage_id)
end

server.start = function()
  qlog("server start");

  -- accept connection
  local socket, ret = qltcp_listen(22880);
  if socket then
    -- spawn storage process
    local storage_id = qlnode_spawn("server", "storage")
    accept(socket, storage_id)
  else
    qerror(ret)
  end
end

_G["server"] = server

