

wifi.setmode(wifi.STATION)
wifi.sta.config("ShubUHodMak","Beareater05")
--wifi.sta.connect()

oldVariable = ""
uart.on("data", 0, function(data)
    if (data != oldVariable) then
        http.get("http://192.168.1.16/", nil, function(code, data)
            oldVariable = data
        end)
    end
end, 0)
