//
//  main.m
//  Window
//
//  Created by ADMIN on 21/12/25.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

// Entry-point Function
int main(int argc, char * argv[])
{
    // Code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

    // Get AppDelegate class name in the form of String
    NSString *delegateClassName = NSStringFromClass([AppDelegate class]);

    int result = UIApplicationMain(argc, argv, nil, delegateClassName);
    
    [pool release];
    
    return (result);
}
