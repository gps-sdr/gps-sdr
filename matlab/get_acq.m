function [A] = get_acq()

A = [];

fp = fopen('../Acq.txt','rt');
if(fp == -1)
    return;
else
    B = fscanf(fp,'%02d,%02d,%f,%f,%f,%1d\n');
    
    
    
    if(length(B) == 6*32)
        A = reshape(B,[6 32]).';
        fclose(fp);
        return;
    else
        fclose(fp);
        A = [];
        return;
    end
end