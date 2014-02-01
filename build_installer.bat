

@set "wix_path=C:\Program Files\WiX Toolset v3.8\bin\"

cd bin

"%wix_path%candle.exe" WinInfo.wxs
"%wix_path%light.exe" -ext WixUIExtension WinInfo.wixobj

cd ..

pause