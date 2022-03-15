from core.breathing_tracker import get_avg_breathing_rate
from core.breathing_trend_creator import create_breathing_trend
from core.db_connector import DatabaseConnector
from core.heatmap_creator import create_pressure_heatmap
from core.image_uploader import upload_breathing_image, upload_pressure_image


def main(csv_file_name: str):
    # Create pressure heatmap
    pressure_image_path = create_pressure_heatmap(csv_file_name=csv_file_name)

    # calculate breathing data
    average_breathing_rate = get_avg_breathing_rate(csv_file_name=csv_file_name)
    breathing_trend_image_path = create_breathing_trend(csv_file_name=csv_file_name)

    # Upload images to GCS
    gcs_pressure_url = upload_pressure_image(image_path=pressure_image_path)
    gcs_breathing_url = upload_breathing_image(image_path=breathing_trend_image_path)

    # Save info to GCP SQL
    db_connector = DatabaseConnector()
    pressure_id = db_connector.insert_pressure_data(path=gcs_pressure_url)
    breathing_id = db_connector.insert_breathing_data(
        average_rate=average_breathing_rate, path=gcs_breathing_url
    )
    db_connector.insert_daily_data(
        patient_id=1, pressure_id=pressure_id, breathing_id=breathing_id
    )
    db_connector.cursor.close()


if __name__ == "__main__":
    main(csv_file_name="pressure_data/03-11-22.csv")
