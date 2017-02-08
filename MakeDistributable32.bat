del /s /q Demo\*.*
rd Demo
md Demo
md Demo\Licenses
md Demo\Licenses\ffmpeg
md Demo\Data

copy HAPI_APP\HAPI_SPRITES\Libs32 Demo\
copy HAPI_APP\HAPI_SPRITES\Licenses Demo\Licenses
copy HAPI_APP\HAPI_SPRITES\Licenses\ffmpeg Demo\Licenses\ffmpeg
copy HAPI_APP\Data Demo\Data\
copy Release\HAPI_APP.exe Demo\Demo32.exe