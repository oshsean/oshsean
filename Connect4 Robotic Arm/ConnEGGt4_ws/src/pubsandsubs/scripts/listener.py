#!/usr/bin/env python
#jasper seffrin 27/7
#the subscriber program

import rospy
from std_msgs.msg import Int32

TURN = 1

def subscriberCallBack(x):
    if(TURN % 2 == 0):
        #print("Player 2")
        rospy.loginfo(rospy.get_caller_id() + " Player 2 has dropped their piece in column -- %s", x.data)
    else:
        #print("Player 1")
        rospy.loginfo(rospy.get_caller_id() + " Player 1 has dropped their piece in column -- %s", x.data)

    #rospy.loginfo(rospy.get_caller_id() + "I have recieved -- %s", x.data)
    global TURN
    TURN += 1
    
#def listener():
rospy.init_node('subscriberNode', anonymous=True)
rospy.Subscriber("talker", Int32, subscriberCallBack)
rospy.spin()

#if __name__ == '__main__':
    #turn = 1
    #listener()

