//
//  main.cpp
//  AugmentedReality
//
//  Created by linsho kaku on 2019/04/15.
//  Copyright © 2019 linsho kaku. All rights reserved.
//

#include <iostream>

#include <opencv2/opencv.hpp>

#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/glew.h>


#include "DrawPrimitives.h"

cv::RNG rng(0);

void get_polies(cv::Mat img_bgr, std::vector<std::vector<cv::Point>>& polies){
    cv::Mat img_gray;
    cv::Mat img_blur;
    cv::Mat img_filtered;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::cvtColor( img_bgr, img_gray, cv::COLOR_BGR2GRAY );
    cv::GaussianBlur(img_gray, img_blur, cv::Size(5, 5), 0);
    cv::adaptiveThreshold(img_blur, img_filtered, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 33, 5);
    cv::findContours(img_filtered, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    
    for(auto contour : contours){
        double eps = 0.05*cv::arcLength(contour, true);
        std::vector<cv::Point> poly;
        cv::approxPolyDP(contour, poly, eps, true);
        cv::Rect rect = cv::boundingRect(poly);
        int poly_area = rect.area();
        int img_area = img_bgr.cols*img_bgr.rows;
        bool is_valid = poly_area < img_area*0.99
        && poly_area > img_area*0.01
        && poly.size() == 4
        && cv::isContourConvex(poly);
        if(is_valid){
            polies.push_back(poly);
        }
    }
}

void draw_polies(cv::Mat& img_bgr, std::vector<std::vector<cv::Point>>& polies){
    for(int i = 0;i < polies.size();i++){
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::drawContours(img_bgr, polies, i, color, 5);
    }
}

void get_edge(cv::Mat& img_bgr, std::vector<std::vector<cv::Point>>& polies, std::vector<std::vector<cv::Vec4f>>& lines){
    for(auto poly : polies){
        for(int i = 1;i < 7;i++){
            
        }
    }
}




/* program & OpenGL initialization */
void init(int argc, char *argv[])
{
    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    
    // enable and set depth parameters
    glEnable( GL_DEPTH_TEST );
    
    glClearDepth( 1.0 );
}

void display(GLFWwindow* window) {
    
    float ratio;
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    
    // clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    gluPerspective( 1, ((double)width/(double)height), 0.01, 100 );
    
    // move to origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // move the object backwards
    glTranslatef( 0.0, 0.0, -10.0 );
    
    // move the object in a fancy way
    const float t = (float) glfwGetTime() * 2.0f;
    const float n = 0.5f;
    //    glTranslatef( 1.0f*sin(n*t), 1.0f*cos(n*t), 0.0f );
    
    // rotate the object
    //    glRotatef((float) glfwGetTime() * 50.f, 1.f, 1.f, 0.f);
    glRotatef(90.f, 1.f, 0.f, 0.f);
    glRotatef((float) glfwGetTime() * 45.f, 0.f, 0.f, 1.f);
    
    // draw a triangle and a red sphere
    //    glBegin(GL_TRIANGLES);
    //    glColor3f(1.f, 0.f, 0.f);
    //    glVertex3f(-0.6f, -0.4f, 0.f);
    //    glColor3f(0.f, 1.f, 0.f);
    //    glVertex3f(0.6f, -0.4f, 0.f);
    //    glColor3f(0.f, 0.f, 1.f);
    //    glVertex3f(0.f, 0.6f, 0.f);
    //    glEnd();
    //
    //    glColor4f( 1.0, 0.0, 0.0, 1.0 );
    drawSnowman(false);
    
    
}

void reshape( GLFWwindow* window, int width, int height ) {
    
    // set a whole-window viewport
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    
    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );
    
    // invalidate display
    //glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    // initialize the window system
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Exercise 6 - Open GL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    
    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );
    
    // initialize the GL library
    init(argc, argv);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        display(window);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

//int main(){
//    std::cout << "Welcome to AR World!!" << std::endl;
//    cv::VideoCapture cap;
//    cap.open(0);
//    const std::string kWinName1 = "Exercise 1 - Original Image";
////    const std::string kWinName2 = "Exercise 1 - Thresholded Image";
//    cv::namedWindow( kWinName1, cv::WINDOW_AUTOSIZE);
////    cv::namedWindow( kWinName2, cv::WINDOW_NORMAL);
//
//    cv::Mat img_bgr;
//    while (true) {
//        cap >> img_bgr;
//        if(img_bgr.empty()){
//            printf("Could not query frame. Trying to reinitialize.\n");
//            exit(0);
//        }
//
//        std::vector<std::vector<cv::Point>> polies;
//        get_polies(img_bgr, polies);
//        draw_polies(img_bgr, polies);
//
//        cv::imshow(kWinName1, img_bgr);
//
//        int key = cv::waitKey (10);
//        if (key == 27) break;
//    }
//    cv::destroyWindow (kWinName1);
////    cv::destroyWindow (kWinName2);
//    std::cout << "Finished\n";
//    return 0;
//}
