#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include <screen/StartScreen.h>

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 800);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("HelloCpp", cocos2d::Rect(0, 0, designResolutionSize.width
																	  , designResolutionSize.height));
		glview->setFrameZoomFactor(1.5);
#else
		glview = GLViewImpl::create("HelloCpp");
#endif
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	director->setDisplayStats(!false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height
									, ResolutionPolicy::NO_BORDER);
	auto frameSize = glview->getFrameSize();
	// if the frame's height is larger than the height of medium size.
	if (frameSize.height > mediumResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height,
												largeResolutionSize.width / designResolutionSize.width));
	}
		// if the frame's height is larger than the height of small size.
	else if (frameSize.height > smallResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height,
												mediumResolutionSize.width / designResolutionSize.width));
	}
		// if the frame's height is smaller than the height of medium size.
	else
	{
		director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height,
												smallResolutionSize.width / designResolutionSize.width));
	}

	register_all_packages();

	// create a scene. it's an autorelease object
	auto scene = StartScreen::create();

	// run
	director->runWithScene(scene);

	cocos2d::UserDefault::getInstance()->setIntegerForKey("User1", 120);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User2", 210);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User3", 80);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User4", -20);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User5", 15);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User6", 100);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User7", 30);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("User8", -10);
	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be paused
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
