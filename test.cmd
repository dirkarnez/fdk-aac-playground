@REM run as Administrator
@echo off
cd /d %~dp0
set DOWNLOADS_DIR=%USERPROFILE%\Downloads
set DOWNLOADS_DIR_LINUX=%DOWNLOADS_DIR:\=/%

SET PATH=^
%DOWNLOADS_DIR%\ffmpeg-6.0-full_build-shared\ffmpeg-6.0-full_build-shared\bin;^
%SystemRoot%\System32;

ffmpeg -i input.mp3 -acodec pcm_s16le -f s16le input.pcm 

pause