# armando_description

ROS 2 package containing the physical and visual description of the Armando robotic arm, developed for educational and simulation purposes. This modified version includes an additional camera mounted on the wrist (end-effector) and a custom C++ keyboard teleoperation node.

## 🛠️ Setup & Installation

To run this package from a clean environment (ROS 2 Humble / Docker container):

1. **Clone the repository** into your ROS 2 workspace's `src` folder:
   ```bash
   mkdir -p ~/ros2_ws/src
   cd ~/ros2_ws/src
   git clone https://github.com/<your-username>/armando_description.git
   ```

2. **Install dependencies** using `rosdep`:
   ```bash
   cd ~/ros2_ws
   sudo apt update
   rosdep update
   rosdep install --from-paths src --ignore-src -y
   ```

3. **Build and source** the workspace:
   ```bash
   colcon build --symlink-install
   source install/setup.bash
   ```

## 🚀 Launch Files

The `armando_description` package includes two main launch files:

### armando_gazebo.launch.py
Manages the physical simulation environment in Gazebo Harmonic. Simulates the dynamics of the robotic arm, loads the controllers, spawns the sensors (including the wrist camera), and enables the ROS-Gazebo bridge:

```bash
ros2 launch armando_description armando_gazebo.launch.py
```

### armando_rviz.launch.py
Starts the visualization interface in RViz2. It subscribes to runtime topics such as TF transforms and camera data, displaying them in a 3D view:

```bash
ros2 launch armando_description armando_rviz.launch.py
```

## 📷 Wrist Camera Validation

After launching the Gazebo simulation, you can verify that the newly mounted wrist camera is streaming image data from inside ROS 2:

1. **Check the camera topic** in the topic list:
   ```bash
   ros2 topic list
   ```

2. **Verify publication rate** (expected ~30 Hz):
   ```bash
   ros2 topic hz /camera_2/image_raw
   ```

3. **View the live video feed** using `rqt_image_view`:
   ```bash
   ros2 run rqt_image_view rqt_image_view /camera_2/image_raw
   ```

## 🎮 Keyboard Teleoperation Node

To control the arm joints from your keyboard, open an additional terminal, source the workspace, and run the teleop executable:

```bash
source ~/ros2_ws/install/setup.bash
ros2 run armando_description armando_teleop
```

### Controls:
* **Joint 0 (`j0`):** `Q` (+step) / `A` (-step)
* **Joint 1 (`j1`):** `W` (+step) / `S` (-step)
* **Joint 2 (`j2`):** `E` (+step) / `D` (-step)
* **Joint 3 (`j3`):** `R` (+step) / `F` (-step)
* **Exit:** `C`

*Note: While moving the arm via teleoperation, observe `/camera_2/image_raw` in `rqt_image_view` to confirm the camera frame moves solidly with the wrist.*
