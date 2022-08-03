#include "cameras_creator.h"

sage::CamerasCreator& sage::CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

sage::CamerasHandler* sage::CamerasCreator::createCamera(short id, sage::CamTypes camType) {
    CamerasHandler* camHandler = nullptr;
    switch (camType) {
        case sage::CamTypes::FFMPEG: {
            /** @todo Remove hardcode. Add xml parser. **/
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Voskresensk/TV/ch0_t.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/dev/video0", RtspTransportType::V4l);
            // camHandler = new CameraFFmpeg("rtsp://root:123456@192.168.1.85/onvif-media/media.amp?profile=balanced_h264&sessiontimeout=60&streamtype=unicast", RtspTransportType::Udp);

            // camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Протасово/4.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Протасово/3.avi", RtspTransportType::Vid);
            if(id == 0) {
                camHandler = new CameraFFmpeg("rtsp://192.168.0.203:554/v2", RtspTransportType::Udp);
            } else if (id == 1) {
                camHandler = new CameraFFmpeg("rtsp://192.168.0.6:554/v2", RtspTransportType::Udp);
            } else if (id == 2) {
                camHandler = new CameraFFmpeg("rtsp://ADMIN:1234@192.168.0.206:554/live/second", RtspTransportType::Udp);
            } else if (id == 3) {
                camHandler = new CameraFFmpeg("rtsp://192.168.0.207:554/snl/live/1/1", RtspTransportType::Udp);
            }
            break;
        }
        case sage::CamTypes::OPENCV: {
            if (id == 0) {
                camHandler = new CameraOpencv("/dev/video0", sage::CVCapType::V4L);
                // camHandler = new CameraOpencv("rtsp://root:123456@192.168.1.85/onvif-media/media.amp?profile=balanced_h264&sessiontimeout=60&streamtype=unicast", sage::CVCapType::FFMPEG);
            } else if (id == 1) {
                Log::debug(cv::getBuildInformation());
                // camHandler = new CameraOpencv("rtsp://root:123456@192.168.1.85/onvif-media/media.amp?profile=balanced_h264&sessiontimeout=60&streamtype=unicast", sage::CVCapType::FFMPEG);
                // camHandler = new CameraOpencv("v4l2src device=/dev/video0 ! video/x-raw,width=640,height=360,framerate=52/1 ! nvvidconv flip-method='+str(flip)+' ! video/x-raw(memory:NVMM), format=I420, width=640, height=360 ! nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! queue ! appsink drop=1", sage::CVCapType::GSTREAMER);
                camHandler = new CameraOpencv("rtsp://admin:admin@121.23.46.111/onvif/media/PRF08.wxp", sage::CVCapType::FFMPEG);
            }
        }
        default:
            break;
    }
    return camHandler;
}

sage::Decoder* sage::CamerasCreator::createDecoder(sage::DecTypes decType) {
    Decoder* decoder = nullptr;
    switch (decType) {
        case sage::DecTypes::FFMPEG: {
            Log::info("Create FFMpeg Decoder");
            decoder = new FFmpegDecoder();
            break;
        }
        default:
            break;
    }
    return decoder;
}
