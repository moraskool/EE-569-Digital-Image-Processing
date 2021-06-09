
%moving image leftMiddle
x0 = [310.25,293.75,223.75,218.75];
y0 = [307.75,344.75,358.75,200.25];

%fixed image leftMiddle
xd = [961.25 - 800 , 949.25 - 800 , 886.75 - 800 , 876.25 - 800];
yd = [598.25 - 300, 632.75 - 300 , 665.75 - 300 , 502.25 - 300];

P =[
x0(1)  y0(1)  1   0    0    0   -x0(1)*xd(1)   -y0(1)*xd(1);   %xd(1);   
x0(2)  y0(2)  1   0    0    0   -x0(2)*xd(2)   -y0(2)*xd(2);   %yd(1);   
x0(3)  y0(3)  1   0    0    0   -x0(3)*xd(3)   -y0(3)*xd(3);   %xd(2);
x0(4)  y0(4)  1   0    0    0   -x0(4)*xd(4)   -y0(4)*xd(4);   %yd(2);
0    0    0 x0(1)   y0(1)   1   -x0(1)*yd(1)   -y0(3)*yd(1);   %xd(3);
0    0    0 x0(2)   y0(2)   1   -x0(2)*yd(2)   -y0(3)*yd(3);   %yd(3);
0    0    0  x0(3)  y0(3)   1   -x0(3)*yd(3)   -y0(4)*yd(3);   %xd(4);
0    0    0  x0(4)  y0(4)   1   -x0(4)*yd(4)   -y0(4)*yd(4) ]; %yd(4)];

b = [xd(1); xd(2);xd(3); xd(3);yd(1); yd(2); yd(3); yd(4)];
h = P\b % the homography matrix

%[U,S,V] = svd(P);
%H = V(:,end);
%H = reshape(H,3,3);

%A = zeros(2*4,9);
%for i=1:4
%    A(2*i-1,:)= [x0(i),y0(i),1,0,0,0,-x0(i)*xd(i), -xd(i)*y0(i),-xd(i)];
 %   A(2*i,:)= [0,0,0,x0(i),y0(i),1, -x0(i)*yd(i), -yd(i)*y0(i),-yd(i)];
%end

%solve for homography
%[U1,S1,V1] = svd(A);
%H1 = V1(:,9);
%H1 = reshape(H1,3,3)
%h = inv(H1)

%moving image MiddleRight
%x1 = [40.75,126.75,37.75,283.25];
%y1 = [193.75,267.25,312.75,330.75];

%this one
%fixed image leftMiddle
%xm = [59 + 300, 79 + 300, 9 + 300, 66+ 300];
%ym = [381 + 795, 121 + 795, 247 + 795, 202+ 795];

%B = zeros(2*4,9);
%for i=1:4
%    B(2*i-1,:)= [x1(i),y1(i),1,0,0,0,-x1(i)*xm(i), -xm(i)*y1(i),-xm(i)];
%    B(2*i,:)= [0,0,0,x1(i),y1(i),1, -x1(i)*ym(i), -ym(i)*y1(i),-ym(i)];
%end

%solve for homography
%[U2,S2,V2] = svd(B);
%H2 = V2(:,9);
%H2 = reshape(H2,3,3);
%H = inv(T2)*H*T1


%function H=homography(x1,y1,x2,y2,x3,y3,x4,y4 , xp1,yp1,xp2,yp2,xp3,yp3,xp4,yp4)
%This function will find the homography betweeb 4 points using svd
%

%A=[
%-x1  -y1  -1   0    0    0   x1*xp1   y1*xp1   xp1;
% 0    0    0 -x1   -y1  -1   x1*yp1   y1*yp1   yp1;
%-x2  -y2  -1   0    0    0   x2*xp2   y2*xp2   xp2;
 %0    0    0 -x2   -y2  -1   x2*yp2   y2*yp2   yp2;
%-x3  -y3  -1   0    0    0   x3*xp3   y3*xp3   xp3;
% 0    0    0 -x3   -y3  -1   x3*yp3   y3*yp3   yp3;
%-x4  -y4   -1  0    0    0   x4*xp4   y4*xp4   xp4;
% 0    0    0  -x4  -y4  -1   x4*yp4   y4*yp4   yp4];

%[U,S,V] = svd(A);


%H=V(:,end);
%H=reshape(H,3,3);
%end




