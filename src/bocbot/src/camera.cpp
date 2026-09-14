#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>

#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>


class CameraNode : public rclcpp::Node
{
public:

    CameraNode()
        : Node("camera_node")
    {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/image",
            rclcpp::SensorDataQoS(),
            std::bind(
                &CameraNode::image_callback,
                this,
                std::placeholders::_1
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Camera node started"
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Subscribing: /camera/image"
        );
    }


private:

    void image_callback(
        const sensor_msgs::msg::Image::SharedPtr msg)
    {
        try
        {
            cv::Mat frame =
                cv_bridge::toCvCopy(
                    msg,
                    "bgr8"
                )->image;

            cv::imshow(
                "Bocbot Camera",
                frame
            );

            cv::waitKey(1);

            RCLCPP_INFO_THROTTLE(
                this->get_logger(),
                *this->get_clock(),
                2000,
                "Image received: %ux%u",
                msg->width,
                msg->height
            );
        }
        catch (const cv_bridge::Exception &e)
        {
            RCLCPP_ERROR(
                this->get_logger(),
                "cv_bridge error: %s",
                e.what()
            );
        }
    }


    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr
        subscription_;
};


int main(
    int argc,
    char *argv[])
{
    rclcpp::init(argc, argv);

    auto node =
        std::make_shared<CameraNode>();

    rclcpp::spin(node);

    cv::destroyAllWindows();

    rclcpp::shutdown();

    return 0;
}