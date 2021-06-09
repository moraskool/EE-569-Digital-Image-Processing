
% define global variables
global LeftHeight;
LeftHeight = 483;
global LeftWidth;
LeftWidth = 322;

global MiddleHeight;
MiddleHeight = 487;
global MiddleWidth;
MiddleWidth = 325;

global RightHeight;
RightHeight = 489;
global RightWidth;
RightWidth = 325;

global BytesPerPixel;
BytesPerPixel = 3;

lower_thresh = 0.025;
higher_thresh = 0.3;

% read the 3 files
left = readRaw('left.raw', LeftHeight, LeftWidth, BytesPerPixel);
middle = readRaw('middle.raw', MiddleHeight, MiddleWidth, BytesPerPixel);
right = readRaw('right.raw', RightHeight, RightWidth, BytesPerPixel);

rowOffset = 300;
colOffset = 800;
height = 1200;
width = 2500;

%create black image. 
panorama = zeros(height, width, 3, 'uint8');
for h = 1 : height
    for w = 1 : width
        for k = 1 : 3
            panorama(h, w, k) = 0; %set to black
        end
    end
end
    
%embbed middle into panorama
currentPixel = 0;
embedpixel = 0;
embedLocation = 0;
for h = rowOffset : rowOffset + MiddleHeight
    
    for w = colOffset : colOffset + MiddleWidth
        for k = 1:3
            panorama(h-1,w-1,k) = middle(h-rowOffset, w-colOffset,k);
        end
        %embedpixel = (h * width) + w;
        %embedLocation = embedpixel * 3;
        %panorama(embedLocation + 1) = middle(currentPixel + 1);
        %panorama(embedLocation + 2) = middle(currentPixel + 2);
        %panorama(embedLocation + 3) = middle(currentPixel + 3);
        %currentPixel = currentPixel + 3;

    end
end

% visualizations
figure(1); imshow(panorama);
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



