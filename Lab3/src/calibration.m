% Raw values from position 1
x1,y1,z1;
% Raw values from position 2
x2,y2,z2;
% Raw values from position 3
x3,y3,z3;
% Raw values from position 4
x4,y4,z4;
% Raw values from position 5
x5,y5,z5;
% Raw values from position 6
x6,y6,z6;
% Y is the normalized Earth gravity vector (see Table 2)
Y=[0,0,1;0,0,-1;0,1,0;0,-1,0;1,0,0;-1,0,0];
%Raw data vector from the six positions
w=[x1,y1,z1,1;x2,y2,z2,1;x3,y3,z3,1;x4,y4,z4,1;x5,y5,z5,1;x6,y6,z6,1];
% W transpose
wt=w';
% X is the calibration parameters, it is calculated from the equation X=(wt*w)^-1*wT*Y
X=inv(wt*w)*wt*Y;
disp(X);