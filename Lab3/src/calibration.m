% Raw values from position 1
x1=0;
y1=-3;
z1=55;
% Raw values from position 2
x2=0;
y2=-1;
z2=-53;
% Raw values from position 3
x3=0;
y3=53;
z3=4;
% Raw values from position 4
x4=0;
y4=-58;
z4=0;
% Raw values from position 5
x5=54;
y5=-4;
z5=-1;
% Raw values from position 6
x6=-54;
y6=-2;
z6=1;
% Y is the normalized Earth gravity vector (see Table 2)
Y=[0,0,1;0,0,-1;0,1,0;0,-1,0;1,0,0;-1,0,0];
%Raw data vector from the six positions
w=[x1,y1,z1,1;x2,y2,z2,1;x3,y3,z3,1;x4,y4,z4,1;x5,y5,z5,1;x6,y6,z6,1];
% W transpose
wt=w';
% X is the calibration parameters, it is calculated from the equation X=(wt*w)^-1*wT*Y
X=inv(wt*w)*wt*Y;
disp(X);