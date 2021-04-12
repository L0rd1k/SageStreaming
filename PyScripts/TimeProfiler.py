import time

LOG_PERIOD = 5
LAST_LOG_TIME = {}

TIME_LIST_SIZE = 20
TIME_LIST = {}

# Usage: @timeProfile('ClassName time: %d ms')
def timeProfile(msg: str):
    def decoWrapper(func):
        def funcWrapper(*args, **kwargs):
            if (LAST_LOG_TIME.get(func) is None):
                LAST_LOG_TIME[func] = time.perf_counter()
                TIME_LIST[func] = []

            start_time = time.perf_counter()
            value = func(*args, **kwargs)
            end_time = time.perf_counter()

            run_time = end_time - start_time
            TIME_LIST[func].append(run_time)
            if (len(TIME_LIST[func]) > TIME_LIST_SIZE):
                TIME_LIST[func].pop(0)

            print(end_time - LAST_LOG_TIME[func])
            if (end_time - LAST_LOG_TIME[func] > LOG_PERIOD):
                LAST_LOG_TIME[func] = end_time
                sum = 0
                for t in TIME_LIST[func]:
                    sum += t
                print(msg % (1000 * (sum / len(TIME_LIST[func]))))
            return value
        return funcWrapper
    return decoWrapper