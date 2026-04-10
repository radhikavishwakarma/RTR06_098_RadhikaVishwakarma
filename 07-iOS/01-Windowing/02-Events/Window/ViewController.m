//
//  ViewController.m
//  Window
//
//  Created by ADMIN on 21/12/25.
//

#import "ViewController.h"

#import "MyView.h"

@implementation ViewController
{
    MyView *view;
}

-(void)loadView
{
    // Code
    view = [[MyView alloc]initWithFrame:CGRectZero];
    [self setView:view];
}

-(void)viewDidLoad
{
    // Code
    [super viewDidLoad];

    // Do any additional setup after loading the view.
}

-(void)didReceiveMemoryWarning
{
    // Code
    [super didReceiveMemoryWarning];
}

-(void)dealloc
{
    // Code
    [view release];
    
    [super dealloc];
}

@end
