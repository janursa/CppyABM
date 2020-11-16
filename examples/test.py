from env import myEnv
import time

obj = myEnv()
obj.setup()
start = time.time()
obj.episode()
end = time.time()
print("Time lapse: {}".format(end-start))