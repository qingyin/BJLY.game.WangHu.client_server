#import <UIKit/UIKit.h>
#import "WXApi.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, WXApiDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;

@end

