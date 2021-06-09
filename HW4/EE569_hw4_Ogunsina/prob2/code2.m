
% define global variables
global ImgHeight;
ImgHeight = 360;
global ImgWidth;
ImgWidth = 575;

global BytesPerPixel;
BytesPerPixel = 1;  % grayscale images
%% PART A : Texture Segmentation : Feature Extraction

%% STEP 0: Reading all training images

% then the composite image
compfileName = strcat('C:\Users\moras\OneDrive\Documents\TextBooks\Digital Image Processing\Homeworks\HW4\hw4MATLAB\prob2\composite.png'); 
compImage = imread(compfileName);
   
%% STEP 1: FILTER BANK COMPUTATION
filterBank = GetLawsFilters();
%%
% testing image read with visualizations
%figure(1); imshow(compImage);
%% STEP 2: USE WINDOW APPRAOCH FIRST TO COMPUTE ENERGY FEATURE 
% let's get our window size (try different window sizes)
windowSize = 15;
%% STEP 3: Applying Law's filters to obtain a vector response, and average energies
% Apply law's filters to each image to obtain a vector response
 compfeatureMatrix = ApplyLawsFilters(filterBank,compImage, windowSize);
%% STEP 4: Energy Feature Computation
%combine all the response (575*360x25 matrix)
EnergyFeatureMatrix = GetEnergyFeature(compfeatureMatrix,windowSize);

%% STEP 5 : Energy Feature Normalization 
%  Reducing features from 25 to 24 dimensions (360*575x24) use only L5'E5 filter
energyFeatureNormMatrix = EnergyFeaturesNormalization(EnergyFeatureMatrix);

%% STEP 6: Get the Feature Matrix
%create the vector response (207000*14) dimensional matrix)
featureMatrix = GetFeatureMatrix(energyFeatureNormMatrix);

%% STEP 8: Apply K-Means clustering on composite Images
% 5 gray levels in the composite image
% I'm using MATLAB'S kmeans function here 
% (0,63, 127,191,255) gray levels may denote these 5 gray levels 
% source : https://www.mathworks.com/help/stats/classification-nearest-neighbors.html?s_tid=CRUX_topnav
kmeansPred = kmeans(featureMatrix, 5);  % without PCA = 24D
%% STEP 9: Apply Segmentation
% 5 gray levels in the composite image
% (0,63, 127,191,255) gray levels may denote these 5 gray levels
% based on the 25D feature matrix i.e without PCA
segmentedFeatures = ApplySegmentation(kmeansPred);
%% ADVANCED SEGMENTATION 
%% STEP 10: Perform Principal Component Analysis to reduce dimension to 3
%feature dimension from 14 to 3 -> 207000x3
PCAfeatureMatrix = PCA(featureMatrix); 

%% STEP 11: Apply K-Means clustering on after PCA reduction
% source : https://www.mathworks.com/help/stats/kmeans.html
kmeansPredPCA = kmeans(PCAfeatureMatrix, 5); % with PCA = 3D
%% STEP 12: Apply Segmentation
% 5 gray levels in the composite image
% (0,63, 127,191,255) gray levels may denote these 5 gray levels
% based on the 25D feature matrix i.e without PCA
segmentedFeaturesPCA = ApplySegmentation(kmeansPredPCA);
%% STEP 13: POST processing to remove holes
% postprocessing to improve the result
% source : https://www.mathworks.com/help/images/ref/imfill.html
holeFilledImgPCA = imfill(segmentedFeaturesPCA); 
holeFilledImg = imfill(segmentedFeatures);

%% SHOW FINAL RESULTS
figure(3);imagesc(uint8(holeFilledImg));
figure(4);imagesc(uint8(holeFilledImgPCA));
%% FUNCTIONS

