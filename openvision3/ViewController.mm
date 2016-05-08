//
//  ViewController.m
//  openvision3
//
//  Created by LIN on 2016-04-16.
//  Copyright © 2016 ZHIMAABC. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
{
    CascadeClassifier face_cascade;
    int plugin;
}
@property (weak, nonatomic) IBOutlet UIImageView *vImagePreview;
@property (weak, nonatomic) IBOutlet UIImageView *vImageOutput;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.videoCamera = [[CvVideoCamera alloc] initWithParentView:self.vImageOutput];
    self.videoCamera.delegate = self;
    self.videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionFront;
    self.videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    self.videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset352x288;
    self.videoCamera.defaultFPS = 15;

    plugin = 0;
    
    NSString* cascadePath = [[NSBundle mainBundle]
                             pathForResource:@"haarcascade_frontalface_alt"
                             ofType:@"xml"];
    face_cascade.load([cascadePath UTF8String]);
}

- (void)viewDidAppear:(BOOL)animated
{
    [self.videoCamera start];
    
    AVCaptureVideoPreviewLayer *leftPreviewLayer = [AVCaptureVideoPreviewLayer layerWithSession:self.videoCamera.captureSession];
    leftPreviewLayer.frame = self.vImagePreview.bounds; // Assume you want the preview layer to fill the view.
    leftPreviewLayer.videoGravity = AVLayerVideoGravityResize;
    [self.vImagePreview.layer addSublayer:leftPreviewLayer];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [self.videoCamera stop];
}

#pragma mark Button Action
- (IBAction)switchCamera:(id)sender {
    [self.videoCamera switchCameras];
    
    self.vImagePreview.layer.sublayers = nil;
    AVCaptureVideoPreviewLayer *leftPreviewLayer = [AVCaptureVideoPreviewLayer layerWithSession:self.videoCamera.captureSession];
    leftPreviewLayer.frame = self.vImagePreview.bounds; // Assume you want the preview layer to fill the view.
    leftPreviewLayer.videoGravity = AVLayerVideoGravityResize;
    [self.vImagePreview.layer addSublayer:leftPreviewLayer];
}
- (IBAction)switchOutput:(id)sender {
    if (plugin == 0 ) {
        plugin = 1;
    } else {
        plugin = 0;
    }
}

#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
- (void)processImage:(cv::Mat&)image;
{
    cv::Size strel_size;
    strel_size = image.size();
    char str[200];
    
    switch(plugin) {
        case 0:
        {
            sprintf(str,"%d x %d FaceDetect",strel_size.width, strel_size.height);
            detectAndDisplay(image, face_cascade);
            break;
        }
        case 1:
        {
            sprintf(str,"%d x %d IttiSaliency",strel_size.width, strel_size.height);
            cv::Mat outputMat;
            SaliencyDetection(image,outputMat);
            image = outputMat;
            break;
        }
        default:
            sprintf(str,"%d x %d",strel_size.width, strel_size.height);
            break;
    }
    cv::putText(image, str, cvPoint(10,20),
                cv::FONT_HERSHEY_PLAIN, 0.8, cvScalar(255,0,0), 1, CV_AA,false);
}
#endif

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
