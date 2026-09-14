#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/path.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <nav_msgs/srv/get_plan.hpp>

#include <vector>
#include <memory>
#include <cmath>
#include <random>


/**
 * @class RRTNode
 * @brief Rapidly-exploring Random Tree (RRT) path planning algorithm
 * 
 * TODO: Implement RRT algorithm for motion planning
 * - Build RRT from start to goal
 * - Implement collision checking
 * - Implement path shortcutting/smoothing
 * - Support RRT* for optimal paths
 * - Support bidirectional RRT (RRT-Connect)
 */
class RRTNode : public rclcpp::Node
{
public:
    RRTNode()
        : Node("rrt_node"),
          random_engine_(std::random_device{}())
    {
        RCLCPP_INFO(this->get_logger(), "RRT Path Planning Node initialized");
        
        // TODO: Create service server for path planning
        path_service_ = this->create_service<nav_msgs::srv::GetPlan>(
            "rrt/plan_path",
            std::bind(&RRTNode::plan_path_callback, this,
                     std::placeholders::_1, std::placeholders::_2)
        );
        
        // TODO: Subscribe to map
        map_subscriber_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
            "/map",
            10,
            std::bind(&RRTNode::map_callback, this, std::placeholders::_1)
        );
        
        // TODO: Publisher for visualization (RViz)
        path_publisher_ = this->create_publisher<nav_msgs::msg::Path>(
            "/rrt/path",
            10
        );
        
        // TODO: Publisher for tree visualization
        tree_publisher_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(
            "/rrt/tree",
            10
        );
    }

private:
    // ============================================================
    // Data Structures
    // ============================================================
    
    /**
     * @brief RRT tree node
     */
    struct TreeNode
    {
        double x, y;              // Position
        int parent_idx;           // Parent node index (-1 for root)
        double cost;              // Cost from root (for RRT*)
        
        TreeNode(double px, double py, int parent = -1)
            : x(px), y(py), parent_idx(parent), cost(0.0) {}
    };
    

    // ============================================================
    // Members
    // ============================================================
    
    rclcpp::Service<nav_msgs::srv::GetPlan>::SharedPtr path_service_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_subscriber_;
    
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr tree_publisher_;
    
    std::mt19937 random_engine_;
    
    // TODO: Store occupancy grid
    // TODO: Store RRT tree (vector of TreeNode)
    // TODO: Store algorithm parameters
    //   - Step size (max_step_length)
    //   - Goal bias (probability to sample goal)
    //   - Max iterations
    //   - Goal threshold
    //   - Collision check resolution
    

    // ============================================================
    // Callbacks
    // ============================================================
    
    void plan_path_callback(
        const std::shared_ptr<nav_msgs::srv::GetPlan::Request> request,
        std::shared_ptr<nav_msgs::srv::GetPlan::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "RRT path planning request received");
        
        double start_x = request->start.pose.position.x;
        double start_y = request->start.pose.position.y;
        double goal_x = request->goal.pose.position.x;
        double goal_y = request->goal.pose.position.y;
        
        // TODO: Run RRT algorithm
        std::vector<TreeNode> path_nodes;
        bool found = run_rrt(start_x, start_y, goal_x, goal_y, path_nodes);
        
        if (found)
        {
            // TODO: Convert tree nodes to path message
            response->plan.header.frame_id = "map";
            response->plan.header.stamp = this->now();
            // TODO: Populate waypoints
            
            RCLCPP_INFO(this->get_logger(), "Path found with %zu waypoints", 
                       response->plan.poses.size());
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "No path found");
        }
    }
    
    void map_callback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
    {
        RCLCPP_DEBUG(this->get_logger(), "Map received: %ux%u", 
                    msg->info.width, msg->info.height);
        
        // TODO: Store map for collision checking
    }


    // ============================================================
    // RRT Algorithm Methods
    // ============================================================
    
    /**
     * @brief Main RRT algorithm
     * TODO: Implement RRT algorithm loop
     */
    bool run_rrt(double start_x, double start_y, 
                 double goal_x, double goal_y,
                 std::vector<TreeNode> &path)
    {
        RCLCPP_INFO(this->get_logger(), "Running RRT algorithm");
        RCLCPP_DEBUG(this->get_logger(), "Start: (%.2f, %.2f) -> Goal: (%.2f, %.2f)",
                    start_x, start_y, goal_x, goal_y);
        
        // TODO: Initialize tree with start node
        // TODO: Main loop:
        //   - Sample random configuration
        //   - Find nearest node in tree
        //   - Extend towards sample
        //   - Check collision
        //   - Add new node if valid
        //   - Check if goal reached
        //   - Apply path smoothing
        // TODO: Backtrack from goal to start to build path
        
        return false;  // Placeholder
    }
    
    /**
     * @brief Sample random configuration in free space
     * TODO: Implement sampling strategy
     */
    std::pair<double, double> sample_random_config()
    {
        // TODO: With probability goal_bias, sample goal
        // TODO: Otherwise, sample random point in valid space
        
        std::uniform_real_distribution<double> dist_x(0.0, 10.0);  // TODO: Use map bounds
        std::uniform_real_distribution<double> dist_y(0.0, 10.0);
        
        return {dist_x(random_engine_), dist_y(random_engine_)};
    }
    
    /**
     * @brief Find nearest node in tree
     * TODO: Implement nearest neighbor search
     */
    int find_nearest_node(const std::vector<TreeNode> &tree,
                         double x, double y)
    {
        // TODO: Linear search (can optimize with KD-tree)
        int nearest_idx = 0;
        double min_dist = 1e10;
        
        for (size_t i = 0; i < tree.size(); i++)
        {
            double dist = std::hypot(tree[i].x - x, tree[i].y - y);
            if (dist < min_dist)
            {
                min_dist = dist;
                nearest_idx = i;
            }
        }
        
        return nearest_idx;
    }
    
    /**
     * @brief Steer from source towards target
     * TODO: Implement steering with step size limit
     */
    std::pair<double, double> steer(double x1, double y1, 
                                    double x2, double y2, 
                                    double step_size)
    {
        double dist = std::hypot(x2 - x1, y2 - y1);
        double move_dist = std::min(step_size, dist);
        
        if (dist > 0)
        {
            double ratio = move_dist / dist;
            return {x1 + ratio * (x2 - x1), y1 + ratio * (y2 - y1)};
        }
        
        return {x1, y1};
    }
    
    /**
     * @brief Check collision between two points
     * TODO: Implement line-of-sight collision checking
     */
    bool is_collision_free(double x1, double y1, double x2, double y2)
    {
        // TODO: Ray casting or circle-based collision check
        // TODO: Sample points along line and check occupancy grid
        return true;  // Placeholder
    }
    
    /**
     * @brief Smooth/shorten path using short-cutting
     * TODO: Implement path smoothing
     */
    void smooth_path(std::vector<TreeNode> &path)
    {
        RCLCPP_DEBUG(this->get_logger(), "Smoothing path");
        
        // TODO: Repeatedly try to connect non-adjacent nodes
        // TODO: If direct connection is collision-free, remove intermediate nodes
    }
};


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<RRTNode>();
    
    RCLCPP_INFO(node->get_logger(), "RRT Path Planning Node started");
    
    rclcpp::spin(node);
    
    rclcpp::shutdown();
    
    return 0;
}
