height = 1300;
width = 2000;

% image to be warped, brings it to the coodinate system of the fixed image
movingLeft = imread('left.png');
fixed = imread('middle.png');
%fixed = imread('MiddleStitched.raw');
%fixed = readRaw('MiddleStitched.raw', height, width, 3);
movingRight = imread('right.png');

%movingLeftg = rgb2gray(movingLeft);
%fixed = rgb2gray(middleFixed);

%points = detectSURFFeatures(fixed);
%imshow(fixed); hold on;
%plot(points.selectStrongest(4));
%fixed = points.selectStrongest(4);

%comment out as needed
%cpselect(movingLeft,middleFixed);
cpselect(movingLeft,fixed);
close(h)

% read image data
% of course, you can use imread as well
function result = readRaw(filename, height, width, NumChannels)
    
    % open file
    f = fopen(filename, 'rb');
 
    % read into a 3d matrix
    array = fread(f, inf, 'uint8');
    
    % then close
    fclose(f);
   
    % then construct a 1d array to store the image file. 
    result = zeros(height, width, NumChannels, 'uint8');
    for h = 1 : height
        for w = 1 : width
            for k = 1 : NumChannels
                result(h, w, k) = array(NumChannels * (width * (h - 1) + (w - 1)) + k);
            end
        end
    end
end
