#include <ros/ros.h>
#include <iostream>
#include <wiringPi.h>
#include <pthread.h>
#include <unistd.h>
//定义PWM周期
#define PWM_WIDTH (2000)
#define PWMPin1 22 //定义PWM引脚
#define PWMPin2 23 //定义PWM引脚
#define PWMPin3 24 //定义PWM引脚
#define PWMPin4 25 //定义PWM引脚
using namespace std;

inline void pwm_maker(int pin, int value);
void* pwm_maker1(void *arg);
void* pwm_maker2(void *arg);
void* pwm_maker3(void *arg);
void* pwm_maker4(void *arg);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pwm_out");
    ros::NodeHandle nh;
    //4路pwm初始值
    int pwm_value1 = 1000;
    int pwm_value2 = 1000;
    int pwm_value3 = 1000;
    int pwm_value4 = 1000;

    //初始化WiringPi
    wiringPiSetup();

    //设置引脚为输出
    pinMode(PWMPin1, OUTPUT);
    pinMode(PWMPin2, OUTPUT);
    pinMode(PWMPin3, OUTPUT);
    pinMode(PWMPin4, OUTPUT);

    pthread_t pth1;
    pthread_t pth2;
    pthread_t pth3;
    pthread_t pth4;
    //创建4个线程用于产生4路pwm信号
    pthread_create(&pth1, NULL, pwm_maker1, (void *) &pwm_value1);
    pthread_create(&pth2, NULL, pwm_maker2, (void *) &pwm_value2);
    pthread_create(&pth3, NULL, pwm_maker3, (void *) &pwm_value3);
    pthread_create(&pth4, NULL, pwm_maker4, (void *) &pwm_value4);

    ros::Rate r(10);
    while (ros::ok())
    {
        cout << "Please input PWM:\n";
        cin >> pwm_value1;
        r.sleep();
        ros::spinOnce();
    }
    pthread_join(pth1,NULL);
    return 0;
}

inline void pwm_maker(int pin, int *value)
{
    while (1)
    {
        //持续高电平
        digitalWrite(pin, HIGH);
        usleep(*value);
        //持续低电平
        digitalWrite(pin, LOW);
        usleep(PWM_WIDTH - *value);
    }
}

void *pwm_maker1(void *arg)
{
    int* value= (int *)arg;
    pwm_maker(PWMPin1, value);
    return 0;
}

void *pwm_maker2(void *arg)
{
    int* value= (int *)arg;
    pwm_maker(PWMPin2, value);
    return 0;
}

void *pwm_maker3(void *arg)
{
    int* value= (int *)arg;
    pwm_maker(PWMPin3, value);
    return 0;
}

void *pwm_maker4(void *arg)
{
    int* value= (int *)arg;
    pwm_maker(PWMPin4, value);
    return 0;
}
