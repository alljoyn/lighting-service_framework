Building the LSF Tutorial App for Android (LSFTutorial)

Prerequisites:

 1) "Eclipse ADT with the Android SDK" http://developer.android.com/sdk/index.html

Four path variables in Eclipse are used to reference the external
binaries needed to build the LSFTutorial project. The steps to import
the project and set up the necessary path variables are as follows:

 1) Start Eclipse, and on the top menu bar select File -> Import...
 2) In the Import dialog, open the General category and choose Existing
    Projects into Workspace
 3) Browse to the LSFTutorial installation directory, and complete
    the project import.
 4) After importing, open the Preferences dialog (Window -> Preferences or
    ADT -> Preferences). Note this is not the same as the Project Properties dialog.
 5) On the left side of the Preferences dialog:
    5.1) expand the General section
    5.2) expand the Workspace section
    5.3) select the Linked Resources option
 6) Click "New" to create a new variable.
 7) Enter "ALLJOYN_HOME" for the variable name.
 8) For location, click the "Folder..." button.
 9) Navigate to the installation directory, and set the "ALLJOYN_HOME"
    variable to "<lsf_tutorial_install_dir>/android/Libraries/AllJoyn"
10) Click OK and then OK again. You should see your newly created variable.
11) Repeat steps 6-10 to create another variable named "LSF_JAVA_HOME" that's
    set to "<lsf_tutorial_install_dir>/android/Libraries/LSFJava"
12) Repeat steps 6-10 to create another variable named "LSF_JAVA_HELPER_HOME"
    that's set to "<lsf_tutorial_install_dir>/android/Libraries/LSFJavaHelper"
13) Repeat steps 6-10 to create another variable named "ANDROID_HOME" that's
    set to "<android_sdk_installation_dir>" (if you are using Eclipse with
    ADT, the Android SDK will be under the ADT installation directory
    <adt_install_dir>/sdk).
14) Right-click on the LSFTutorial project, and click "Refresh".
15) With an Android device connected to the computer, right-click on the
    project again and select Run As -> Android Application

