'''
Created on Jan 15, 2015

@author: sveigri
'''

from threading import Thread, Lock
 
i = 0
lock = Lock()
def someThreadFunction1():
    global i
    for x in range(1000000):
        lock.acquire()
        i += 1
        lock.release()
        
    print("Hello from a thread1!")

def someThreadFunction2():
    global i
    for x in range(1000000):
        lock.acquire()
        i -= 1
        lock.release()
           
    print("Hello from a thread2!")
         
     
# Potentially useful thing:
# In Python you "import" a global variable, instead of "export"ing it when you declare it
# (This is probably an effort to make you feel bad about typing the word "global")
#    global i
 
def main():
    someThread1 = Thread(target = someThreadFunction1, args = (),)
    someThread2 = Thread(target = someThreadFunction2, args = (),)
     
    someThread1.start()
    someThread2.start()
     
    someThread1.join()
    someThread2.join()
     
    print("Hello from main!")
    print(i)
 
main()