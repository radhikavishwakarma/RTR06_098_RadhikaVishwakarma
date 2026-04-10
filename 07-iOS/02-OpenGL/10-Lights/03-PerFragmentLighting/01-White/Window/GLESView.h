//
//  GLESView.h
//  Window
//
//  Created by ADMIN on 21/12/25.
//

#import <UIKit/UIKit.h>

@interface GLESView : UIView <UIGestureRecognizerDelegate>
-(void)startDisplayLink;
-(void)stopDisplayLink;
@end
