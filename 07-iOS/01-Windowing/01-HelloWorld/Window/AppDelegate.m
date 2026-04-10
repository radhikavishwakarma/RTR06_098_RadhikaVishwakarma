//
//  AppDelegate.m
//  Window
//
//  Created by ADMIN on 21/12/25.
//

#import "AppDelegate.h"

@implementation AppDelegate

-(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Code
    return(YES);
}

-(UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options
{
    // Code
    return([[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role]);
}

-(void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions
{
    // Code
}

-(void)dealloc
{
    // Code
    [super dealloc];
}

@end
