function [ data ] = ApplyCalib( dataUncalib, offset, scale )

data = bsxfun(@minus,dataUncalib, offset);
data = bsxfun(@times,data, scale);

end

