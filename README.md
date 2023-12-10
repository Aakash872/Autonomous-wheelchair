# README

Contributor: Anurag Maurya

## File structure:

The repository has 5 packages:

******1) DR_SPAAM_ROS:****** Package for detecting person from LiDAR data

************************************************2) rf2o_laser_odometry:************************************************ For generating fake odometry in some cases

********************************3) rplidar_ros:******************************** Standard package for getting sensor data from RPLiDAR

**********4) wc_navigation:********** Relevant launch files for navigation of wheelchair in 2D mode.

**********5) wc_navigation3D:********** Relevant launch files for navigation of wheelchair in 3D mode.

---

## Some more details regarding files:

The goal_set.py node and [sock.py](http://sock.py) node use socket API to get goal positions from another computer working on BCI headset data and publish the corresponding goal to the navigation stack.

---

## Launching navigation

### 2D Mapping

For mapping the arena using Hector SLAM

**Launch rplidar node**

```bash
roslaunch rplidar_ros rplidar.launch
```

**Launching the mapping node** 

```bash
roslaunch wc_navigation map_hector.launch
```

**Saving the map**

```bash
rosrun map_server map_saver -f test_1
```

---

### 2D Navigation

To launch the navigation using only RViZ 

```bash
roslaunch wc_navigation wc_navigation.launch
```

To launch the navigation and integrate the BCI headset

```bash
roslaunch wc_navigation wc_bci.launch
```

---

### 3D Mapping using Realsense

**********Using D415 here, other cameras might have different configurations**********

1) **Install Realsense wrappers**

[GitHub - IntelRealSense/realsense-ros at ros1-legacy](https://github.com/IntelRealSense/realsense-ros/tree/ros1-legacy)

`sudo apt-get install ros-$ROS_DISTRO-realsense2-camera`

2) **Install RTAB-MAP**

[https://github.com/introlab/rtabmap_ros](https://github.com/introlab/rtabmap_ros)

Can install from source or apt too

`sudo apt-get install ros-$ROS_DISTRO-rtabmap-ros`

3) **Run into terminal**

```bash
roslaunch realsense2_camera rs_camera.launch align_depth:=true
```

Make sure that /camera/aligned_depth_to_color/image_raw is being published

4) **Run into terminal**

```bash
roslaunch rtabmap_ros rtabmap.launch rtabmap_args:="--delete_db_on_start --Optimizer/GravitySigma 0.3" depth_topic:=/camera/aligned_depth_to_color/image_raw rgb_topic:=/camera/color/image_raw camera_info_topic:=/camera/color/camera_info approx_sync:=false
```

**********D435i**********

**Run into terminal**

```bash
rosrun imu_filter_madgwick imu_filter_node _use_mag:=false  _publish_tf:=false _world_frame:="enu"  /imu/data_raw:=/camera/imu  /imu/data:=/rtabmap/imu
```

**Run into terminal**

```bash
roslaunch rtabmap_ros rtabmap.launch	rtabmap_args:="--delete_db_on_start --Optimizer/GravitySigma 0.3"	depth_topic:=/camera/aligned_depth_to_color/image_raw	rgb_topic:=/camera/color/image_raw	camera_info_topic:=/camera/color/camera_info	approx_sync:=false	wait_imu_to_init:=true	imu_topic:=/rtabmap/imu
```

### Mapping using entire setup

```bash
roslaunch wc_navigation3D wc_3d_mapping.launch
```

### Navigation in 3D using entire setup

```bash
	roslaunch wc_navigation3D wc_3D2.launch
```

### Getting Person detections

```bash
roslaunch dr_spaam_ros dr_spaam_ros.launch
```
### Launching Low level controller

```bash
rosrun rosserial_python serial_node.py /dev/ttyACM0
```

### Demonstrations

2D navigation in a lab environment

![Screenshot from 2023-10-26 10-38-09.png](README%20b4d642b1ba3249dd85b1bd94d6f46439/Screenshot_from_2023-10-26_10-38-09.png)

3D mapping in a lab environment

![Screenshot from 2023-10-26 10-43-20.png](README%20b4d642b1ba3249dd85b1bd94d6f46439/Screenshot_from_2023-10-26_10-43-20.png)

3D navigation in lab environment

![Screenshot from 2023-10-26 10-46-57.png](README%20b4d642b1ba3249dd85b1bd94d6f46439/Screenshot_from_2023-10-26_10-46-57.png)

### Videos

2D navigation

[2D_nav.mp4](README%20b4d642b1ba3249dd85b1bd94d6f46439/2D_nav.mp4)

3D navigation

[3D_nav.mp4](README%20b4d642b1ba3249dd85b1bd94d6f46439/3D_nav.mp4)

## Issues

**uvc streamer issues:**

[https://github.com/IntelRealSense/realsense-ros/issues/2149](https://github.com/IntelRealSense/realsense-ros/issues/2149)

## References

```
@article{Jia2021Person2DRange,
  title        = {{Self-Supervised Person Detection in 2D Range Data using a
                   Calibrated Camera}},
  author       = {Dan Jia and Mats Steinweg and Alexander Hermans and Bastian Leibe},
  booktitle    = {International Conference on Robotics and Automation (ICRA)},
  year         = {2021}
}
```