% do zero padding here
function resultImage = ZeroPadding(Image, windowSize)
   
   % have to apply boundary extension first
   % zero padding is applied here
   % let's extend it by (windowSize - 1)/2 pixels on each side
   
    ImgHeight = 360;
    ImgWidth = 575;
    padHeight = ImgHeight + windowSize - 1;   % 374
    padWidth = ImgWidth + windowSize - 1; % 589
    div = (windowSize - 1)/2;
    
    resultImage= zeros(padHeight, padWidth);
    for row = 1:ImgHeight
        for col = 1:ImgWidth
            resultImage(row + div, col+div)= Image(row, col);
        end
    end
end

% define laws filter
% returns a 25 filterbank  
function filterBank = GetLawsFilters()
    filters = [
               [1 4 6 4 1];     %L5
               [-1 -2 0 2 1];   %E5
               [-1 0 2 0 -1];   %S5
               [-1 2 0 -2 1];   %W5
               [1 -4 6 -4 1]];  %R5
           
    filterBank = zeros(5,5);  % create an empty 5x5 matrix, used as our filterbank
    filterCount = 1;  % to keep track of how many filters we have used so far
    for i = 1 : 5
       for j = 1 : 5 
           % get tensor product of the 5 1D kernels
           % can also use squeeze here,might change later
           filterBank(:,:,filterCount) =  filters(i,:)'*filters(j,:);
           filterCount = filterCount+1;
       end
    end
end


