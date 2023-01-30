from ctypes import *
import threading
import time

BUFSIZE = 4096
filehelper_dll = CDLL("./lib_file_function.so")
 
#call C function to check connection
filehelper_dll.connect() 

def compileFiles(dirname, write_filename):
    filehelper_dll.concat_files(dirname, write_filename)

def concurrent_files():
    start_time = time.time()
    t1 = threading.Thread(target=compileFiles, args=[b'aclImdb/train/pos', b'train_pos_compiled.txt'])
    t2 = threading.Thread(target=compileFiles, args=[b'aclImdb/train/neg', b'train_neg_compiled.txt'])
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    print("Parallel run time: %.2f seconds" % (time.time() - start_time))  

    
def files_to_array(filename):
  arr = []
  with open(filename) as FileObj:
    for lines in FileObj:
      arr.append(lines)

  return arr

if __name__ == "__main__":
  concurrent_files()
  files_to_array("train_pos_compiled.txt")