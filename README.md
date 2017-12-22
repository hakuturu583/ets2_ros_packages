# ets2_ros_packages
ROS packages for building Autonomous driving system in Euro Track Simulator2(ETS2)
## packages
### ets2_ros_bridge
bridges bewteen ROS and ETS2  
#### nodes
ets2_ros_bridge_node
- parameters  
  - ~/window_id  
    - type : string  
    - description : window_id of ETS2. You can get this infomation from "xwininfo" command.
- topics(Subscription)
  - NONE
- topics(Publish)
  - ~/image
    - type : sensor_msgs/Image
    - description : images from ETS2 game window  
