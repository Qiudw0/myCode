# -*- coding: gbk -*-
#����˳���

import socket as S
import thread,random,time,sys

count = 1                                                    #��¼���յ���������
def server(port):
    host = ''                                                #�����÷��������Խ������������ַ������
    BUFSIZE = 1024                                           #���ý�����Ϣ������ֽ���
    global count
    addr_1 = (host,port)                                     #��������󶨵ĵ�ַ
    head_msg = ''                                            #��¼ͷ����Ϣ
    ori_port=0                                               #Դ�˿�(�ͻ��˶˿�)
    obj_port=port                                            #Ŀ�Ķ˿�(����˶˿�)

    #����һ���������˵��׽���
    udpServer = S.socket(S.AF_INET,S.SOCK_DGRAM)
    udpServer.setsockopt(S.SOL_SOCKET,S.SO_REUSEADDR,1)      #s��������˿�����

    #�󶨷������׽���
    udpServer.bind(addr_1)

    data_1='Requested succeedly!'                            #�����ؿͻ��˵�����
    while True:
        sleep_time = random.uniform(0,1)                     #�������С��һ���˯��ʱ��
        data_2,addr_2 = udpServer.recvfrom(BUFSIZE)          #�������Կͻ��˵�����
        ori_port = addr_2[1]                                 #��Դ�˿���Ϊ�ͻ��˶˿�ֵ
        head_msg = 'Դ�˿ڣ�%s Ŀ�Ķ˿�: %s ' % (str(ori_port),str(obj_port))   #UDP���ݵ�ͷ����Ϣ
        if data_2:                                           #����ӿͻ��˷��ص���Ϣ��Ϊ��
            print head_msg+data_2+'\n'                       #������Կͻ��˵�������Ϣ
            time.sleep(sleep_time)                           #�趨˯��ʱ�䣬ģ�⴫���ӳ�
            udpServer.sendto('Reply from %s : %s' % ('localhost',data_1),addr_2)    #������Ϣ���ͻ���
        else:
            udpServer.sendto('Requested falied!')
        count+=1                                             #������һ���ͻ��˵�����
        if count>10:                                         #��count�ﵽ10ʱ�˳����̣߳��ͻ��˶Է���˵�ping��
            thread.exit_thread()
    udpServer.close()                                        #�ر��׽���

if __name__=='__main__':
    server(int(sys.argv[1]))
