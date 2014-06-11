function [ bitOut8 ] = bitUnpack( bitIn7 )
bitIn7 = uint8(bitIn7);
bitOut8 = zeros(length(bitIn7)-ceil(length(bitIn7)/8),1);
    for i=1:length(bitIn7)-ceil(length(bitIn7)/8)
        j = floor((8*(i-8))/7+9);
        a = 1+mod(i-1,7);
        b = -6 + mod(i-1,7);
        
        
        bitOut8( i ) = bitor( bitshift( bitIn7(j), a ),...
        bitshift( bitIn7(j+1), b ) );
    end
end