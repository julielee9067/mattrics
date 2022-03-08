from pathlib import Path

from gcloud import storage

from core.secrets.secret_config import (
    BREATHING_BUCKET_NAME,
    PRESSURE_BUCKET_NAME,
    SECRET_KEY_FILE_PATH,
)


def upload_pressure_image(image_path: str) -> None:
    client = storage.Client.from_service_account_json(
        json_credentials_path=SECRET_KEY_FILE_PATH
    )
    bucket = client.get_bucket(PRESSURE_BUCKET_NAME)
    blob = bucket.blob(Path(image_path).name)
    blob.upload_from_filename(image_path)


# TODO: MIGHT NEED TO FIX SECRET KEY?
def upload_breathing_image(image_path: str) -> None:
    client = storage.Client.from_service_account_json(
        json_credentials_path=SECRET_KEY_FILE_PATH
    )
    bucket = client.get_bucket(
        BREATHING_BUCKET_NAME
    )  # TODO: ACTUALLY CREATE BUCKET FOR BREATHING
    blob = bucket.blob(Path(image_path).name)
    blob.upload_from_filename(image_path)


if __name__ == "__main__":
    upload_pressure_image(image_path="pressure_data/test.jpeg")
