for i=1:10
6 - mod(i-1,7)
-1-mod(i-1,7)
end
bits = [240, 15, 51, 204, 0, 255, 240, 15, 51, 204, 0, 255 ]
dec2bin(bits)
bits = bitPack( bits );
dec2bin(bits)
bits = bitUnpack(bits);
dec2bin(bits)