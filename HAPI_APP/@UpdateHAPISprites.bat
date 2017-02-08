del /s /q HAPI_SPRITES\*.*
rd HAPI_SPRITES
md HAPI_SPRITES
md HAPI_SPRITES\Licenses
md HAPI_SPRITES\Licenses\ffmpeg
md HAPI_SPRITES\Libs32
md HAPI_SPRITES\Libs64
md HAPI_SPRITES\HAPI
md HAPI_SPRITES\glm
md HAPI_SPRITES\glm\glm
md HAPI_SPRITES\glm\glm\detail
md HAPI_SPRITES\glm\glm\gtc
md HAPI_SPRITES\glm\glm\gtx
md HAPI_SPRITES\glm\glm\simd
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\ HAPI_SPRITES\
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\HAPI HAPI_SPRITES\HAPI
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\Licenses HAPI_SPRITES\Licenses
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\Licenses\ffmpeg HAPI\Licenses\ffmpeg
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\Libs32 HAPI_SPRITES\Libs32
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\Libs64 HAPI_SPRITES\Libs64
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\glm HAPI_SPRITES\glm
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\glm\glm HAPI_SPRITES\glm\glm
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\glm\glm\detail HAPI_SPRITES\glm\glm\detail
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\glm\glm\gtc HAPI_SPRITES\glm\glm\gtc
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\glm\glm\gtx HAPI_SPRITES\glm\glm\gtx
copy ..\..\..\HAPI_2D_2016\HAPI_SPRITES\glm\glm\simd HAPI_SPRITES\glm\glm\simd