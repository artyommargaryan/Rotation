#include <iostream>
#include <opencv2/opencv.hpp>

struct MouseData {
    std::vector<cv::Point2f> points;
    cv::Mat image;
};

void onMouse(int event, int x, int y, int flags, void *userdata) {
    auto *data = static_cast<MouseData *>(userdata);

    if (event == cv::EVENT_LBUTTONDOWN) {
        data->points.emplace_back(x, y);

        cv::circle(data->image, cv::Point(x, y), 5, cv::Scalar(0, 255, 0), -1);

        cv::imshow("Image", data->image);
    }
}

bool comparePointsByY(const cv::Point2f &p1, const cv::Point2f &p2) {
    return p1.y < p2.y;
}

void sortPoints(std::vector<cv::Point2f> &points) {
    std::sort(points.begin(), points.end(), comparePointsByY);

    if (points[0].x > points[1].x) {
        cv::Point2f tmp = points[0];
        points[0] = points[1];
        points[1] = tmp;
    }

    if (points[2].x < points[3].x) {
        cv::Point2f tmp = points[2];
        points[2] = points[3];
        points[3] = tmp;
    }
}

cv::Mat rotateImagePart(const cv::Mat &image, std::vector<cv::Point2f> &points) {
    cv::Point2f srcPoints[4];

    for (int i = 0; i < points.size(); ++i) {
        srcPoints[i] = points[i];
    }

    float width = (float) cv::norm(srcPoints[1] - srcPoints[0]);
    float height = (float) cv::norm(srcPoints[3] - srcPoints[0]);
    cv::Point2f dstPoints[4] = {cv::Point2f(0, 0), cv::Point2f(width, 0), cv::Point2f(width, height),
                                cv::Point2f(0, height)};

    cv::Mat transform = cv::getPerspectiveTransform(srcPoints, dstPoints);

    cv::Mat warpedImage;
    cv::warpPerspective(image, warpedImage, transform, cv::Size((int) width, (int) height));

    return warpedImage;
}

int main() {
    std::string path;
    std::cout << "file path: ";
    std::cin >> path;

    cv::Mat image = cv::imread(path);

    cv::Mat imageWithPoints = image.clone();

    cv::namedWindow("Image");
    MouseData mouseData;
    mouseData.image = imageWithPoints;
    cv::setMouseCallback("Image", onMouse, &mouseData);

    cv::imshow("Image", imageWithPoints);

    cv::waitKey(0);

    sortPoints(mouseData.points);

    cv::Mat rotatedPart = rotateImagePart(image, mouseData.points);

    cv::imshow("Rotated Part", rotatedPart);
    cv::waitKey(0);

    return 0;
}
