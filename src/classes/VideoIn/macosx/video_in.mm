/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#include "rubyk/planet.h"
#include "VideoIn/video_in.h"

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

/* ======================== Capture delegate ============== */

@interface CaptureDelegate : NSObject {
  VideoIn *video_in_;
  Value current_frame_;

  /** Size of the frame in pixels.
   */
  size_t cols_, rows_;

  /** Number of bytes to get to next row.
   */
  size_t step_;

  /** Initial padding in bytes to get to first element.
   */
  size_t padding_;
}

- (id)initWithVideoIn:(VideoIn*)video_in;
/* ================ Capture Delegate method ===== */
- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection;
@end


/* ======================== VideoIn::Implementation ======= */

class VideoIn::Implementation {
public:
  Implementation(VideoIn *master)
      : master_(master),
        capture_session_(nil),
        capture_input_(nil),
        capture_output_(nil),
        capture_delegate_(nil),

        window_(nil),
        capture_view_(nil),
        x_(0), y_(0), preview_width_(320), preview_height_(240),
        capture_on_(false),
        preview_open_(false) {}

  ~Implementation() {
    stop_capture();
    close_preview();
    if (capture_delegate_) [capture_delegate_ release];
  }

  // ---------------------------------------------------------------
  const Value set_device(const std::string &device_name) {
    if (capture_on_) {
      // change device...
    } else {
      device_name_ = device_name;
    }
    return gNilValue;
  }

  // ---------------------------------------------------------------
  const Value start_capture() {
    if (!Planet::gui_ready()) {
      return Value(INTERNAL_SERVER_ERROR, "Could not start NSApp (needed for Video in).");
    }

    capture_delegate_ = [[CaptureDelegate alloc] initWithVideoIn:master_];

    NSError *error = nil;
    capture_session_ = [[QTCaptureSession alloc] init];

    // Find video device
    QTCaptureDevice *device;
    if (device_name_ != "") {
      device = [QTCaptureDevice deviceWithUniqueID:[NSString stringWithUTF8String:device_name_.c_str()]];
    } else {
      device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
    }

    if (!device) {
      return FValue(BAD_REQUEST_ERROR, "Could not find device '", device_name_.c_str(), "'.");
    }

    if (![device open:&error]) {
      return FValue(BAD_REQUEST_ERROR, "Could not open device '", device_name_.c_str(), "'.");
    }


    // Add the video device to the session as a device input.
    capture_input_ = [[QTCaptureDeviceInput alloc] initWithDevice:device];
    if (![capture_session_ addInput:capture_input_ error:&error]) {
      return FValue(INTERNAL_SERVER_ERROR, "Could not add input '%s' to capture session.", device_name_.c_str());
    }

    capture_output_ = [[QTCaptureDecompressedVideoOutput alloc] init];
    [capture_output_ setDelegate:capture_delegate_];
    [capture_output_ setAutomaticallyDropsLateVideoFrames:YES];

    NSMutableDictionary* pixelBufferAttributes = [[NSMutableDictionary alloc] init];

    [pixelBufferAttributes
      setObject:[NSNumber numberWithUnsignedInt:k24RGBPixelFormat]
      forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey
    ];

    [capture_output_ setPixelBufferAttributes:pixelBufferAttributes];
    [pixelBufferAttributes release];

    if (![capture_session_ addOutput:capture_output_ error:&error]) {
      return Value(INTERNAL_SERVER_ERROR, "Could not add output delegate to capture session.");
    }

    if (capture_view_) {
      [capture_view_ setCaptureSession:capture_session_];
    }

    [capture_session_ startRunning];

    return Value("Started ok");
  }

  // ---------------------------------------------------------------
  void stop_capture() {

    if (capture_session_) {
      if ([capture_session_ isRunning]) [capture_session_ stopRunning];

      if (capture_output_) {
        [capture_session_ removeOutput:capture_output_];
        [capture_output_ release];
        capture_output_ = nil;
      }

      if (capture_input_) {
        [capture_session_ removeInput:capture_input_];
        QTCaptureDevice *device = [capture_input_ device];

        if ([device isOpen]) {
          [device close];
        }

        [capture_input_ release];
        capture_input_ = nil;
      }
      [capture_session_ release];
      capture_session_ = nil;

      [capture_delegate_ release];
      capture_delegate_ = nil;
    }
  }

