
% define global variables
global ImgHeight;
ImgHeight = 128;
global ImgWidth;
ImgWidth = 128;

global BytesPerPixel;
BytesPerPixel = 1;  % grayscale images
%% PART A : Texture Classification : Feature Extraction
%% STEPTRAIN 0: Get the Filter Bank using Law's Filter
filterBankTrain = GetLawsFilters();
%% get the labels, transposed (to be vertical so can match with our features)
labels = [0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3]';
%% STEPTRAIN 1: Reading all training images

% first, create empty matrices to store the staked images
blanketImages = zeros(128, 128);
brickImages = zeros(128, 128);
grassImages = zeros(128, 128);
riceImages = zeros(128, 128);

 % then read them
 % there are 9 images of each class
for i = 1:9
   blanketfileName = strcat('C:\Users\moras\OneDrive\Documents\TextBooks\Digital Image Processing\Homeworks\HW4\hw4MATLAB\prob1\train\blanket',num2str(i),'.raw');
   brickfileName = strcat('C:\Users\moras\OneDrive\Documents\TextBooks\Digital Image Processing\Homeworks\HW4\hw4MATLAB\prob1\train\brick',num2str(i),'.raw');
   grassfileName = strcat('C:\Users\moras\OneDrive\Documents\TextBooks\Digital Image Processing\Homeworks\HW4\hw4MATLAB\prob1\train\grass',num2str(i),'.raw');
   ricefileName = strcat('C:\Users\moras\OneDrive\Documents\TextBooks\Digital Image Processing\Homeworks\HW4\hw4MATLAB\prob1\train\rice',num2str(i),'.raw');

    blanketImages(:,:,i) = readRaw(blanketfileName,ImgHeight, ImgWidth, BytesPerPixel);
    brickImages(:,:,i) = readRaw(brickfileName,ImgHeight, ImgWidth, BytesPerPixel);
    grassImages(:,:,i) =  readRaw(grassfileName,ImgHeight, ImgWidth, BytesPerPixel);
    riceImages(:,:,i) = readRaw(ricefileName,ImgHeight, ImgWidth, BytesPerPixel);
end
%%
% testing image read with visualizations
%figure(1); imshow( riceImages(:,:,7))

%% STEPTRAIN 2: Reduce Illuminataion Effects i.e subract image mean
blanketImagesReducedIllum = SubtractMeanTrain(blanketImages);
brickImagesReducedIllum = SubtractMeanTrain(brickImages);
grassImagesReducedIllum = SubtractMeanTrain(grassImages);
riceImagesReducedIllum = SubtractMeanTrain(riceImages);

%% STEPTRAIN 3: Applying Law's filters to obtain a vector response, and average energies
% Apply law's filters to each image to obtain a vector response
 blanketfeatureTrainMatrix = ApplyLawsFiltersTrain(filterBankTrain,blanketImagesReducedIllum);
 brickfeatureTrainMatrix = ApplyLawsFiltersTrain(filterBankTrain,brickImagesReducedIllum);
 grassfeatureTrainMatrix = ApplyLawsFiltersTrain(filterBankTrain, grassImagesReducedIllum);
 ricefeatureTrainMatrix = ApplyLawsFiltersTrain(filterBankTrain, riceImagesReducedIllum);

 
%% STEPTRAIN 3: combine all responses into 1 feature Matrix
%combine all the response (36*25 i.e (9x4 images) x (25 filters) dimensional matrix)
featureTrainMatrix = [blanketfeatureTrainMatrix; brickfeatureTrainMatrix; grassfeatureTrainMatrix; ricefeatureTrainMatrix];

%% STEPTRAIN 3a : Reducing features from 25 to 15 dimensions (36*15 )
reducedfeatureTrainMatrix = reduceFeaturesTrain(featureTrainMatrix);

%% STEPTRAIN 3b : To find feature dimension with strongest descriminant power
%  for reporting
% just calculate the variance
varianceTrainVectors = zeros(15, 1);
for i = 1:15
    varianceTrainVectors(i) = var(reducedfeatureTrainMatrix(:,i));
end

