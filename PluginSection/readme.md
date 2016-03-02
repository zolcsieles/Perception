#VIREIO PERCEPTION Plugins

Created using [Visual C++ 2013 Express for Windows Desktop] 
(VS2012 project files also available)

####Environment Variables

    * FREESPACE      : The folder for libfreespace (Hillcrest Labs SDK).
    * DXSDK_DIR      : Your DirectX SDK folder (already set automatically by the June 2010 DirectX SDK).
    * OVRSDKROOT     : Your Oculus SDK root directory.
##### for OSVR-nodes:
    * BOOST_ROOT     : Boost needed for the OSVR render manager
    * OSVRRENDERMANAGER_INCLUDE_DIRS       : OSVR Render Manager include x64.
    * OSVRRENDERMANAGER_INCLUDE_DIRS_Win32 : OSVR Render Manager include Win32.
    * OSVRRENDERMANAGER_LIBRARIES          : OSVR Render Manager lib x64.
    * OSVRRENDERMANAGER_LIBRARIES_Win32    : OSVR Render Manager lib Win32.
    * osvr_DIR       : Your OSVR core directory. (x64)
    * osvr_DIR_Win32 : Your OSVR core directory (Win32)
    

(use the command "setx -m osvr_DIR %~dp0" - f.i. OSVR core x64 - as administrator to quickly set an environment variable for new directories)

####Required dlls

For OSVR copy following dlls (from the OSVR core, differentiate between 32 or 64 bit) to the games executeable folder :
    
    * osvrClient.dll
    * osvrClientKit.dll
    * osvrCommon.dll
    * osvrUtil.dll

####Visual C++ Redistributables

Also, take care to have the latest Visual Studio Redistributables installed, download at https://www.microsoft.com/ . 

####Plugin List

    * Vireio Constructor - DevNr 2006 - PluginNr 32
    * Vireio Stereo Splitter - DevNr 2006 - PluginNr 64
    * Vireio Stereo Presenter - DevNr 2006 - PluginNr 65
    * Vireio Cinema - DevNr 2006 - PluginNr 128
    * Vireio Oculus Tracker - DevNr 2006 - PluginNr 256
    * Vireio Oculus Renderer - DevNr 2006 - PluginNr 258
    * Vireio Oculus Direct Mode - DevNr 2006 - PluginNr 259
    * Vireio OSVR Tracker - DevNr 2006 - Plugin Nr 288

####Known Aquilinus Plugins aside this repository:

    * VRBoost - DevNr 2006 - PluginNr 512