function [axis, angle] = quatern2euler(q)
%QUATERN2EULER Converts a quaternion orientation to ZYX Euler angles
%
%   q = quatern2euler(q)
%
%   Converts a quaternion orientation to ZYX Euler angles where phi is a
%   rotation around X, theta around Y and psi around Z.
%
%   For more information see:
%   http://www.x-io.co.uk/node/8#quaternions
%
%	Date          Author          Notes
%	27/09/2011    SOH Madgwick    Initial release

angle = 2*atan2(norm(q(2:4)),q(1));
if angle == 0
    axis = [0,0,0];
else
    axis = q(2:4)./sin(angle/2);
end

end

