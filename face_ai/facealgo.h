#ifndef FACEALGO_H
#define FACEALGO_H

#include <opencv2/dnn.hpp>
#include<opencv2/opencv.hpp>
struct faceInfo {
    std::string name;
    cv::Mat detResult;
};


class FaceAlgo {
public:
    FaceAlgo();
    void initFaceModels(std::string detect_model_path, std::string recog_model_path, std::string face_db_dir);
    void detectFace(cv::Mat &frame, std::vector<std::shared_ptr<faceInfo>> &results, bool showFPS);
    void matchFace(cv::Mat &frame,std::vector<std::shared_ptr<faceInfo>> &results,bool l2 = false);
    void registFace(cv::Mat &faceRoi, std::string name);
private:
    std::map<std::string, cv::Mat> face_models;
    cv::Ptr<cv::FaceDetectorYN> faceDetector;
    cv::Ptr<cv::FaceRecognizerSF> faceRecognizer;
};


#endif // FACEALGO_H
