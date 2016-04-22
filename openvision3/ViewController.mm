//
//  ViewController.m
//  openvision3
//
//  Created by LIN on 2016-04-16.
//  Copyright © 2016 ZHIMAABC. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UIImageView *vImagePreview;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.videoCamera = [[CvVideoCamera alloc] initWithParentView:self.vImagePreview];
    self.videoCamera.delegate = self;
    self.videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionFront;
    self.videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationLandscapeRight;
    self.videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPresetHigh;
    self.videoCamera.defaultFPS = 15;
    
}

- (void)viewDidAppear:(BOOL)animated
{
    [self.videoCamera start];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [self.videoCamera stop];
}

#pragma mark Button Action
- (IBAction)switchCamera:(id)sender {
    [self.videoCamera switchCameras];
}

#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
- (void)processImage:(cv::Mat&)image;
{
    cv::Mat outputMat;
    SaliencyDetection(image,outputMat);
    image = outputMat;
    
    cv::putText(image, "Hello, please add your opencv code at there!", cvPoint(30,30),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255,0,0), 1, CV_AA,false);
    
}
#endif

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
