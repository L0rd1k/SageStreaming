#include "cameras_creator.h"

sage::CamerasCreator& sage::CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

sage::CamerasHandler* sage::CamerasCreator::createCamera(short id, sage::CamTypes camType) {
    CamerasHandler* camHandler = nullptr;
    switch (camType) {
        case sage::CamTypes::FFMPEG: {
            /** @todo Remove hardcode. Add xml/ini parser. **/
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Voskresensk/TV/ch0_t.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("rtsp://root:123456@192.168.1.85/onvif-media/media.amp?profile=balanced_h264&sessiontimeout=60&streamtype=unicast", RtspTransportType::Udp);
            // camHandler
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Протасово/4.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Протасово/3.avi", RtspTransportType::Vid);
            
            // if (id == 0) {
            //     camHandler = new CameraFFmpeg("rtsp://ADMIN:1234@192.168.0.208:554/live/main", RtspTransportType::Udp);
            // } else {
            //     camHandler = new CameraFFmpeg("rtsp://192.168.0.203:554/v2", RtspTransportType::Udp);
            // }

            if (id == 0) {
                // camHandler = new CameraFFmpeg("rtsp://192.168.0.203:554/v2", RtspTransportType::Udp);
                camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
            } else if (id == 1) {
                camHandler = new CameraFFmpeg("rtsp://192.168.0.6:554/v2", RtspTransportType::Udp);
            } else if (id == 2) {
                camHandler = new CameraFFmpeg("rtsp://ADMIN:1234@192.168.0.206:554/live/second", RtspTransportType::Udp);
            } else if (id == 3) {
                camHandler = new CameraFFmpeg("rtsp://192.168.0.207:554/snl/live/1/1", RtspTransportType::Udp);
            } else if (id == 4) {
                camHandler = new CameraFFmpeg("rtsp://ADMIN:1234@192.168.0.208:554/live/main", RtspTransportType::Udp);
            }
            break;
        }
        case sage::CamTypes::OPENCV: {
            if (id == 0) {
                camHandler = new CameraOpencv("/home/ilya/Видео/SlovakiaRobot/Sample.avi", sage::CVCapType::ANY);
            } else if (id == 1) {
                camHandler = new CameraOpencv("rtsp://root:123456@192.168.1.85/onvif-media/media.amp?profile=balanced_h264&sessiontimeout=60&streamtype=unicast", sage::CVCapType::FFMPEG);
            } else if (id == 2) {
                camHandler = new CameraOpencv("/dev/video0", sage::CVCapType::ANY);
            } else if (id == 3) {
                camHandler = new CameraOpencv("rtsp://root:123456@192.168.1.85/onvif-media/media.amp?profile=balanced_h264&sessiontimeout=60&streamtype=unicast", sage::CVCapType::FFMPEG);
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
