mkdir out

cmake -A x64 -B .\rtaudio_build -S .\rtaudio\
cmake --build .\rtaudio_build\ --config Release
cmake --install .\rtaudio_build\ --config Release --prefix .\out\


cmake -A x64 -B .\pthread_build -S .\pthread-win32\
cmake --build .\pthread_build\ --config Release
cmake --install .\pthread_build\ --config Release --prefix .\out\


xcopy /S /I /Q /Y /F .\out\lib\pthreadVC3.lib .\airspyone_host\libairspy\vc\pthreadVC2.lib
xcopy /S /I /Q /Y /F .\out\lib\pthreadVC3.lib .\airspyhf\libairspyhf\pthreadVC2.lib

MSBuild.exe .\libusb\msvc\libusb.sln /p:Configuration=Release  /p:Platform=x64
xcopy /S /I /Q /Y /F .\libusb\build\v143\x64\Release\lib\*.lib .\out\lib\ 

xcopy .\libusb\libusb\*.h .\out\include /S /Y


xcopy /S /I /Q /Y /F .\out\lib\libusb-1.0.lib .\airspyone_host\libairspy\vc\libusb-1.0.lib
xcopy /S /I /Q /Y /F .\out\lib\libusb-1.0.lib  .\airspyhf\libairspyhf\libusb-1.0.lib 


set relativeIncPath=.\out\include

for %%i in (%relativeIncPath%) do set absoluteIncPath=%%~fi

set relativeLibPath=.\out\lib

for %%i in (%relativeLibPath%) do set absoluteLibPath=%%~fi


set absoluteLibUsbPath=%absoluteLibPath%\libusb-1.0.lib

:: Convert backslashes to forward slashes
set absoluteLibUsbPath=%absoluteLibUsbPath:\=/%



MSBuild.exe /p:Configuration=Release  /p:Platform=x64 /p:PlatformToolset=v143 .\airspyone_host\libairspy\vc\airspy_2019.sln /p:IncludePath="%absoluteIncPath%"

xcopy     /S /I /Q /Y /F .\airspyone_host\libairspy\x64\Release\ .\out\airspy\
xcopy .\airspyone_host\libairspy\src\*.h .\out\airspy\

MSBuild.exe /p:Configuration=Release  /p:Platform=x64 /p:PlatformToolset=v143 .\airspyhf\libairspyhf\airspyhf.sln  /p:IncludePath="%absoluteIncPath%"

xcopy   /S /I /Q /Y /F .\airspyhf\libairspyhf\x64\Release\ .\out\airspyhf\
xcopy .\airspyhf\libairspyhf\src\*.h .\out\airspyhf\

cmake -A x64 -B .\fftw_build -S ..\core\submodules\fftw\ -DENABLE_FLOAT=ON -DENABLE_SSE=ON
cmake --build .\fftw_build\ --config Release
cmake --install .\fftw_build\ --config Release --prefix .\out\

cmake -A x64 -B .\hackrf_build -S .\hackrf\host -DLIBUSB_INCLUDE_DIR="%absoluteIncPath%" -DLIBUSB_LIBRARIES="%absoluteLibUsbPath%" -DTHREADS_PTHREADS_INCLUDE_DIR="%absoluteIncPath%" -DTHREADS_PTHREADS_WIN32_LIBRARY="%absoluteLibPath%\pthreadVC3.lib" -DFFTW_INCLUDES="%absoluteIncPath%" -DFFTW_LIBRARIES="%absoluteLibPath%\fftw3f.lib" 
cmake --build .\hackrf_build\ --config Release
cmake --install .\hackrf_build\ --config Release --prefix .\out\

cmake -A x64 -B .\librtlsdr_build -S .\librtlsdr\ -DLIBUSB_INCLUDE_DIRS="%absoluteIncPath%" -DLIBUSB_LIBRARIES="%absoluteLibUsbPath%" -DTHREADS_PTHREADS_INCLUDE_DIR="%absoluteIncPath%" -DTHREADS_PTHREADS_LIBRARY="%absoluteLibPath%\pthreadVC3.lib" 
cmake --build .\librtlsdr_build\ --config Release
cmake --install .\librtlsdr_build\ --config Release --prefix .\out\


cmake -A x64 -B .\libxml_build -S .\libxml2\ -DLIBXML2_WITH_ICONV=OFF -DLIBXML2_WITH_PYTHON=OFF
cmake --build .\libxml_build\ --config Release 
cmake --install .\libxml_build\ --config Release --prefix .\out\


cmake -A x64 -B .\zstd_build -S ..\core\submodules\zstd\build\cmake
cmake --build .\zstd_build\ --config Release
cmake --install .\zstd_build\ --config Release --prefix .\out\

set absoluteZstdPath=%absoluteLibPath%\zstd.lib
for %%i in (%relativeZstdPath%) do set absoluteZstdPath=%%~fi


cmake -A x64 -B .\libiio_build -S .\libiio\ -DCMAKE_PREFIX_PATH="%absoluteLibPath%\cmake\libxml2-2.13.3" -DWITH_XML_BACKEND=ON  -DWITH_ZSTD=ON -DWITH_USB_BACKEND=ON -DLIBUSB_INCLUDE_DIR="%absoluteIncPath%" -DLIBUSB_LIBRARIES="%absoluteLibUsbPath%" -DLIBZSTD_INCLUDE_DIR="%absoluteIncPath%" -DLIBZSTD_LIBRARIES="%absoluteZstdPath%" 
cmake --build .\libiio_build\ --config Release
cmake --install .\libiio_build\ --config Release --prefix .\out\

set absoluteLibiioLibraries = %absoluteLibPath%\libiio.lib
cmake -A x64 -B .\libad9361_build -S .\libad9361-iio\ -DLIBIIO_LIBRARIES="%absoluteLibiioLibraries%"  -DLIBIIO_INCLUDEDIR="%absoluteIncPath%"
cmake --build .\libad9361_build\ --config Release
cmake --install .\libad9361_build\ --config Release --prefix .\out\


cmake -A x64 -B .\boost_build -S .\boost\
cmake --build .\boost_build\ --config Release
cmake --install .\boost_build\ --config Release --prefix .\out\

set absoluteBoostIncludePath=%absoluteIncPath%\boost-1_86
set absoluteBoostLibPath=%absoluteLibPath%\

REM cd boost\tools\build

cmake -A x64 -B .\uhd_build -S .\uhd\host\ -DENABLE_TESTS=OFF  -DLIBUSB_LIBRARIES="%absoluteLibUsbPath%"  -DLIBUSB_INCLUDE_DIRS="%absoluteIncPath%"  -DBoost_INCLUDE_DIR="%absoluteBoostIncludePath%" -DBoost_LIBRARY_DIR="%absoluteBoostLibPath%" -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=ON 
cmake --build .\uhd_build\ --config Release
cmake --install .\uhd_build\ --config Release --prefix .\out\

