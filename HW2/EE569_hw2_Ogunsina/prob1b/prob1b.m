
% define global variables
global ElephantImgHeight;
ElephantImgHeight = 321;
global ElephantImgWidth;
ElephantImgWidth = 481;

global Ski_PersonImgHeight;
Ski_PersonImgHeight = 481;
global Ski_PersonImgWidth;
Ski_PersonImgWidth = 321;

global BytesPerPixel;
BytesPerPixel = 3;

lower_thresh = 0.025;
higher_thresh = 0.3;

% read the two files
elephant = readRaw('elephant.raw', ElephantImgHeight, ElephantImgWidth, BytesPerPixel);
skiPerson = readRaw('ski_person.raw', Ski_PersonImgHeight, Ski_PersonImgWidth, BytesPerPixel);

% convert to grayscale
elephant_gray = rgbToGray(elephant); 
skiPerson_gray = rgbToGray(skiPerson);

%apply canny edge detection 
elephant_cannyEdge = CannyEdge(elephant_gray, lower_thresh, higher_thresh);
skiPerson_cannyEdge = CannyEdge(skiPerson_gray, lower_thresh, higher_thresh);


% visualizations
figure(1); imshow(elephant_gray);
title(['Grayscale image of elephant.raw']);

figure(2); imshow(skiPerson_gray);
title(['Grayscale image of ski_person.raw']);

figure(3); imshow(elephant_cannyEdge);
title(['Canny Edges image of elephant.raw = '  num2str(lower_thresh) ' and higher threshold = '  num2str(higher_thresh)]);

figure(4);imshow(skiPerson_cannyEdge);
title(['Canny Edges image of ski_person.raw with lower threshold = ' num2str(lower_thresh) ' and higher threshold = '  num2str(higher_thresh)]);

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

% turn to rgb image
function result = rgbToGray(image)
    result = 0.2989 * image(:,:,1) + 0.5870 * image(:,:,2) + 0.1140 * image(:,:,3);
end

% apply cannyedge with low and high thresholds
function result = CannyEdge(image, low, high)

  %  and a low threshold : disregard edges below lower thresh
  %  a high threshold : preserve edges above higher thresh 
  %  between these values [0 1]
  result = edge(image, 'Canny', [low high]);
end



