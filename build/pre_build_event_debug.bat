@echo off

set outDir=..\bin
set to3rdParty=..\..\3rdparty

set opencvBinDir=%to3rdParty%\opencv-3.4.0\Windows\x64\vc15\bin
set opencvFiles=(opencv_core340d opencv_ffmpeg340_64 opencv_highgui340d opencv_imgcodecs340d opencv_imgproc340d opencv_video340d opencv_videoio340d)

for %%i in %opencvFiles% do (
	xcopy %opencvBinDir%\%%i.dll %outDir%  /D /Y
	xcopy %opencvBinDir%\%%i.pdb %outDir%  /D /Y
)
