function [ out ] = keypressInFig( fig, key )
%KEPRESSINFIG Summary of this function goes here
%   Detailed explanation goes here
key_array = get( fig, 'CurrentCharacter');

out = 0;
if length(key_array) == 1
    if key_array == key
        out = 1;
    end
end

end

