Building the LSF Helper Library for Java (LSFJavaHelper)

Prerequisites:
1) The Eclipse development environment with the Android SDK and NDK plugins.
2) The AllJoyn Android SDK.
3) The LSFJavaHelper library project depends on the LSFJava bindings project.
   Make sure both are imported into your Eclipse workspace.

Three path variables in Eclipse are required to build the LSFJavaHelper project.
The following steps will show you how to add the required variables to Eclipse.

1) From the top menu bar, select Window -> Preferences.
2) On the left side, expand the General -> Workspace menus and select Linked Resources.
3) Click "New" to create a new variable.
4) Enter "ALLJOYN_HOME" for the variable name.
5) For location, click the "Folder..." button.
6) Locate the root folder of the AllJoyn Android SDK and set the path of the ALLJOYN_HOME
   variable to "<path_to_android_AJ_SDK_root_folder>/alljoyn_android/core/alljoyn-14.02.00-rel"
7) Click OK and then OK again. You should see your newly created variable.
8) Repeat steps 3-7 to create another variable named "NDK_HOME" that points to the root folder
   of the Android NDK (e.g., "<path_to_downloads_folder>/android-ndk-r9c").
9) Repeat steps 3-7 to create another variable named "ANDROID_HOME" that points to the root folder
   of the Android SDK (e.g., "<path_to_downloads_folder>/adt-bundle-linux-x86_64-20131030/sdk")

