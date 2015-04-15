# -*- coding:gbk -*-
#�ͻ��˳���

import socket as S
import threading,time,random,string,sys

count = 1                                                #��¼�ͻ��˷�������Ĵ���
maxRtt = 0                                               #��¼������������ʱ��
minRtt = 0                                               #��¼��С����������ʱ��
avgRtt = 0                                               #��¼���������ƽ������ʱ��
sumRtt = 0                                               #��¼���������ʱ���
tag = 1                                                  #��Ϊ��������������ı�־
first_minRtt = 1                                         #��Ϊ��¼��һ����СRTT�ı�־
send = 0                                                 #��¼�ɹ��������������
#���庯�������ͻ��˵�socket�׽���
def clientSocket(name,lock,host,port):
    BUFSIZE = 1024                                       #��¼������Ϣ������ֽ���
    addr_1 = (host,port)                                 #����˵�ַ
    rtt = 0                                              #�۷�ʱ��
    #����ȫ�ֱ���
    global count
    global maxRtt   
    global minRtt  
    global sumRtt
    global avgRtt    
    global tag
    global first_minRtt
    global send

    #�����ͻ��˵�socket�׽��֣������ö˿ڿ�����
    udpClient = S.socket(S.AF_INET,S.SOCK_DGRAM)
    udpClient.setsockopt(S.SOL_SOCKET,S.SO_REUSEADDR,1)

    while count<=10:                                     #���������Ϊ10
        try:
            lock.acquire()                               #�ͻ��˽��̻��һ�������������ݲ���Ϊ�������̹���֪���ý����ͷ���
            data_1 = 'Request from %s :PingUDP SequenceNumber: %s TimeStamp: %s ' % (name,str(count),string.atof(time.time()))            #���͸�����˵���Ϣ
            time_1 = string.atof(time.time())            #��¼����������������ʱ�䣬������ת��Ϊ�����͵���

            #ģ����鴫���ӳٵı�־��
            sign = random.random()           
            if sign > 0.5:                               #����֮һ�ĸ��ʻ�����ӳ��ӳ�
                time.sleep(0.5)                          #�ӳ�0.5�뷢������
            udpClient.sendto(data_1,addr_1)              #�����˷���������Ϣ
            
            #�������Է������˵�����
            data_2,addr_2 = udpClient.recvfrom(BUFSIZE)
            time_2 = string.atof(time.time())            #��¼���յ�������������Ϣ��ʱ�䣬������ת��Ϊ�����͵���
            rtt = time_2-time_1                          #�����۷�ʱ��
        
            if rtt <= 1:                                 #����۷�ʱ�䲻����1�룬���������˷�������Ϣdata_2
                sumRtt+=rtt
                send+=1                                  #�ɹ����͵����������1
                if rtt > maxRtt:                         #�˴�����ʱ���������¼������۷�ʱ��
                    maxRtt = rtt
                elif first_minRtt:                       #��ǰ�������ǳɹ����״�����
                    first_minRtt = 0
                    minRtt = rtt
                elif (first_minRtt==0) and (rtt < minRtt): #�˴�����ʱ��С����С�۷�ʱ��
                    minRtt = rtt
                print '\n'+data_2+' RTT: ',int(1000*rtt)   #����ӷ���˷��ͻ�������Ϣ
            else:                                          #���͵�����ʧ��
                print '\nRequest failed!'
            count+=1                                       #������һ������
            lock.release()                                 #�ͻ��˽����ͷ���
        except:                                            #�����쳣
            print
    udpClient.close()                                      #�ر��׽���
    avgRtt = (1000*sumRtt)/10                              #��ƽ������ʱ�䣬��λ����
    if tag:                                                #���е������Ѿ����������ͳ�ƽ��
        tag = 0
        print '\n'
        print 'send: %s  relpay: %s  lost: %s' % (str(count-1),str(send),str(count-send-1)),'\n'
        print 'ƽ��RTT: %s  ���RTT: %s  ��СRTT: %s ' % (str(int(avgRtt)),str(int(1000*maxRtt)),str(int(1000*minRtt)))

#����clientThread��̳�threading.Thread��
class clientThread(threading.Thread):   
    #��ʼ��clientTreading��
    def __init__(self,lock,threadName,host,port):
        '''
        @��ʼ������
        @para host:�����IPֵ
        @para port:����˶˿ں�
        @para lock:������
        @para threadName:�߳�����
        '''

        #��ʾ���ø���ĳ�ʼ������
        super(clientThread,self).__init__(name=threadName)
        self.lock = lock
        self.host = host
        self.port = port

    #��д�����run����
    def run(self):
        '''
        @��д�����run���������߳�����֮����ø÷���
        '''
        clientSocket(self.name,self.lock,self.host,self.port) #����clientSocket����
        

#���崴���̵߳ķ���
def createThread(host,port):
    lock = threading.Lock()
    #for i in range(2):
       # clientThread(lock,'thread_'+str(i)).start()
    clientThread(lock,'A',host,port).start()  #�����������߳�
    clientThread(lock,'B',host,port).start()  #�����������߳�

if __name__ == '__main__':
    createThread(sys.argv[1],int(sys.argv[2]))
