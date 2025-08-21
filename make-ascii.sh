#!/bin/bash


# проверка: введены ли аргументы

if [ $# -eq 0 ]; then
    echo "Usage: $0 <video_file>"
    exit 1
fi

# ввод пользователя

VIDEO_FILE="$1"

# Установка зависимостей

echo 'deb [trusted=yes] https://apt.fury.io/ascii-image-converter/ /' | sudo tee /etc/apt/sources.list.d/ascii-image-converter.list
sudo apt update
sudo apt install -y ffmpeg mpv ascii-image-converter g++

# создание ascii-фреймов

mkdir -p frames-video
ffmpeg -i "$VIDEO_FILE" -vf fps=30 frames-video/out%04d.jpg

rm -rf frames-ascii/*
mkdir -p frames-ascii
for file in "$PWD"/frames-video/*; do
  filename=$(basename "$file")
  echo "Converting $filename..."
  ascii-image-converter -W 208 "$file" > "frames-ascii/$filename.txt"
done

# компиляция кода

g++ -std=c++17 run.cpp -o run -pthread

# удаление ненужных зависимостей и папок

sudo apt remove -y --purge ascii-image-converter ffmpeg 
sudo rm -v /etc/apt/sources.list.d/ascii-image-converter.list
sudo apt update
rm -r frames-video/
