//
//  Application.hpp
//  Vulkan
//
//  Created by shiva.pandiri on 11/5/22.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>

class KSApplication {

public:
    
    KSApplication();
    
    virtual ~KSApplication(){}
    
    //TODO Overide? and sepearate for child;
   // KSApplication(android_app *papp) ;
    //friends
   // friend AndroidEvents;
    //friend CustomEvents;

public:

    virtual void run();
    //AAssetManager *getAssetManager();
    //int getAssetFD(const char* assetLoc);
    //IKSStream* _openAsset(const char* assetPath);

    //UI

    //View* getContentView() const;

protected:

    //createUI and Listeners and set in Renderer
    virtual void onWindowInit(){};
    virtual void onDestroy(){};
   /* virtual void onCreate();
    virtual void onStart();
    virtual void onResume();
    virtual void onPause();
    virtual void onStop();
   

    
    virtual void onWindowResized();
    virtual void onWindowRedrawNeeded();
    virtual void onWindowTermination();
    virtual void onContentRectChanged();

    virtual void onLowMemory();
    virtual void onFocusLost();
    virtual void onFocusGained();
    virtual void onConfigChanged();
    virtual void onSaveState();
    virtual void onInputChange();
    void onScreenRotation();
    */

    //Public?
    //void setContentView(const View *content);

protected:
   // AAsset *getAsset(const char* assetPath);
private:
   // bool updateDisplayMetrics();

public:
    //
    //VulkanContext vulkanContext;
    // GLuint uiProgram;
   // bool bWindowInit=false,bGraphicsInit=false,bAppFirstOpen=true;
    //int screenOrientation  = ACONFIGURATION_ORIENTATION;

    //Activity call backs;Protected?????

    //onCreate is the main method;
protected:
   // DisplayMetrics displayMetrics{0};

private:
   // android_app *app;
    //GLUIRenderer renderer;
   // GLContext glContext;
  //  KSWindow window;

   // CustomEvents *customEvents;////////////

    const char* APPTAG = "KSApplication";//check TS


};



#endif /* Application_hpp */