%% STEPTRAIN 4: Perform Principal Component Analysis to reduce
%feature dimension from 25 to 3 -> 3x36
PCAfeatureTrainMatrix = PCATrain(reducedfeatureTrainMatrix); 

%% STEPTRAIN 5 : Apply the K nearest Neighbour
% source : https://www.mathworks.com/help/stats/classification-nearest-neighbors.html?s_tid=CRUX_topnav
% NumNeighbours = Number of Nearest Neighbours to find = 3
% NSMethod : Nearest Neighbour search method use exhaustive
% Distance = use the Mahalanobis distance metric
KnnModel = fitcknn(PCAfeatureTrainMatrix,labels,'NumNeighbors',3,...
    'NSMethod','exhaustive','Distance','mahalanobis',...
    'Standardize',1);

% % use default params,
% had a problem here with pdist. 
% followed the resolution at 
predKnnTrain3D = predict(KnnModel,PCAfeatureTrainMatrix); % on training samples

%% TESTING TESTING TESTING CODE 
%% Need to preprocess the Test images in similar way too 
filterBankTest = GetLawsFilters();
%% STEPTEST 1: Reading all testing images
testImages = zeros(128, 128); % empty image to store values
% there is 1 image of each class
for i = 1:12
   testFileName = strcat('C:\Users\moras\OneDrive\Documents\TextBooks\Digital Image Processing\Homeworks\HW4\hw4MATLAB\prob1\test\',num2str(i),'.raw');
   testImages(:,:,i) = readRaw(testFileName,ImgHeight, ImgWidth, BytesPerPixel);
end
%%
% testing image read with visualizations
 %figure(1); imshow( testImages(:,:,1)/255);

%% STEPTEST 2: Reduce Illuminataion Effects i.e subract global image mean
testImagesSubMean = SubtractMeanTest(testImages);
%% STEPTEST 3: Applying Law's filters to obtain a vector response, and average energies
testfeatureMatrix = ApplyLawsFiltersTest(filterBankTest,testImagesSubMean);
%% STEPTEST 4 : Reducing features from 25 to 15 dimensions (36*15 )
reducedfeatureTestMatrix = reduceFeaturesTest(testfeatureMatrix);

%% STEPTEST x :Optional, To find feature dimension with strongest descriminant power
%  for reporting
varianceTestVectors = zeros(15, 1);
for i = 1:15
    varianceTestVectors(i) = var(reducedfeatureTestMatrix(:,i));
end
%% STEPTEST 4: Perform Principal Component Analysis to reduce
%feature dimension from 25 to 3 -> 3x36
PCAFeatureTestMatrix = PCATest(reducedfeatureTestMatrix);
 
%% 
% Now let's test the KNN on the test samples
predKnnTest3D = predict(KnnModel,PCAFeatureTestMatrix); 
%% PART B : Advanced  Texture Classification 
%% K-Means clustering on Test Images
% source : https://www.mathworks.com/help/stats/kmeans.html
kmeansPredPCA = kmeans(PCAFeatureTestMatrix, 4); % with PCA 3D
kmeansPred = kmeans(testfeatureMatrix, 4);  % without PCA 25D

%% SVM classifier on trainning images
% source https://www.mathworks.com/help/stats/fitcecoc.html
% % use default params,
% with PCA, 3D
SVMModel = fitcecoc(PCAfeatureTrainMatrix, labels); 
[predTrain3D, scoreTrain3D] = predict(SVMModel,PCAfeatureTrainMatrix); % on training samples

% not needed
% without PCA, 25D
%SVMM25Dodel = fitcecoc(featureTrainMatrix, labels); % use default params,
%[predTrain25D, scoreTrain25D] = predict(SVMModel,featureTrainMatrix); % on training samples

% Now let's test it on the test samples
[predTest3D, scoreTest3D] = predict(SVMModel,PCAFeatureTestMatrix); 

%disp(SVMModel.ClassNames);
%CVMdl = crossval(SVMModel,'Options'); % Cross-validate Mdl using 10-fold cross-validation.
%genError = kfoldLoss(CVMdl); % Estimate the generalized classification error.
 
%% Random Forest classifier : Create A Bag of Decisin Trees
% source : https://www.mathworks.com/help/stats/treebagger.html
% % use default params,
% with PCA, 3D
RFModel = TreeBagger(5,PCAfeatureTrainMatrix,labels, 'OOBPrediction', 'On', 'Method', 'classification');
view(RFModel.Trees{1},'Mode','graph')

%predict of Train samples
predRFTrain3D = predict(RFModel,PCAfeatureTrainMatrix); % on training samples

% Now let's predict it on the test samples
predRFTest3D = predict(RFModel,PCAFeatureTestMatrix); 

%% FUNCTIONS
% read image data
% of course, you can use imread as well
function result = readRaw(filename, height, width, NumChannels)
    
    % open file
    f = fopen(filename, 'r');
    
    if (f == -1)
        printError('Can not open input image file');
    end
 
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

% write image to file
function writeRaw(filename, image)
    
    % write the image to the file name
    f = fopen(filename, 'wb');
    if (f == -1)
        printError('Can not open output image file');
    end
    fwrite(f, image', 'uint8');
    fclose(f);
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

% Filter Bank response computation
% use 25 5x5 law's filters to extract the response vectors
% from each pixel in image (convolution is involved)
function featureTrainMatrix = ApplyLawsFiltersTrain(filterBank, TrainingImages)
    featureTrainMatrix = zeros(9,25); % each image in the class, 25 values
    
    for imgs = 1:9
       %Image = TrainingImages{imgs}; % get each image
       Image = TrainingImages(:,:,imgs); % get each image
       for f = 1:25
           filter = filterBank(:,:,f);  % get each filter
           averageEnergy = convolve(filter, Image); % then convolve with the image
           featureTrainMatrix(imgs, f) = averageEnergy; % this becomes the average energy of that convolution
       end
    end
end

% Filter Bank response computation
% use 25 5x5 law's filters to extract the response vectors
% from each pixel in image (convolution is involved)
function featureMatrix = ApplyLawsFiltersTest(filterBankTest, TrainingImages)
    featureMatrix = zeros(9,25); % each image in the class, 25 values
    
    for imgs = 1:12
       %Image = TrainingImages{imgs}; % get each image
       Image = TrainingImages(:,:,imgs); % get each image
       for f = 1:25
           filter = filterBankTest(:,:,f);  % get each filter
           averageEnergy = convolve(filter, Image); % then convolve with the image
           featureMatrix(imgs, f) = averageEnergy; % this becomes the average energy of that convolution
       end
    end
end


% perform convolution here and 
% return the output as the average energy from 
% the filer convolution
function averageEnergy = convolve(filter, Image)

   % have to apply boundary extension first
   % zero padding is applied here
   % let's extend it by 2 pixels on each side
    padSize = 132;
    imgSize = 128;
    paddedImage= zeros(padSize, padSize);
    
    for row = 1:imgSize
        for col = 1:imgSize
            paddedImage(row+2, col+2) = Image(row, col);
        end
    end
    
    % then do the convolution now
    convolutedImage= zeros(imgSize,imgSize);

     for row = 3: padSize - 2
         for col = 3:padSize - 2
             convolution = 0;
             weightSum = 0;     % the weight for each convolvoluton
             for i = -2:2       % do "up" and "down"
                 for j = -2:2   % do "left"  and "right"
                     pixel = paddedImage(row+i, col+j);   % get the pixel from the padded image
                     convolution = convolution + (pixel*filter(i+3, j+3));   % then convolve
                     weightSum = weightSum + filter(i+3, j+3);               % and update our weights
                 end                
             end
             convolutedImage(row-2, col-2) = convolution;   % do a fix to "remove" the padded rows and columns
         end
     end
     
     %get the average energy of each image after applying all the filters
     sum = 0;
     for row = 1:imgSize
         for col = 1:imgSize
            sum =  sum + abs(convolutedImage(row, col));
         end
     end
     averageEnergy = (sum/(128*128));
end

function subMeanTrain = SubtractMeanTrain(trainImg)
    
    %Calculate average pixel value of each image and subtract each pixel by
    %it's average
    subMeanTrain = trainImg;
    
    % reduce the illuminaton of each image class
    for i = 1:9
       %img = trainImg{i};
       img = trainImg(:,:,i);
       pixel = 0;
       
       % get the sum of each image class
       for row = 1:128
          for col = 1:128
              pixel = pixel + img(row,col);
          end
       end
       
       average = (pixel/(128*128)); % find the average
       % then subtract it from each image
       for row = 1:128
          for col = 1:128
              subMeanTrain(row,col,i) = img(row, col)- average;
          end
       end
    end
end

function subMeanTest = SubtractMeanTest(trainImg)
    
    %Calculate average pixel value of each image and subtract each pixel by
    %it's average
    subMeanTest = trainImg;
    
    % reduce the illuminaton of each image class
    for i = 1:12
       img = trainImg(:,:,i);
       pixel = 0;
       
       % get the sum of each image class
       for row = 1:128
          for col = 1:128
              pixel = pixel + img(row,col);
          end
       end
       
       average = (pixel/(128*128)); % find the average
       % then subtract it from each image
       for row = 1:128
          for col = 1:128
              subMeanTest(row,col,i) = img(row, col)- average;
          end
       end
    end
end

%  Reducing features from 25 to 15 dimensions (36*15 ) 
function reducedfeatureTrainMatrix = reduceFeaturesTrain(featureTrainMatrix)
    reducedfeatureTrainMatrix = zeros(36, 15);
    for i=1:36
        reducedfeatureTrainMatrix(i,1)= featureTrainMatrix(i,1); %LL
        
        reducedfeatureTrainMatrix(i,2)= (featureTrainMatrix(i,2) + featureTrainMatrix(i,6))/2;  %LE/EL
        reducedfeatureTrainMatrix(i,3)= (featureTrainMatrix(i,8) + featureTrainMatrix(i,12))/2; %LS/SL
        
        reducedfeatureTrainMatrix(i,4)= featureTrainMatrix(i,7);  %EE
        
        reducedfeatureTrainMatrix(i,5)= (featureTrainMatrix(i,3) + featureTrainMatrix(i,11))/2; %LW/WL
        reducedfeatureTrainMatrix(i,6)= (featureTrainMatrix(i,9) + featureTrainMatrix(i,17))/2; %LR/RL
        
        reducedfeatureTrainMatrix(i,7)= featureTrainMatrix(i,13); %SS
        
        reducedfeatureTrainMatrix(i,8)= (featureTrainMatrix(i,4) + featureTrainMatrix(i,16))/2;  %WR/RW
        reducedfeatureTrainMatrix(i,9)= (featureTrainMatrix(i,10) + featureTrainMatrix(i,22))/2; %ES/SE
        
        reducedfeatureTrainMatrix(i,10)= featureTrainMatrix(i,19); %WW
        
        reducedfeatureTrainMatrix(i,11)= (featureTrainMatrix(i,5) + featureTrainMatrix(i,21))/2;  %EW/WE
        reducedfeatureTrainMatrix(i,12)= (featureTrainMatrix(i,14) + featureTrainMatrix(i,18))/2; %ER/RE
        
        reducedfeatureTrainMatrix(i,13)= featureTrainMatrix(i,25); %RR
        
        reducedfeatureTrainMatrix(i,14)= (featureTrainMatrix(i,20) + featureTrainMatrix(i,24))/2; %SW/WS
        reducedfeatureTrainMatrix(i,15)= (featureTrainMatrix(i,15) + featureTrainMatrix(i,23))/2; %SR/RS
    end
end

 % Reducing features from 25 to 15 dimensions (36*15 )
function reducedFeatureMatrix = reduceFeaturesTest(featureMatrix)
    reducedFeatureMatrix = zeros(12, 15);
    for i=1:12
        reducedFeatureMatrix(i,1)= featureMatrix(i,1);
        
        reducedFeatureMatrix(i,2)= (featureMatrix(i,2) + featureMatrix(i,6))/2;
        reducedFeatureMatrix(i,3)= (featureMatrix(i,8) + featureMatrix(i,12))/2;
        
        reducedFeatureMatrix(i,4)= featureMatrix(i,7);
        
        reducedFeatureMatrix(i,5)= (featureMatrix(i,3) + featureMatrix(i,11))/2;
        reducedFeatureMatrix(i,6)= (featureMatrix(i,9) + featureMatrix(i,17))/2;
        
        reducedFeatureMatrix(i,7)= featureMatrix(i,13);
        
        reducedFeatureMatrix(i,8)= (featureMatrix(i,4) + featureMatrix(i,16))/2;
        reducedFeatureMatrix(i,9)= (featureMatrix(i,10) + featureMatrix(i,22))/2;
        
        reducedFeatureMatrix(i,10)= featureMatrix(i,19);
        
        reducedFeatureMatrix(i,11)= (featureMatrix(i,5) + featureMatrix(i,21))/2;
        reducedFeatureMatrix(i,12)= (featureMatrix(i,14) + featureMatrix(i,18))/2;
        
        reducedFeatureMatrix(i,13)= featureMatrix(i,25);
        
        reducedFeatureMatrix(i,14)= (featureMatrix(i,20) + featureMatrix(i,24))/2;
        reducedFeatureMatrix(i,15)= (featureMatrix(i,15) + featureMatrix(i,23))/2;
        
        reducedFeatureMatrix(i,14)= (featureMatrix(i,20) + featureMatrix(i,24))/2;
        reducedFeatureMatrix(i,15)= (featureMatrix(i,15) + featureMatrix(i,23))/2;
    end
end


% peform feature reduction here
% using PCA 
function features_pca = PCATrain(featureTrainMatrix)

    data_std = zscore(featureTrainMatrix); % get the zscore
    
    %Need the covariance matrix
    sigma = (1/36)*(data_std')*(data_std);  % square filter response i.e data_std
    
    % get the eigen values for report purpose
    eigenVal = eig(sigma); 
    
    %apply SVD to sigma
    [U, ~, ~] = svd(sigma);
    Ured = U(:,1:3);
    
    %finding k dimension subspace here k = 3
    subspace = Ured' * data_std' ;
    features_pca = subspace';
    
    x = subspace(1,:)'; %First principal component
    y = subspace(2,:)'; %Second principal component
    z = subspace(3,:)'; %third principal component 
    
    %Seggregating according to labels
    x1 = x(1:9,:); y1 = y(1:9,:);z1 = z(1:9,:);
    x2 = x(10:18,:);y2 = y(10:18,:); z2 = z(10:18,:);
    x3 = x(19:27,:); y3 = y(19:27,:); z3 = z(19:27,:);
    x4 = x(28:36,:);y4 = y(28:36,:); z4 = z(28:36,:);
    
    %Plotting
    figure(2);
    scatter3(x1,y1,z1, 'r'); hold on;
    scatter3(x2, y2, z2, 'g');hold on;
    scatter3(x3, y3, z3, 'b'); hold on;
    scatter3(x4, y4, z4, 'c'); hold off
    xlabel('First Principal component');
    ylabel('Second Principal component');
    zlabel('Third Principal component');
    title('PCA for Training images');
end


% perform feature reduction here
% using PCA
function features_pca = PCATest(featureMatrix)

    data_std = zscore(featureMatrix); % get zscore
    
    %Covariance matrix
    sigma = (1/36)*(data_std')*(data_std); % then the co-variance  % square filter response i.e data_std
    
    % get the eigen values for report purpose
    eigenValtest = eig(sigma); 
    
    %apply SVD to sigma
    [U, S, V] = svd(sigma);
    Ured = U(:,1:3);
    
    %finding k dimension subspace here k = 3
    subspace = Ured' * data_std' ;
    features_pca = subspace';
    
    x = subspace(1,:)'; %First principal component
    y = subspace(2,:)'; %Second principal component
    z = subspace(3,:)'; %third principal component 
    
    %Plotting
    figure(3);scatter3(x,y,z, 'r'); hold on;
    xlabel('First Principal component');
    ylabel('Second Principal component');
    zlabel('Third Principal component');
    title('Test images');
end
