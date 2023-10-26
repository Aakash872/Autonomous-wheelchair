#!/usr/bin/env python
import rospy
from std_msgs.msg import String
import socket

def talker():
    pub = rospy.Publisher('biogoal', String, queue_size=10)
    rospy.init_node('goal_letter', anonymous=True)
    rate = rospy.Rate(10) 
    s = socket.socket()  
    port = 4444
    s.bind(("10.147.17.239", port))
    s.listen(5)
    c, addr = s.accept()
    print("data recieved")
    hello_str = c.recvfrom(1024).decode()
    while not rospy.is_shutdown():
        #hello_str = "hello world %s" % rospy.get_time()
        #rospy.loginfo(hello_str)
        print(hello_str)
	#rospy.logwarn(hello_str)
        pub.publish(hello_str)
        rate.sleep()
    s.close()
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
