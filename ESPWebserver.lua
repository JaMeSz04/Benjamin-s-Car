

wifi.setmode(wifi.SOFTAP)
wifi.ap.config({ssid="ShubUhodMak",pwd="Beareater05"})
--uart.setup(0, 19200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)
cfg =
{
    ip="192.168.1.16",
    netmask="255.255.255.0",
    gateway="192.168.4.1"
}
wifi.ap.setip(cfg)
--print(wifi.ap.getip())

bc = wifi.ap.getbroadcast()
--print(bc)

srv=net.createServer(net.TCP) 
srv:listen(3616,function(conn) 
    
    conn:on("receive",function(conn,payload) 
        --print("client connectted!!!")
        --print(payload) 
          
        local _, _, method, path, vars = string.find(payload, "([A-Z]+) (.+)?(.+) HTTP");

        if(method == nil)then
            _, _, method, path = string.find(payload, "([A-Z]+) (.+) HTTP");
        end
        local _GET = {}
        if (vars ~= nil)then
            for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
                _GET[k] = v
            end
        end
        buf = "HTTP/1.1 200 OK \r\n\r\n";
        buf = buf.."<!DOCTYPE html> <html> <body>"
        buf = buf.."<h1> Benjamin's Car Controller </h1><br>";
        buf = buf.."<p> <a href=\"?command=f\"><button>Forward</button>
        </a>&nbsp;<a href=\"?command=b\"><button>Backward</button></a></p>";
        buf = buf.."<p> <a href=\"?command=l\"><button>Left</button></a>&nbsp;<a href=\"?command=r\">
        <button>Right</button></a></p>";
        buf = buf.."<p> <a href=\"?command=s\"><button>Stop</button></a></p>";
        buf = buf.."</body></html>";
        --print(_GET.command)
        if(_GET.command == "f")then
              --print("move forward")
              uart.write(0, "f\n")
        elseif(_GET.command == "b")then
              --print("move backward")
              uart.write(0, "b\n")
        elseif(_GET.command == "l")then
              --print("move left")
              uart.write(0, "l\n")
        elseif(_GET.command == "r")then
              --print("move right")
              uart.write(0, "r\n")
        elseif(_GET.command == "s")then
              --print("stop")
              uart.write(0, "s\n")
        end
      
        --print("Rendered the page")
        conn:send(buf) 
        --print("response sent")
        --print("closing conn")
        conn:close()
        --print("conn closed")
    end)
end)

    

-- conn:close
    
