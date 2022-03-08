import time

import schedule


def job():
    print("I'm working...")


if __name__ == "__main__":
    schedule.every(1).minute.do(job)
    schedule.every().day.at("08:00").do(job)

    while 1:
        schedule.run_pending()
        time.sleep(1)
