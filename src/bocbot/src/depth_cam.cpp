#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

#include <memory>


/**
 * @class DepthCameraNode
 * @brief Depth camera processing and point cloud generation
 * 
 * TODO: Process depth camera data
 * - Subscribe to depth images and camera info
 * - Convert depth images to point clouds
 * - Apply filters (temporal, spatial)
 * - Detect obstacles/surfaces
 * - Publish processed data
 */
class DepthCameraNode : public rclcpp::Node
{
public:
    DepthCameraNode()
        : Node("depth_camera_node")
    {
        RCLCPP_INFO(this->get_logger(), "Depth Camera Node initialized");
        
        // TODO: Subscribe to depth image and camera info topics
        depth_subscriber_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/depth",
            rclcpp::SensorDataQoS(),
            std::bind(&DepthCameraNode::depth_callback, this, std::placeholders::_1)
        );
        
        camera_info_subscriber_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
            "/camera/camera_info",
            rclcpp::SensorDataQoS(),
            std::bind(&DepthCameraNode::camera_info_callback, this, std::placeholders::_1)
        );
        
        // TODO: Publisher for processed point cloud
        pointcloud_publisher_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
            "/camera/point_cloud",
            rclcpp::SensorDataQoS()
        );
        
        // TODO: Publisher for processed depth image
        processed_depth_publisher_ = this->create_publisher<sensor_msgs::msg::Image>(
            "/camera/depth_processed",
            rclcpp::SensorDataQoS()
        );
    }

private:
    // ============================================================
    // Members
    // ============================================================
    
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_subscriber_;
    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_subscriber_;
    
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud_publisher_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr processed_depth_publisher_;
    
    // TODO: Store camera intrinsics (fx, fy, cx, cy)
    // TODO: Store depth scale factor
    // TODO: Store filtering parameters
    

    // ============================================================
    // Callbacks
    // ============================================================
    
    void depth_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        RCLCPP_DEBUG(this->get_logger(), "Depth image received: %dx%d", 
                     msg->width, msg->height);
        
        try
        {
            // TODO: Convert ROS image to OpenCV Mat
            // TODO: Apply filters (median blur, bilateral filter, etc)
            // TODO: Detect invalid/noisy pixels
            // TODO: Convert to point cloud
            // TODO: Publish results
        }
        catch (const cv_bridge::Exception &e)
        {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge error: %s", e.what());
        }
    }
    
    void camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
    {
        RCLCPP_DEBUG(this->get_logger(), "Camera info received");
        
        // TODO: Extract and store camera intrinsics
        // TODO: Store depth scale factor if available
    }


    // ============================================================
    // Processing Methods
    // ============================================================
    
    /**
     * @brief Filter depth image
     * TODO: Implement various filtering techniques
     */
    cv::Mat filter_depth_image(const cv::Mat &depth_image)
    {
        // TODO: Apply median filter
        // TODO: Apply bilateral filter
        // TODO: Apply temporal filtering
        return depth_image;  // Placeholder
    }
    
    /**
     * @brief Convert depth image to point cloud
     * TODO: Implement depth to 3D conversion
     */
    sensor_msgs::msg::PointCloud2 depth_to_pointcloud(
        const cv::Mat &depth_image,
        const sensor_msgs::msg::CameraInfo &camera_info)
    {
        sensor_msgs::msg::PointCloud2 cloud;
        cloud.header.frame_id = "camera_link";
        cloud.header.stamp = this->now();
        
        // TODO: Implementation
        // - Use camera intrinsics to project depth pixels to 3D
        // - Filter points by depth range
        // - Organize points in PointCloud2 format
        
        return cloud;
    }
    
    /**
     * @brief Detect obstacles in depth data
     * TODO: Implement obstacle detection
     */
    bool detect_obstacles(const cv::Mat &depth_image)
    {
        // TODO: Analyze depth histogram
        // TODO: Detect close objects
        // TODO: Determine obstacle locations
        return false;  // Placeholder
    }
};


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<DepthCameraNode>();
    
    RCLCPP_INFO(node->get_logger(), "Depth Camera Node started");
    
    rclcpp::spin(node);
    
    rclcpp::shutdown();
    
    return 0;
}
