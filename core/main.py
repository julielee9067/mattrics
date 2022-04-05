from core.breathing_trend_creator import create_breathing_trend
from core.constants import NUM_COL
from core.db_connector import DatabaseConnector
from core.heatmap_creator import (
    create_pressure_heatmap,
    get_final_pressure_data,
    get_pressure_data_from_db,
)
from core.image_uploader import upload_breathing_image, upload_pressure_image
from core.util_functions import create_2d_array_from_1d


def db_main():
    db_connector = DatabaseConnector()
    raw_data = db_connector.get_raw_data()
    data = get_pressure_data_from_db(raw_data=raw_data)
    print(data)
    data = create_2d_array_from_1d(original_array=data, num_col=NUM_COL)
    pressure_image_path = create_pressure_heatmap(data=data)

    breathing_trend_image_path, average_breathing_rate = create_breathing_trend(
        csv_path=""
    )

    # Upload images to GCS
    gcs_pressure_url = upload_pressure_image(image_path=pressure_image_path)
    gcs_breathing_url = upload_breathing_image(image_path=breathing_trend_image_path)

    # Save info to GCP SQL
    db_connector = DatabaseConnector()
    pressure_id = db_connector.insert_pressure_data(path=gcs_pressure_url)
    breathing_id = db_connector.insert_breathing_data(
        average_rate=average_breathing_rate, path=gcs_breathing_url
    )

    db_connector.insert_daily_data(pressure_id=pressure_id, breathing_id=breathing_id)
    db_connector.cursor.close()


def csv_main(csv_path: str = None):
    # Create pressure heatmap
    data = get_final_pressure_data(csv_path=csv_path)
    pressure_image_path = create_pressure_heatmap(data=data)

    # calculate breathing data
    breathing_trend_image_path, average_breathing_rate = create_breathing_trend(
        csv_path=csv_path
    )

    # Upload images to GCS
    gcs_pressure_url = upload_pressure_image(image_path=pressure_image_path)
    gcs_breathing_url = upload_breathing_image(image_path=breathing_trend_image_path)

    # Save info to GCP SQL
    db_connector = DatabaseConnector()
    pressure_id = db_connector.insert_pressure_data(path=gcs_pressure_url)
    breathing_id = db_connector.insert_breathing_data(
        average_rate=average_breathing_rate, path=gcs_breathing_url
    )
    db_connector.insert_daily_data(pressure_id=pressure_id, breathing_id=breathing_id)
    # db_connector.insert_pressure_daily_data(pressure_id=pressure_id)
    db_connector.cursor.close()


if __name__ == "__main__":
    db_main()
    # csv_main(csv_path="pressure_data/fullmat_1.csv")
