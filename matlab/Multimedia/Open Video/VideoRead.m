%/*************************************************************************
%/* File Name: VideoRead.m
%/*************************************************************************

close all
clear

%%% The video name
vName = 'garden_sif';

%%% The video format
%%% Use the '.avi' file format for uncompressed '.avi'
vFormat = '.avi';

%%% The complete video name
%%% Make sure the path is correct
%%% By default, the video should be in the same folder as this '.m' file
videoName = strcat(vName,vFormat);


%%% Read the video into object obj
obj = VideoReader(videoName)



%%% The number of video frames
nframes = get(obj,'NumberOfFrames');


%%% Read the 'FrameId' video frame
FrameId = 1
tempVideoFrame = read(obj,FrameId);

%%% Display the first video frame
figure(1)
imshow(tempVideoFrame)

%%% The height and width of the first video frame
%%% colorN the number of color components only used for the RGB color video
[height width colorN] = size(tempVideoFrame);

%%% Convert the RGB space to Ycbcr
Iycbcr = rgb2ycbcr(tempVideoFrame);
SIycbcr = Iycbcr;
CI = Iycbcr(:,:,1);

%%% Display the Y component of the video frame
figure(2)
imshow(CI)

%%% Convert to double precision so that you can modify 'CI' to change the
%%% video frame
CI = double(CI);











