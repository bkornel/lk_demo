@echo off

set outDir=..\bin
set to3rdParty=..\..\3rdparty

set opencvBinDir=%to3rdParty%\opencv-3.4.0\Windows\x64\vc15\bin
set opencvFiles=(opencv_core340 opencv_ffmpeg340_64 opencv_highgui340 opencv_imgcodecs340 opencv_imgproc340 opencv_video340 opencv_videoio340)

for %%i in %opencvFiles% do (
	xcopy %opencvBinDir%\%%i.dll %outDir%  /D /Y
	xcopy %opencvBinDir%\%%i.pdb %outDir%  /D /Y
)
