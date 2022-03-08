import time

import schedule


def job():
    # TODO:
    #  1. Parse raw data
    #  2. Save raw data into the database
    #  3. Create image from the data from the database
    #  4. Upload image into GCP bucket
    #  5. Save URL from the bucket into the database

    print("I'm working...")


if __name__ == "__main__":
    schedule.every(1).minute.do(job)
    schedule.every().day.at("08:00").do(job)

    while 1:
        schedule.run_pending()
        time.sleep(1)
