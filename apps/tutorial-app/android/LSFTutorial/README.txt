Building the LSF Tutorial App for Android (LSFTutorial)

Prerequisites:

 1) The Eclipse development environment with the Android SDK plugins.

Four path variables in Eclipse are used to reference the external
binaries needed to build the LSFTutorial project. The steps to import
the project and set up the necessary path variables are as follows:

 1) Extract the LSFTutorial release package into a convenient directory,
    and note the path to the installation.
 2) Start Eclipse, and on the top menu bar select File -> Import...
 3) In the Import dialog, open the General category and choose Existing
    Projects into Workspace
 4) Browse to the LSFTutorial installation directory, and complete
    the project import.
 5) After importing, go back to the top menu bar in Eclipse and select
    Window -> Preferences.
 6) On the left side of the Preferences dialog, expand the menu
    General -> Workspace, and then select the Linked Resources option.
 7) Click "New" to create a new variable.
 8) Enter "ALLJOYN_HOME" for the variable name.
 9) For location, click the "Folder..." button.
10) Navigate to the installation directory, and set the "ALLJOYN_HOME"
    variable to "<lsf_tutorial_install_dir>/android/Libraries/AllJoyn"
11) Click OK and then OK again. You should see your newly created variable.
12) Repeat steps 7-11 to create another variable named "ANDROID_HOME" that's
    set to "<lsf_tutorial_install_dir>/android/Libraries/Android"
13) Repeat steps 7-11 to create another variable named "LSF_JAVA_HELPER_HOME"
    that's set to "<lsf_tutorial_install_dir>/android/Libraries/LSFJavaHelper"
14) Repeat steps 7-11 to create another variable named "LSF_JAVA_HOME" that's
    set to "<lsf_tutorial_install_dir>/android/Libraries/LSFJava"

