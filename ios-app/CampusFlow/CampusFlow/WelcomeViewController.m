//
//  WelcomeViewController.m
//  CampusFlow
//
//  Created by Ellie on 9/24/16.
//  Copyright © 2016 HackGT. All rights reserved.
//

#import "WelcomeViewController.h"
#import <DCAnimationKit/UIView+DCAnimationKit.h>
#import "BLEManager.h"

@interface WelcomeViewController ()

@end

@implementation WelcomeViewController

static WelcomeViewController *inst;

+ (WelcomeViewController*) instance {
    return inst;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.codeTextField setHidden:TRUE];
    self.stage = 1;
    inst = self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)onConnectResult:(bool)result {
    if (!result) {
        self.stage -= 2;
        [self onNextButtonPressed:nil];
        UIAlertController *cont = [UIAlertController alertControllerWithTitle:@"Invalid Code" message:@"We couldn't find your FlowBox, please try again" preferredStyle:UIAlertControllerStyleAlert];
        [self presentViewController:cont animated:TRUE completion:nil];
        [self.nextButton setEnabled:TRUE];
    } else {
        [self onNextButtonPressed:nil];
    }
}

- (IBAction)onNextButtonPressed:(id)sender {
    [UIView animateWithDuration:1 animations:^{
        switch (self.stage) {
            case 1: {
                [self.labelATitle setHidden:TRUE];
                [self.labelADesc setHidden:TRUE];
                [self setTitle:@"Pair your FlowBox"];
                self.labelATitle.text = @"Please ensure bluetooth is turned on and tap next";
                [self.labelATitle setHidden:FALSE];
                break;
            }
            case 2: {
                if (![[BLEManager instance] isBTEnabled]) {
                    UIAlertController *cont = [UIAlertController alertControllerWithTitle:@"Enable Bluetooth" message:@"Please enable bluetooth" preferredStyle:UIAlertControllerStyleAlert];
                    [self presentViewController:cont animated:TRUE completion:nil];
                    return;
                }
                [self.labelADesc setHidden:TRUE];
                [self.labelATitle setText:@"Push the button on your FlowBox, and enter the code below"];
                [self.codeTextField setHidden:FALSE];
                break;
            }
            case 3: {
                [self.labelATitle setText:@"Connecting to FlowBox..."];
                [self.codeTextField setHidden:TRUE];
                [self.nextButton setEnabled:FALSE];
                dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                    [[BLEManager instance] connectToFlowBoxWithCode:[[self.codeTextField text] intValue]]
                    
                });
                break;
            }
            case 4: {
                [self.labelATitle setText:@"Connected! You're almost ready to use your FlowBox!"];
                break;
            }
            case 5: {
                [[self parentViewController] dismissViewControllerAnimated:TRUE completion:nil];
            }
                
        }
        self.stage += 1;
    }];
}
@end
