function [ s ] = InitSerial( port, portName )
if exist( portName, 'var' )
    fclose(s);
    delete(s);
    clear s;
end

s = serial(port,'BaudRate', 500000, 'InputBufferSize', 2048);
fopen(s);

end

