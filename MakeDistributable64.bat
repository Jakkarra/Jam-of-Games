del /s /q Demo\*.*
rd Demo
md Demo
md Demo\Licenses
md Demo\Licenses\ffmpeg
md Demo\Data

copy HAPI_APP\HAPI_SPRITES\Libs64 Demo\
copy HAPI_APP\HAPI_SPRITES\Licenses Demo\Licenses
copy HAPI_APP\HAPI_SPRITES\Licenses\ffmpeg Demo\Licenses\ffmpeg
copy HAPI_APP\Data Demo\Data\
copy x64\Release\HAPI_APP.exe Demo\Demo64.exe

echo ho