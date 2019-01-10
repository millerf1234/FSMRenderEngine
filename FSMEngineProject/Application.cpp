#include "Application.h"

#include <istream>

Application::Application() {
    LOG(INFO) << "Constructor for application called!\n";


    glfwInit();

    glfwTerminate();
}


Application::~Application() {

}

void Application::launch() {
    LOG(INFO) << "Application Launched!";

    std::cin.get();
}
