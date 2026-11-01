#include <rclcpp/rclcpp.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <trajectory_msgs/msg/joint_trajectory_point.hpp>
#include <termios.h>
#include <unistd.h>
#include <iostream>

class ArmandoTeleop : public rclcpp::Node
{
public:
    ArmandoTeleop() : Node("armando_teleop")
    {
        publisher_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>(
            "/joint_trajectory_controller/joint_trajectory", 10);
        
        // Inizializza le posizioni dei 4 giunti a 0
        positions_ = {0.0, 0.0, 0.0, 0.0};
        
        RCLCPP_INFO(this->get_logger(), "Nodo Teleop avviato.");
        RCLCPP_INFO(this->get_logger(), "Controlli: Q/A (j0), W/S (j1), E/D (j2), R/F (j3). Premi 'C' per uscire.");
    }

    void run()
    {
        char key;
        double step = 0.05; // Incremento in radianti per ogni pressione

        while (rclcpp::ok()) {
            key = getch();

            if (key == 'c' || key == 'C') {
                break;
            }

            bool moved = false;
            switch (key) {
                case 'q': positions_[0] += step; moved = true; break;
                case 'a': positions_[0] -= step; moved = true; break;
                case 'w': positions_[1] += step; moved = true; break;
                case 's': positions_[1] -= step; moved = true; break;
                case 'e': positions_[2] += step; moved = true; break;
                case 'd': positions_[2] -= step; moved = true; break;
                case 'r': positions_[3] += step; moved = true; break;
                case 'f': positions_[3] -= step; moved = true; break;
            }

            if (moved) {
                publish_trajectory();
                RCLCPP_INFO(this->get_logger(), "Giunti: [%.2f, %.2f, %.2f, %.2f]", 
                            positions_[0], positions_[1], positions_[2], positions_[3]);
            }
        }
    }

private:
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr publisher_;
    std::vector<double> positions_;

    // Funzione per leggere la tastiera in modo asincrono su Linux
    char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0) perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
        return buf;
    }

    void publish_trajectory() {
        auto msg = trajectory_msgs::msg::JointTrajectory();
        msg.joint_names = {"j0", "j1", "j2", "j3"};

        auto point = trajectory_msgs::msg::JointTrajectoryPoint();
        point.positions = positions_;
        
        // Tempo per raggiungere il punto (movimento rapido per teleop)
        point.time_from_start.sec = 0;
        point.time_from_start.nanosec = 200000000; // 0.2 secondi

        msg.points.push_back(point);
        publisher_->publish(msg);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ArmandoTeleop>();
    node->run();
    rclcpp::shutdown();
    return 0;
}