% perform convolution here and 
% return the output as the average energy from 
% the filer convolution
function convolvedImage = convolve(filter, Image, windowSize)


   % have to apply boundary extension first
   % zero padding is applied here
   % let's extend it by 7 pixels on each side
   
    ImgHeight = 360;
    ImgWidth = 575;
    padHeight = ImgHeight + windowSize - 1;   % 374
    padWidth = ImgWidth + windowSize - 1; % 589
    div = (windowSize - 1)/2;
    
    resultImage= zeros(padHeight, padWidth);
    for row = 1:ImgHeight
        for col = 1:ImgWidth
            resultImage(row+div, col+div)= Image(row, col);
        end
    end
    
   convolvedImage = zeros(ImgHeight,ImgWidth);
   
    % do Convolution here
     for row = div + 1:360 + div          %(from divth + 1 row to 360th row 
         for col = div + 1:575 + div      %(from divth + 1 cols to 575nd cols 
             weightSum = 0;      % the weight for each convolvoluton
             for i = -2:2        % do "up" and "down"
                 for j = -2:2    % do "left"  and "right"
                     pixel = resultImage(row+i, col+j);   % get the pixel from the padded image
                     weightSum = weightSum + (pixel * filter(i+3, j+3)); % then convolve
                 end                
             end
             convolvedImage(row - div, col - div) = weightSum;  % do a fix to "remove" the padded rows and columns
         end
     end
       
end

% Filter Bank response computation
% use 25 5x5 law's filters to extract the response vectors
% from each pixel in image (convolution is involved)
function featureMatrix = ApplyLawsFilters(filterBank, Image, windowSize)
    
    featureMatrix = zeros(360,575); % emtpy matrix of feataure matrix
    
    imgs = 1;
       for f = 1:25
           filter = filterBank(:,:,f);  % get each filter
           averageEnergy = convolve(filter, Image, windowSize); % then convolve and get average energy
           featureMatrix(:,:,imgs) = averageEnergy; % this the feature matrix
           imgs = imgs + 1;
       end
    
end

% perform convolution here and 
% return the output as the average energy from 
% the filer convolution
function EnergyMatrix = GetEnergyFeature(Image, windowSize)

   div = (windowSize - 1)/2;
   % have to apply boundary extension first
   % zero padding is applied here
   % let's extend it by 7 pixels on each side
    paddedImage= zeros(360 + (windowSize - 1), 575 + (windowSize - 1));
    
    
    for i = 1:25
        %ZeroPadding(Image, windowSize);
        paddedImage(:,:,i) = ZeroPadding(Image(:,:,i), windowSize);
    end
    
    
    % Get Energy Matrix
    EnergyMatrix = zeros(360,575);
    
    % get the average energy of each image after applying all the filters
    % replace with center pixel
    for imgs = 1:25
        
        %for each image, 
        for row = div + 1:360 + div
            for col = div + 1:575 + div
                  %Now take average of the windowsize/2 x windowsize/2 feature feature
                  %get the average energy of each image after applying all the filters
                  averageEnergy = 0;
                  for i = -(div):div
                    for j = -(div):div
                        
                        averageEnergy = averageEnergy + abs(paddedImage(row + i, col + j, imgs));                       
                    end                
                  end
                  EnergyMatrix(row - div, col - div, imgs) = averageEnergy/(windowSize * windowSize);                  
            end
        end
    end 
    
    
     
end

%  Energy Feature normalization
%  Use its L5'55 energy to normal all other features at each pixel.
function featureMatrixNormlzed = EnergyFeaturesNormalization(energyMatrix)
   
   featureMatrixNormlzed = zeros(360, 575);
    
    for row = 1:360       % height
        for col = 1:575   % width
            val = energyMatrix(row, col, 1);%L5L5
            for imgs = 2:25
               featureMatrixNormlzed(row, col, imgs - 1) = (energyMatrix(row, col, imgs))/(val); 
            end           
        end
    end
end


function featureMatResult = GetFeatureMatrix(EnergyMatrixNorm)        
    
    ImgHeight = 360;
    ImgWidth = 575;
    ImgSize = ImgHeight * ImgWidth; % get the size in 1D of the image
    % construct zero matrix of imgsize*24filters.
    % discarding the L5'E5 filter
    featureMatResult = zeros(ImgSize, 24);
    count = 1;
    
    % loop through the pixels
    for rows = 1:ImgHeight
        for cols = 1:ImgWidth
            for imgs = 1:24  % for each filter in 
                featureMatResult(count, imgs) = EnergyMatrixNorm(rows, cols, imgs); 
            end
            count = count+1;
        end
    end
end

% peform feature reduction here
% using PCA
function features_pca = PCA(featureMatrix)

    data_std = zscore(featureMatrix); % get the zscore
    
    %Need the covariance matrix
    sigma = (1/36)*(data_std')*(data_std); % square filter response i.e data_std
    
    % get the eigen values for report purpose
    eigenVal = eig(sigma); 
    
    %apply SVD to the covariance matrix
    [U, ~, ~] = svd(sigma);
    Ured = U(:,1:3);
    
    %finding k dimension subspace here k = 3
    subspace = Ured' * data_std' ;
    
    x = subspace(1,:)'; %First principal component
    y = subspace(2,:)'; %Second principal component
    z = subspace(3,:)'; %third principal component 
   
    %Plotting
    features_pca = subspace';
    scatter3(x,y,z, 'r');
    xlabel('First Principal component');
    ylabel('Second Principal component');
    zlabel('Third Principal component');
    title('Train images');
     
   
end

% do segmentation here , based on the feature matrix
% 5 gray levels in the composite image
% (0,63, 127,191,255) gray levels may denote these 5 gray levels 
function segmentedResult = ApplySegmentation(KmeansclassifiedVector)
    
    segmentedResult = zeros(450, 600);
    % use the default gray levels in homework handout
    count = 1;
    
    % loop through rows and cols
    for rows = 1:360
        
        for cols = 1:575
          
            class = KmeansclassifiedVector(count); % get the kmeans clustered class
            
            if(class == 1)
                segmentedResult(rows, cols) = 0;    % gray level 1         
            elseif(class == 2)
                segmentedResult(rows, cols) = 63;   % gray level 2
            elseif(class == 3)
                segmentedResult(rows, cols) = 127;  % gray level 3
            elseif(class == 4)
                segmentedResult(rows, cols) = 191;  % gray level 4
            else
                segmentedResult(rows, cols) = 255;  % gray level 5
            end
            count = count+1;
        end
    end
end