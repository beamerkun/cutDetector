#!/bin/sh
# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE

if [ $# -ne 1 ]; then
  echo "pass only one argument!"
  exit 1
fi

if [ ! -d $1 ]; then
  echo "pass proper directory!"
  exit 1
fi

libraries=("bin/libopencv_core2411.dll" "bin/libopencv_highgui2411.dll" "bin/libopencv_imgproc2411.dll")
ffmpeg="../sources/3rdparty/ffmpeg/opencv_ffmpeg.dll"
ffmpeg_ver="opencv_ffmpeg2411.dll"

for i in ${libraries[@]}; do
  file="$OPENCV_DIR/$i" 
  if [ -e $file ]; then
    cp $file $1
    echo "$i copied successfully!"
  else
    echo "$file not found!"
  fi
done

ffmpeg_path=$OPENCV_DIR/$ffmpeg 
if [ -e $ffmpeg_path ]; then
  cp $ffmpeg_path $1/$ffmpeg_ver
  echo "ffmpeg copied successfully!"
else
  echo "ffmpeg not found!"
fi
