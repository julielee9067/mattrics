from pathlib import Path

from gcloud import storage

from core.secrets.secret_config import (
    BREATHING_BUCKET_NAME,
    PRESSURE_BUCKET_NAME,
    SECRET_KEY_FILE_PATH,
)


def upload_pressure_image(image_path: str) -> str:
    client = storage.Client.from_service_account_json(
        json_credentials_path=SECRET_KEY_FILE_PATH
    )
    bucket = client.get_bucket(PRESSURE_BUCKET_NAME)

    blob = bucket.blob(Path(image_path).name)
    blob.upload_from_filename(image_path)
    gcs_url = f"https://storage.googleapis.com/{bucket.name}/{blob.name}"
    return gcs_url


def upload_breathing_image(image_path: str) -> str:
    client = storage.Client.from_service_account_json(
        json_credentials_path=SECRET_KEY_FILE_PATH
    )
    bucket = client.get_bucket(BREATHING_BUCKET_NAME)
    blob = bucket.blob(Path(image_path).name)
    blob.upload_from_filename(image_path)
    gcs_url = f"https://storage.googleapis.com/{bucket.name}/{blob.name}"
    return gcs_url


if __name__ == "__main__":
    # print(
    #     upload_pressure_image(
    #         image_path="pressure_data/adam_johnson/adam_johnson_2022_03_10_23_59_28.png"
    #     )
    # )

    print(
        upload_breathing_image(
            image_path="breathing_data/adam_johnson/breathing_trend/adam_johnson_2022_03_11_00_35_01.png"
        )
    )
