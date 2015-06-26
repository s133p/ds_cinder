DS Cinder
=========
BigWorld framework implemented in Cinder.

----------

Installation
------------
This version of DS Cinder requires **Visual Studio 2010 + SP1 installed.**

Download [cinder 0.8.4](libcinder.org/release/cinder_0.8.4_vs2010.zip)
Backup download [mirror](http://update.downstreamdev.com/ds/cinder/cinder_0.8.4_vc2010.zip)

 -  You'll need **two** environment variables set:
   - `CINDER` should point to the 0.8.4 cinder expanded distributable. e.g:

     ```Batchfile
     setx CINDER %USERPROFILE%\code\cinder_0.8.4_vc2010
     ```

   - `DS_PLATFORM` should point to this repository. e.g:

     ```Batchfile
     setx DS_PLATFORM %USERPROFILE%\code\ds_cinder
     ```

 -  You should currently start by copying the example/starter as the basis for your new app.  If you do this, everything will be setup correctly and ready to go.  You can stop reading right here! **(if you DON'T do this step and copy the starter project, then you will need to manually set up the new solution yourself.)**


----------

 
Property sheets
---------------

We are using **Visual Studio property sheets** to manage basic settings like libraries and include paths. The property sheets are located in `ds_cinder/vc10/PropertySheets`, `PlatformSetup.props` (release config) and `PlatformSetup_d.props` (debug config).  What this means is that in the future, all include paths, libraries, and library include paths should be added to these property sheets, so that apps will inherit any changes.  All of these values are specified in the property sheets Common Properties->User Macros page.

 1. Open the Property Manager (`View -> Property Manager`).
 2. For each project, in the Debug config add `PlatformSetup_d.props` and in release add `PlatformSetup.props` (in both cases, right  3. click the config and select Add Existing Property Sheet...).
 4. In `C/C++ -> General` set `Additional Include Directories` to `$(platform_include_d)"` (in debug) or `"$(platform_include)` (in release)
 5. In `Linker -> General` set `Additional Library Directories` to `$(platform_lib_include_d);%(AdditionalLibraryDirectories)` (in debug) or `$(platform_lib_include);%(AdditionalLibraryDirectories)` (in release)
 6. In `Linker -> Input` set Additional Dependencies to `$(platform_lib_d);%(AdditionalDependencies)` (in debug) or `$(platform_lib);%(AdditionalDependencies)` (in release)
 7. In `Linker -> Input` set `Ignore Specific` to `$(platform_lib_ignore_d)` (in debug) or `$(platform_lib_ignore)` (in release)


----------

DLLs
----

We are currently making use of ZeroMQ, which requires a DLL to be located in the same folder as the executable.  To automatically copy over the correct files, add a build step.

 1. Open the properties on your project.
 2. Go to Configuration `Properties -> Build Events -> Post Build Event`
 3. Set Command line to: `copy /Y "$(DS_PLATFORM)\lib\zmq\lib\*" "$(OutDir)"`
 4. Set Description to something like `Copying ZMQ`


----------


For using markdown
------------------

Add AWESOMIUM to the dscinder defines and it will compile the awesomium stuff in. Then copy the files in the "for_markdown" folder to there correct locations.


----------


To compile with GSTREAMER
-------------------------

Install **Gstreamer 1.2.3** runtime and development files available [from the gstreamer site][1]. Use the x86, not x64 versions.
  - [Runtime installer](http://gstreamer.freedesktop.org/data/pkg/windows/1.2.3/gstreamer-1.0-x86-1.2.3.msi)
  - [Development files installer](http://gstreamer.freedesktop.org/data/pkg/windows/1.2.3/gstreamer-1.0-devel-x86-1.2.3.msi)

----------


Troubleshooting installation
--------------------------------

 - If you get errors for `xaudio.h`: install [latest DirectX SDK][2]
 - If you get errors for `Gstreamer` and you **DO** know that you have installed the correct version (**1.2.3**), make sure you installed it on root level of drive `c:\` (make a symbolic link to wherever else you might have installed it incorrectly)
 - If you get errors of missing `Boost cstdint` headers: make sure your cinder distribution does include Boost!
 - If you get `LNK1123: failure during conversion to COFF: file invalid or corrupt'`: [Install SP1 for your Visual Studio 2010][3]


  [1]: http://gstreamer.freedesktop.org/data/pkg/windows/
  [2]: http://lmgtfy.com/?q=directx%20sdk%20download
  [3]: http://stackoverflow.com/a/10890428/1055628
