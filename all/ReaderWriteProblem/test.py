#coding=utf-8
import threading  
import random  
import time  
from Writer import Writer

#gloabal count

ReaderCount = 0;  


class Reader(threading.Thread):  
    """class using semaphore"""  

    def __init__(self,threadName,CountSem,WriteSem,ReadSem,AddRead,SleepTime):  

      """initialize thread"""  


      threading.Thread.__init__(self,name=threadName)  
      #self.sleepTime=random.randrange(1,6) 
      #self.sleepTime = random.randrange(1,6)  
       
      #set the semaphore as a data attribute of the class  
       
      self.CountSemaphore = CountSem
      self.WriteSemaphore = WriteSem
      self.ReadSemaphore = ReadSem
      self.AddReadSemaphore = AddRead
      self.sleepTime = SleepTime

    def run(self):
      #acquire write mutex
      global ReaderCount
      print " Thread %s  enters function run() and is waiting for  AddReadSemaphore!" % (self.getName()) 
      self.AddReadSemaphore.acquire()
      print " Thread %s  is waiting for  ReadSemaphore!" % (self.getName()) 
      self.ReadSemaphore.acquire()

      self.CountSemaphore.acquire() 
      if ReaderCount == 0:
        print " Thread %s  is waiting for  WriteSemaphore!" % (self.getName()) 
        self.WriteSemaphore.acquire()
      
      ReaderCount = ReaderCount + 1

      self.CountSemaphore.release()
      self.AddReadSemaphore.release()



      print " Thread %s  is reading! ReaderCount is %d now" % (self.getName(),ReaderCount)
      time.sleep(self.sleepTime)
      print " Thread %s  's reading finish!" % (self.getName())

      self.CountSemaphore.acquire()
      ReaderCount = ReaderCount - 1
      if (ReaderCount == 0):
          self.WriteSemaphore.release(); 
      #release the  write mutex  
      self.CountSemaphore.release()  
      self.ReadSemaphore.release()


if __name__ == "__main__":  
    threads = []   
    #Semaphore类的一个对象用计数器跟踪获取和释放信号量的线程数量。  

    #semaphore allows five threads to enter critical section  
    threadWriteSem = threading.Semaphore(1)
    threadCountSem = threading.Semaphore(1)
    threadReadSem = threading.Semaphore(2)
    threadAddReadSem = threading.Semaphore(1)

    #threads.append(Writer("thread"+str(0),threadCountSem,threadWriteSem,threadAddReadSem,5))
    #threads.append(Writer("thread"+str(1),threadCountSem,threadWriteSem,threadAddReadSem,5))
    threads.append( Reader("thread"+str(1), threadCountSem, threadWriteSem, threadReadSem, threadAddReadSem, 5) )
    threads.append(Writer("thread"+str(2),threadCountSem,threadWriteSem,threadAddReadSem,3))
    threads.append(Reader("thread"+str(3),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem,3 ))
    #threads.append(Reader("thread"+str(3),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem,3))

    #threads.append(Writer("thread"+str(4),threadCountSem,threadWriteSem,threadAddReadSem))
    #threads.append(Reader("thread"+str(5),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem))

    #for i in range(1,10): 
      #threads.append(Reader("thread"+str(i),threadCountSem,threadWriteSem,threadReadSem,threadAddReadSem))

    #for i in range(1,10): 
      #threads.append(Writer("thread"+str(10+i),threadCountSem,threadWriteSem,threadAddReadSem))  

       #创建一个列表，该列表由SemaphoreThread对象构成，start方法开始列表中的每个线程 
    print len(threads)
    for thread in threads: 
      thread.start() 