  // ---------------------------------------------------------------
  const Value open_preview(int x, int y) {
    if (preview_open_) return gNilValue;

    if (!Planet::gui_ready()) {
      return Value(INTERNAL_SERVER_ERROR, "Could not start NSApp.");
    }

    { NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
      capture_view_ = [[QTCaptureView alloc] initWithFrame:NSMakeRect(0, 0, preview_width_, preview_height_)];

      int style = NSClosableWindowMask | NSResizableWindowMask | NSTitledWindowMask | NSMiniaturizableWindowMask;

      window_ = [[NSWindow alloc] initWithContentRect:NSMakeRect(x_, y_, preview_width_, preview_height_)
        styleMask:style
        backing:NSBackingStoreBuffered
        defer:NO];

      [capture_view_ setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      [[window_ contentView] addSubview:capture_view_];

      if (capture_session_) {
        [capture_view_ setCaptureSession:capture_session_];
      }

      [window_ makeKeyAndOrderFront:window_];

      [pool drain];
    }

    preview_open_ = true;

    return gNilValue;
  }

  // ---------------------------------------------------------------
  const Value close_preview() {
    if (!preview_open_) return gNilValue;
    { NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

      if (capture_view_) {
        [capture_view_ release];
        capture_view_ = nil;
      }

      if (window_) {
        [window_ release];
        window_ = nil;
      }

      [pool drain];
    }

    preview_open_ = false;
    return gNilValue;
  }

  VideoIn *master_;

  QTCaptureSession *capture_session_;
  QTCaptureDeviceInput *capture_input_;
  QTCaptureDecompressedVideoOutput *capture_output_;
  CaptureDelegate *capture_delegate_;

  NSWindow *window_;
  QTCaptureView *capture_view_;
  std::string device_name_;
  int x_, y_, preview_width_, preview_height_;
  bool capture_on_;
  bool preview_open_;
};
/* ======================== Capture delegate ============== */

@implementation CaptureDelegate
- (id)initWithVideoIn:(VideoIn*)video_in {
  if ( (self = [super init]) ) {
    video_in_ = video_in;
  }
  return self;
}

- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection {
  [sampleBuffer incrementSampleUseCount];

  if (CVPixelBufferLockBaseAddress(videoFrame, 0) == 0) {
    if (!current_frame_.is_matrix()) {
      if (CVPixelBufferIsPlanar(videoFrame)) {
        std::cerr << "Cannot capture frame with multiple planes (planar data not supported).\n";
        return;
      }

      cols_ = CVPixelBufferGetWidth(videoFrame);
      rows_ = CVPixelBufferGetHeight(videoFrame);

      size_t pad_l, pad_r, pad_t, pad_b;

      CVPixelBufferGetExtendedPixels(videoFrame, &pad_l, &pad_r, &pad_t, &pad_b);

      size_t bytes_per_row = CVPixelBufferGetBytesPerRow(videoFrame);

      step_ = (cols_ + pad_l + pad_r) * 3; // 3 bytes per pixel, we add left and right padding to the width
      padding_ = (pad_t * step_) + (pad_l * 3);

      std::cout << "received CVImageBufferRef " << cols_ << "x" << rows_
                << " type: " << CVPixelBufferGetPixelFormatType(videoFrame)
                << " pad_L " << pad_l
                << " pad_R " << pad_r
                << " pad_T " << pad_t
                << " pad_B " << pad_b
                << " bytes_per_row: " << bytes_per_row
                << " padding : " << padding_
                << " step : " << step_
                << "\n";
                current_frame_ = MatrixValue();
                current_frame_.adopt(new Matrix(
                  cols_,
                  rows_,
                  CV_8UC3, // 3 unsigned ints for each pixel
                  (unsigned char*)CVPixelBufferGetBaseAddress(videoFrame) + padding_,
                  step_    // step in bytes to get to next row
                ));
    } else {

    }


    // ======== change matrix data
    current_frame_.matrix_->data = ((unsigned char*)CVPixelBufferGetBaseAddress(videoFrame)) + padding_;

    // we do not update dataend and datastart because nobody should call locateROI or adjustROI

    video_in_->frame_changed(current_frame_);

    CVPixelBufferUnlockBaseAddress(videoFrame, 0);
  }

  [sampleBuffer decrementSampleUseCount];
}

@end

/* ======================== VideoIn ======================= */

VideoIn::VideoIn() {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    impl_ = new VideoIn::Implementation(this);
  [pool drain];
}

VideoIn::~VideoIn() {
  delete impl_;
}

const Value VideoIn::set_device(const std::string &device_name) {
  return impl_->set_device(device_name);
}

// ---------------------------------------------------------------
const Value VideoIn::start_capture() {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    Value res = impl_->start_capture();
  [pool drain];

  return res;
}

// ---------------------------------------------------------------
void VideoIn::stop_capture() {
  impl_->stop_capture();
}

// ---------------------------------------------------------------
const Value VideoIn::open_preview(int x, int y) {
  return impl_->open_preview(x, y);
}

// ---------------------------------------------------------------
const Value VideoIn::close_preview() {
  return impl_->close_preview();
}
