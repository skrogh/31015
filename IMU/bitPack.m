function [ bitOut7 ] = bitPack( bitIn8 )
bitIn8 = uint8(bitIn8); %cast input to byte array.

bitIn8 = [ bitIn8 uint8(0) ];

for i=1:(length(bitIn8) + ceil(length(bitIn8)/7) - 1)
    j = floor((7*(i+8))/8-6)
    j = floor((7*(i-8))/8+8)
    a =  -mod(i-1,8)-1
    b = 7 - mod(i-1,8)
    
    bitOut7( i ) = bitshift( bitIn8(j), a );
    if mod(i-1,8)~=0
        bitOut7( i ) = bitor( bitOut7( i ), bitshift( bitIn8(j-1), b ) );
    end
    bitOut7(i) = bitand( bitOut7(i), uint8(127) );
end
end