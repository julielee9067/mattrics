import time

import schedule

from core.breathing_tracker import create_breathing_pattern
from core.heatmap_creator import create_pressure_heatmap
from core.image_uploader import upload_breathing_image, upload_pressure_image


def job():
    # TODO:
    #  0. GET PATIENT NAME
    #  If we have multiple patients, we have to update all -> lower priority
    #  1. Create image from the raw data from the database
    #  Check if there is any update from raw data before
    #  4. If updated, then wipe out < today - 3 worth of data
    #  2. Upload image into GCP bucket
    #  3. Save URL from the bucket into the database

    # Get patient name
    patient_name = "Adam Johnson".replace(" ", "_").lower()

    # Create image for pressure
    pressure_path = create_pressure_heatmap(patient_name=patient_name)

    # Create image for breathing
    breathing_path = create_breathing_pattern(patient_name=patient_name)

    # Upload image into GCP bucket
    upload_pressure_image(image_path=pressure_path)
    time.sleep(1)
    upload_breathing_image(image_path=breathing_path)
    time.sleep(1)

    # Get URL from the bucket

    # Save URL into the DB

    print("I'm working...")


def run_schedule(str_time: str):
    schedule.every(1).minute.do(job)
    schedule.every().day.at(str_time).do(job)

    while 1:
        schedule.run_pending()
        time.sleep(1)


if __name__ == "__main__":
    run_schedule(str_time="08:00")
